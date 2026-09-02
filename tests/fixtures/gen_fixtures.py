#!/usr/bin/env python3
"""Generate the pcap fixtures used by the test suite.

The frames are assembled by hand so the fixtures stay tiny, reproducible and
capturable without root. Run from this directory: python3 gen_fixtures.py
"""

import struct
import sys

PCAP_MAGIC = 0xA1B2C3D4
DLT_EN10MB = 1

CLIENT_MAC = bytes.fromhex("020000000001")
SERVER_MAC = bytes.fromhex("020000000002")

CLIENT_IP = "10.0.0.1"
SERVER_IP = "10.0.0.2"
RESOLVER_IP = "10.0.0.3"

CLIENT_PORT = 51137
REDIS_PORT = 6379

FIN, SYN, RST, PSH, ACK = 0x01, 0x02, 0x04, 0x08, 0x10


def checksum(data):
    if len(data) % 2:
        data += b"\x00"
    total = 0
    for i in range(0, len(data), 2):
        total += (data[i] << 8) | data[i + 1]
    while total >> 16:
        total = (total & 0xFFFF) + (total >> 16)
    return (~total) & 0xFFFF


def ip_bytes(addr):
    return bytes(int(p) for p in addr.split("."))


def ipv4(src, dst, proto, payload, ihl=5, total_len=None):
    ver_ihl = (4 << 4) | ihl
    length = 20 + len(payload) if total_len is None else total_len
    header = struct.pack(
        ">BBHHHBBH4s4s",
        ver_ihl, 0, length, 0x1234, 0, 64, proto, 0,
        ip_bytes(src), ip_bytes(dst),
    )
    header = header[:10] + struct.pack(">H", checksum(header)) + header[12:]
    return header + payload


def tcp(sport, dport, seq, ack, flags, payload, src, dst, doff=5):
    offset_flags = struct.pack(">BB", (doff << 4), flags)
    header = struct.pack(">HHII", sport, dport, seq, ack) + offset_flags \
        + struct.pack(">HHH", 65535, 0, 0)
    pseudo = ip_bytes(src) + ip_bytes(dst) + struct.pack(">BBH", 0, 6, len(header) + len(payload))
    csum = checksum(pseudo + header + payload)
    header = header[:16] + struct.pack(">H", csum) + header[18:]
    return header + payload


def udp(sport, dport, payload, src, dst, length=None):
    ulen = 8 + len(payload) if length is None else length
    header = struct.pack(">HHHH", sport, dport, ulen, 0)
    pseudo = ip_bytes(src) + ip_bytes(dst) + struct.pack(">BBH", 0, 17, 8 + len(payload))
    csum = checksum(pseudo + header + payload)
    header = header[:6] + struct.pack(">H", csum)
    return header + payload


def ether(src_mac, dst_mac, payload):
    return dst_mac + src_mac + struct.pack(">H", 0x0800) + payload


def to_server(payload, seq, ack, flags):
    return ether(CLIENT_MAC, SERVER_MAC,
                 ipv4(CLIENT_IP, SERVER_IP, 6,
                      tcp(CLIENT_PORT, REDIS_PORT, seq, ack, flags, payload,
                          CLIENT_IP, SERVER_IP)))


def to_client(payload, seq, ack, flags):
    return ether(SERVER_MAC, CLIENT_MAC,
                 ipv4(SERVER_IP, CLIENT_IP, 6,
                      tcp(REDIS_PORT, CLIENT_PORT, seq, ack, flags, payload,
                          SERVER_IP, CLIENT_IP)))


def resp(*args):
    out = "*%d\r\n" % len(args)
    for a in args:
        out += "$%d\r\n%s\r\n" % (len(a), a)
    return out.encode()


def write_pcap(path, packets, snaplen=1500):
    with open(path, "wb") as fh:
        fh.write(struct.pack("<IHHiIII", PCAP_MAGIC, 2, 4, 0, 0, snaplen, DLT_EN10MB))
        for ts, frame, caplen, origlen in packets:
            sec = int(ts)
            usec = int(round((ts - sec) * 1_000_000))
            data = frame[:caplen] if caplen is not None else frame
            wire = len(frame) if origlen is None else origlen
            fh.write(struct.pack("<IIII", sec, usec, len(data), wire))
            fh.write(data)
    print("wrote %s (%d packets)" % (path, len(packets)))


def full(ts, frame):
    return (ts, frame, None, None)


def redis_session():
    """Handshake, two request/response pairs, and one unrelated DNS query."""
    dns = ether(CLIENT_MAC, SERVER_MAC,
                ipv4(CLIENT_IP, RESOLVER_IP, 17,
                     udp(40000, 53, b"\x12\x34\x01\x00" + b"\x00" * 8,
                         CLIENT_IP, RESOLVER_IP)))
    return [
        full(0.000000, to_server(b"", 1000, 0, SYN)),
        full(0.000100, to_client(b"", 5000, 1001, SYN | ACK)),
        full(0.000150, to_server(b"", 1001, 5001, ACK)),
        full(1.000000, to_server(resp("GET", "a"), 1001, 5001, PSH | ACK)),
        full(1.000615, to_client(b"$1\r\nb\r\n", 5001, 1021, PSH | ACK)),
        full(2.000000, to_server(resp("SET", "k", "v"), 1021, 5008, PSH | ACK)),
        full(2.002000, to_client(b"+OK\r\n", 5008, 1052, PSH | ACK)),
        full(3.000000, dns),
    ]


