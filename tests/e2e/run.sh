#!/bin/sh
# Replays the pcap fixtures through the built binary and checks the output.
set -u

cd "$(dirname "$0")/.." || exit 1

TCPKIT=${TCPKIT:-../src/tcpkit}
STATS_PORT=${STATS_PORT:-33999}
BUSY_PORT=${BUSY_PORT:-33998}
ESC=$(printf '\033')
failures=0

tmpdir=$(mktemp -d) || exit 1
trap 'rm -rf "$tmpdir"' EXIT

# Colour codes are emitted unconditionally, so strip them before matching.
strip_ansi() {
    sed "s/${ESC}\[[0-9;]*m//g"
}

# Leaves the combined output in $out and the exit status in $status.
tcpkit_run() {
    TZ=UTC "$TCPKIT" "$@" > "$tmpdir/out" 2>&1
    status=$?
    out=$(strip_ansi < "$tmpdir/out")
}

replay() {
    fixture=$1
    shift
    tcpkit_run -r "fixtures/$fixture" -P "$STATS_PORT" "$@"
}

ok() { printf '  ok   %s\n' "$1"; }

fail() {
    printf '  FAIL %s\n' "$1"
    failures=$((failures + 1))
}

expect_contains() {
    case $2 in
        *"$3"*) ok "$1" ;;
        *) fail "$1: expected to find \"$3\" in:
$2" ;;
    esac
}

expect_not_contains() {
    case $2 in
        *"$3"*) fail "$1: did not expect to find \"$3\" in:
$2" ;;
        *) ok "$1" ;;
    esac
}

expect_lines() {
    got=$(printf '%s' "$2" | grep -c . )
    if [ "$got" -eq "$3" ]; then
        ok "$1"
    else
        fail "$1: expected $3 lines, got $got"
    fi
}

expect_status() {
    if [ "$2" -eq "$3" ]; then
        ok "$1"
    else
        fail "$1: expected exit status $3, got $2"
    fi
}

echo "== e2e: redis latency"
replay redis-session.pcap -p redis
expect_status "exits cleanly" "$status" 0
expect_lines "reports one line per request" "$out" 2
expect_contains "reports the client and server endpoints" "$out" \
    "10.0.0.1:51137 => 10.0.0.2:6379"
expect_contains "formats the GET command" "$out" "GET a"
expect_contains "formats the SET command" "$out" "SET k v"
expect_contains "reports the GET latency" "$out" "0.615 ms"
expect_contains "reports the SET latency" "$out" "2.000 ms"

echo "== e2e: latency threshold"
replay redis-session.pcap -p redis -t 1
expect_lines "-t drops requests under the threshold" "$out" 1
expect_contains "-t keeps the slow request" "$out" "SET k v"

echo "== e2e: raw mode"
replay redis-session.pcap -p raw
expect_lines "prints every captured packet" "$out" 8

echo "== e2e: udp payload accounting"
replay redis-session.pcap -p raw
expect_contains "the udp header is not counted as payload" "$out" \
    "10.0.0.1.40000 > 10.0.0.1.40000: length 12"

echo "== e2e: malformed frames"
replay truncated.pcap -p raw
expect_status "a capture of malformed frames exits cleanly" "$status" 0
expect_lines "only the two well formed frames are reported" "$out" 2
expect_not_contains "no negative payload length is reported" "$out" "length -"
expect_not_contains "no payload length beyond the frame is reported" "$out" "length 3960"
expect_not_contains "no udp length beyond the frame is reported" "$out" "length 59992"

echo "== e2e: capture filter"
replay redis-session.pcap -p raw udp
expect_lines "a udp filter keeps only the dns packet" "$out" 1
expect_contains "the kept packet is the dns query" "$out" "10.0.0.1.40000"

replay redis-session.pcap -p raw "tcp port 9999"
expect_lines "a filter matching nothing prints nothing" "$out" 0

replay redis-session.pcap -p raw "tcp port 6379"
expect_lines "a matching filter keeps the tcp packets" "$out" 7

replay redis-session.pcap -p redis udp
expect_lines "a udp filter suppresses the redis latency lines" "$out" 0

replay redis-session.pcap -p redis "tcp port 6379"
expect_lines "a matching filter keeps the redis latency lines" "$out" 2

echo "== e2e: error handling"
replay does-not-exist.pcap -p redis
expect_status "a missing capture file exits with an error" "$status" 1
expect_contains "a missing capture file is named in the error" "$out" \
    "does-not-exist.pcap"

replay redis-session.pcap -p redis "not a filter"
expect_status "an unparsable filter exits with an error" "$status" 1
expect_contains "an unparsable filter is reported" "$out" "filter expression"

tcpkit_run -r fixtures/redis-session.pcap -Z
expect_status "an unknown option exits with an error" "$status" 1
expect_contains "an unknown option is named in the error" "$out" "-Z"

replay redis-session.pcap -p bogus
expect_status "an unknown protocol exits with an error" "$status" 1

echo "== e2e: unavailable stats port"
# The holder takes an ephemeral port and prints it, so the check never races
# another test run for a fixed port number.
if command -v python3 >/dev/null 2>&1; then
    python3 -c "import socket, sys, time
s = socket.socket()
s.bind(('0.0.0.0', 0))
s.listen(1)
sys.stdout.write('%d\\n' % s.getsockname()[1])
sys.stdout.flush()
time.sleep(60)" > "$tmpdir/port" 2>/dev/null &
    holder=$!

    busy_port=""
    i=0
    while [ $i -lt 50 ]; do
        busy_port=$(cat "$tmpdir/port" 2>/dev/null)
        [ -n "$busy_port" ] && break
        sleep 0.1
        i=$((i + 1))
    done

    if [ -n "$busy_port" ]; then
        tcpkit_run -r fixtures/redis-session.pcap -P "$busy_port" -p redis
        expect_status "a busy stats port does not abort the capture" "$status" 0
        expect_contains "a busy stats port is reported" "$out" "Failed to bind stats port"
        expect_contains "the capture still runs" "$out" "GET a"
    else
        echo "  skip busy stats port (could not reserve a port)"
    fi

    kill "$holder" 2>/dev/null
    wait "$holder" 2>/dev/null
else
    echo "  skip busy stats port (python3 not found)"
fi

if [ "$failures" -ne 0 ]; then
    echo "e2e: $failures failed"
    exit 1
fi
echo "e2e: all checks passed"