def truncated():
    """Frames whose captured length or header fields promise more than is there."""
    good = to_server(resp("GET", "a"), 1001, 5001, PSH | ACK)
    long_ip = ether(CLIENT_MAC, SERVER_MAC,
                    ipv4(CLIENT_IP, SERVER_IP, 6,
                         tcp(CLIENT_PORT, REDIS_PORT, 1, 1, PSH | ACK, b"hello",
                             CLIENT_IP, SERVER_IP),
                         ihl=15))
    lying_len = ether(CLIENT_MAC, SERVER_MAC,
                      ipv4(CLIENT_IP, SERVER_IP, 6,
                           tcp(CLIENT_PORT, REDIS_PORT, 1, 1, PSH | ACK, b"hello",
                               CLIENT_IP, SERVER_IP),
                           total_len=4000))
    long_tcp = ether(CLIENT_MAC, SERVER_MAC,
                     ipv4(CLIENT_IP, SERVER_IP, 6,
                          tcp(CLIENT_PORT, REDIS_PORT, 1, 1, PSH | ACK, b"hello",
                              CLIENT_IP, SERVER_IP, doff=15)))
    lying_udp = ether(CLIENT_MAC, SERVER_MAC,
                      ipv4(CLIENT_IP, RESOLVER_IP, 17,
                           udp(40000, 53, b"\x12\x34", CLIENT_IP, RESOLVER_IP,
                               length=60000)))
    return [
        # SYN pair first so the redis path registers the server endpoint.
        full(0.000000, to_server(b"", 1000, 0, SYN)),
        full(0.000100, to_client(b"", 5000, 1001, SYN | ACK)),
        (1.000000, good, 8, 74),          # cut inside the ethernet header
        (1.000001, good, 14, 74),         # ethernet only
        (1.000002, good, 24, 74),         # ethernet + partial IP header
        (1.000003, good, 34, 74),         # ethernet + IP, no TCP header
        (1.000004, good, 40, 74),         # ethernet + IP + partial TCP header
        full(1.000005, long_ip),          # IHL claims 60 bytes of IP header
        full(1.000006, long_tcp),         # data offset claims 60 bytes of TCP header
        full(1.000007, lying_len),        # ip_len claims 4000 bytes
        full(1.000008, lying_udp),        # uh_ulen claims 60000 bytes
    ]


def malformed_payload():
    """Well-formed frames carrying payloads that no protocol parser should trust."""
    return [
        full(0.000000, to_server(b"", 1000, 0, SYN)),
        full(0.000100, to_client(b"", 5000, 1001, SYN | ACK)),
        # RESP array that ends mid-token, with no terminating CRLF.
        full(1.000000, to_server(b"*2\r\n$3\r\nGET", 1001, 5001, PSH | ACK)),
        full(1.000500, to_client(b"$1\r\nb\r\n", 5001, 1012, PSH | ACK)),
        # No CRLF anywhere, so a line scanner has nothing to stop on.
        full(2.000000, to_server(b"GETNOCRLFATALL", 1012, 5008, PSH | ACK)),
        full(2.000500, to_client(b"+OK", 5008, 1026, PSH | ACK)),
        # Single byte that only looks like the start of a RESP array.
        full(3.000000, to_server(b"*", 1026, 5011, PSH | ACK)),
        full(3.000500, to_client(b"-ERR", 5011, 1027, PSH | ACK)),
    ]


def stress(connections=4000):
    """Many short-lived connections, to keep the capture thread busy while the
    stats endpoint is polled. Not a committed fixture: it is generated on demand
    by the race check because of its size."""
    global CLIENT_PORT
    packets = []
    ts = 0.0
    for i in range(connections):
        CLIENT_PORT = 20000 + (i % 40000)
        packets.append(full(ts, to_server(b"", 1000, 0, SYN)))
        packets.append(full(ts + 0.0001, to_client(b"", 5000, 1001, SYN | ACK)))
        packets.append(full(ts + 0.001, to_server(resp("GET", "a"), 1001, 5001, PSH | ACK)))
        packets.append(full(ts + 0.002, to_client(b"$1\r\nb\r\n", 5001, 1021, PSH | ACK)))
        ts += 0.01
    return packets


if __name__ == "__main__":
    if len(sys.argv) > 2 and sys.argv[1] == "--stress":
        write_pcap(sys.argv[2], stress())
    else:
        write_pcap("redis-session.pcap", redis_session())
        write_pcap("truncated.pcap", truncated())
        write_pcap("malformed-payload.pcap", malformed_payload())
