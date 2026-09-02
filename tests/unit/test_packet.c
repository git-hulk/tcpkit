#include <stdlib.h>
#include <arpa/inet.h>
#include "tk_test.h"
#include "packet.h"

#define SPORT 51137
#define DPORT 6379

/* A capture is modelled as an exactly sized heap allocation, so any read past
 * the bytes the parser was told about lands in an AddressSanitizer redzone. */
struct capture {
    unsigned char *bytes;
    int caplen;
    int wirelen;
};

struct spec {
    int ip_hl_words;     /* 0 selects the usual 5 */
    int l4_words;        /* 0 selects the usual 5, tcp only */
    int ip_len_field;    /* 0 computes it from the payload */
    int udp_len_field;   /* 0 computes it from the payload */
    const char *payload;
    int payload_len;
    int caplen;          /* 0 captures the whole frame */
    int wirelen;         /* 0 uses the whole frame */
};

static struct capture cut(const unsigned char *full, int total, struct spec s) {
    struct capture c;

    c.caplen = s.caplen > 0 && s.caplen < total ? s.caplen : total;
    c.wirelen = s.wirelen > 0 ? s.wirelen : total;
    c.bytes = malloc(c.caplen);
    memcpy(c.bytes, full, c.caplen);
    return c;
}

static void fill_ip(struct ip *ip, int hl_words, int total_len, int proto) {
#ifdef _IP_VHL
    ip->ip_vhl = (4 << 4) | hl_words;
#else
    ip->ip_hl = hl_words;
    ip->ip_v = 4;
#endif
    ip->ip_len = htons(total_len);
    ip->ip_p = proto;
    inet_pton(AF_INET, "10.0.0.1", &ip->ip_src);
    inet_pton(AF_INET, "10.0.0.2", &ip->ip_dst);
}

static struct capture build_tcp(struct spec s) {
    unsigned char full[512];
    struct ip *ip;
    struct tcphdr *tcp;
    int hl = s.ip_hl_words ? s.ip_hl_words : 5;
    int doff = s.l4_words ? s.l4_words : 5;
    int iphdr = hl * 4, tcphdr = doff * 4;
    int total = iphdr + tcphdr + s.payload_len;

    memset(full, 0, sizeof(full));
    ip = (struct ip *)full;
    fill_ip(ip, hl, s.ip_len_field ? s.ip_len_field : total, IPPROTO_TCP);

    tcp = (struct tcphdr *)(full + iphdr);
#if defined(__FAVOR_BSD) || defined(__APPLE__)
    tcp->th_sport = htons(SPORT);
    tcp->th_dport = htons(DPORT);
    tcp->th_seq = htonl(1001);
    tcp->th_ack = htonl(5001);
    tcp->th_flags = 0x18;
    tcp->th_win = htons(65535);
    tcp->th_off = doff;
#else
    tcp->source = htons(SPORT);
    tcp->dest = htons(DPORT);
    tcp->seq = htonl(1001);
    tcp->ack_seq = htonl(5001);
    tcp->psh = 1;
    tcp->ack = 1;
    tcp->window = htons(65535);
    tcp->doff = doff;
#endif
    if (s.payload_len) memcpy(full + iphdr + tcphdr, s.payload, s.payload_len);
    return cut(full, total, s);
}

static struct capture build_udp(struct spec s) {
    unsigned char full[512];
    struct ip *ip;
    struct udphdr *udp;
    int hl = s.ip_hl_words ? s.ip_hl_words : 5;
    int iphdr = hl * 4;
    int udp_len = 8 + s.payload_len;
    int total = iphdr + udp_len;

    memset(full, 0, sizeof(full));
    ip = (struct ip *)full;
    fill_ip(ip, hl, s.ip_len_field ? s.ip_len_field : total, IPPROTO_UDP);

    udp = (struct udphdr *)(full + iphdr);
#if defined(__FAVOR_BSD) || defined(__APPLE__)
    udp->uh_sport = htons(40000);
    udp->uh_dport = htons(53);
    udp->uh_ulen = htons(s.udp_len_field ? s.udp_len_field : udp_len);
#else
    udp->source = htons(40000);
    udp->dest = htons(53);
    udp->len = htons(s.udp_len_field ? s.udp_len_field : udp_len);
#endif
    if (s.payload_len) memcpy(full + iphdr + 8, s.payload, s.payload_len);
    return cut(full, total, s);
}

/* Touches every byte the parser reported as payload. */
static int sum_payload(const struct user_packet *p) {
    int i, sum = 0;

    for (i = 0; i < p->payload_size; i++) sum += (unsigned char)p->payload[i];
    return sum;
}

static int parse_tcp(struct capture c, struct user_packet *p) {
    struct timeval tv;

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    memset(p, 0, sizeof(*p));
    return process_tcp_packet(tv, (const struct ip *)c.bytes, c.caplen, c.wirelen, p);
}

static int parse_udp(struct capture c, struct user_packet *p) {
    struct timeval tv;

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    memset(p, 0, sizeof(*p));
    return process_udp_packet(tv, (const struct ip *)c.bytes, c.caplen, c.wirelen, p);
}

static void test_tcp_parses_a_complete_packet(void) {
    struct spec s;
    struct capture c;
    struct user_packet p;

    memset(&s, 0, sizeof(s));
    s.payload = "PING\r\n";
    s.payload_len = 6;
    c = build_tcp(s);

    TK_EQ_INT(parse_tcp(c, &p), 0);
    TK_EQ_INT(p.port_src, SPORT);
    TK_EQ_INT(p.port_dst, DPORT);
    TK_EQ_INT(p.seq, 1001);
    TK_EQ_INT(p.ack, 5001);
    TK_EQ_INT(p.is_tcp, 1);
    TK_EQ_INT(p.payload_size, 6);
    TK_EQ_INT(p.wire_payload_size, 6);
    TK_EQ_INT(sum_payload(&p), 'P' + 'I' + 'N' + 'G' + '\r' + '\n');
    free(c.bytes);
}

static void test_tcp_rejects_a_truncated_header(void) {
    struct spec s;
    struct capture c;
    struct user_packet p;
    int cuts[] = { 20, 24, 30, 39 };
    unsigned i;

    for (i = 0; i < sizeof(cuts) / sizeof(cuts[0]); i++) {
        memset(&s, 0, sizeof(s));
        s.payload = "PING";
        s.payload_len = 4;
        s.caplen = cuts[i];
        c = build_tcp(s);
        TK_EQ_INT(parse_tcp(c, &p), -1);
        free(c.bytes);
    }
}

static void test_tcp_rejects_a_header_longer_than_the_capture(void) {
    struct spec s;
    struct capture c;
    struct user_packet p;

    /* IHL claims 60 bytes of IP header, only 44 were captured. */
    memset(&s, 0, sizeof(s));
    s.ip_hl_words = 15;
    s.payload = "PING";
    s.payload_len = 4;
    s.caplen = 44;
    c = build_tcp(s);
    TK_EQ_INT(parse_tcp(c, &p), -1);
    free(c.bytes);

    /* Data offset claims 60 bytes of TCP header, only 44 were captured. */
    memset(&s, 0, sizeof(s));
    s.l4_words = 15;
    s.payload = "PING";
    s.payload_len = 4;
    s.caplen = 44;
    c = build_tcp(s);
    TK_EQ_INT(parse_tcp(c, &p), -1);
    free(c.bytes);
}

static void test_tcp_rejects_a_length_beyond_the_frame(void) {
    struct spec s;
    struct capture c;
    struct user_packet p;

    memset(&s, 0, sizeof(s));
    s.ip_len_field = 4000;
    s.payload = "PING";
    s.payload_len = 4;
    c = build_tcp(s);

    TK_EQ_INT(parse_tcp(c, &p), -1);
    free(c.bytes);
}

static void test_tcp_rejects_a_length_under_the_headers(void) {
    struct spec s;
    struct capture c;
    struct user_packet p;

    memset(&s, 0, sizeof(s));
    s.ip_len_field = 20;    /* no room for the tcp header it carries */
    s.payload = "PING";
    s.payload_len = 4;
    c = build_tcp(s);

    TK_EQ_INT(parse_tcp(c, &p), -1);
    free(c.bytes);
}

static void test_tcp_clamps_the_payload_to_the_captured_bytes(void) {
    struct spec s;
    struct capture c;
    struct user_packet p;

    /* 40 bytes of payload on the wire, 10 of them captured. */
    memset(&s, 0, sizeof(s));
    s.payload = "0123456789abcdefghijklmnopqrstuvwxyz0123";
    s.payload_len = 40;
    s.caplen = 50;
    c = build_tcp(s);

    TK_EQ_INT(parse_tcp(c, &p), 0);
    TK_EQ_INT(p.wire_payload_size, 40);
    TK_EQ_INT(p.payload_size, 10);
    TK_EQ_INT(sum_payload(&p), '0'+'1'+'2'+'3'+'4'+'5'+'6'+'7'+'8'+'9');
    free(c.bytes);
}

static void test_udp_parses_a_complete_packet(void) {
    struct spec s;
    struct capture c;
    struct user_packet p;

    memset(&s, 0, sizeof(s));
    s.payload = "\x12\x34\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    s.payload_len = 12;
    c = build_udp(s);

    TK_EQ_INT(parse_udp(c, &p), 0);
    TK_EQ_INT(p.port_src, 40000);
    TK_EQ_INT(p.port_dst, 53);
    TK_EQ_INT(p.is_tcp, 0);
    /* The udp header is not payload. */
    TK_EQ_INT(p.payload_size, 12);
    TK_EQ_INT(p.wire_payload_size, 12);
    TK_EQ_INT(sum_payload(&p), 0x12 + 0x34 + 0x01);
    free(c.bytes);
}

static void test_udp_rejects_a_truncated_header(void) {
    struct spec s;
    struct capture c;
    struct user_packet p;

    memset(&s, 0, sizeof(s));
    s.payload = "query";
    s.payload_len = 5;
    s.caplen = 24;   /* ip header plus half a udp header */
    c = build_udp(s);

    TK_EQ_INT(parse_udp(c, &p), -1);
    free(c.bytes);
}

static void test_udp_rejects_a_length_beyond_the_frame(void) {
    struct spec s;
    struct capture c;
    struct user_packet p;

    memset(&s, 0, sizeof(s));
    s.udp_len_field = 60000;
    s.payload = "query";
    s.payload_len = 5;
    c = build_udp(s);

    TK_EQ_INT(parse_udp(c, &p), -1);
    free(c.bytes);
}

static void test_udp_rejects_a_length_under_its_own_header(void) {
    struct spec s;
    struct capture c;
    struct user_packet p;

    memset(&s, 0, sizeof(s));
    s.udp_len_field = 4;
    s.payload = "query";
    s.payload_len = 5;
    c = build_udp(s);

    TK_EQ_INT(parse_udp(c, &p), -1);
    free(c.bytes);
}

static void free_test_request(void *value) {
    struct request *req = (struct request *)value;

    free(req->payload);
    free(req);
}

static void add_request(struct hashtable *requests, const char *key, long sec) {
    struct request *req = malloc(sizeof(*req));

    req->tv.tv_sec = sec;
    req->tv.tv_usec = 0;
    req->seq = 1;
    req->payload = malloc(8);
    memcpy(req->payload, "GET a", 6);
    req->size = 5;
    hashtable_add(requests, (char *)key, req);
}

static void test_expiry_drops_only_the_stale_requests(void) {
    struct hashtable *requests = hashtable_create(16);
    struct timeval now;

    requests->free = free_test_request;
    add_request(requests, "old", 100);
    add_request(requests, "borderline", 150);
    add_request(requests, "fresh", 195);

    now.tv_sec = 200;
    now.tv_usec = 0;

    /* A 60s timeout: 100 is 100s old, 150 is 50s old, 195 is 5s old. */
    TK_EQ_INT(requests_expire(requests, now, 60000000), 1);
    TK_EQ_INT(requests->size, 2);
    TK_CHECK(hashtable_get(requests, "old") == NULL, "the stale request must go");
    TK_CHECK(hashtable_get(requests, "borderline") != NULL, "50s is not stale yet");
    TK_CHECK(hashtable_get(requests, "fresh") != NULL, "5s is not stale");

    /* Later still, and everything but the newest has aged out. */
    now.tv_sec = 260;
    TK_EQ_INT(requests_expire(requests, now, 60000000), 2);
    TK_EQ_INT(requests->size, 0);

    hashtable_destroy(requests);
}

static void test_expiry_keeps_requests_when_time_goes_backwards(void) {
    struct hashtable *requests = hashtable_create(16);
    struct timeval now;

    requests->free = free_test_request;
    add_request(requests, "later", 300);

    /* Out of order capture timestamps must not evict anything. */
    now.tv_sec = 200;
    now.tv_usec = 0;
    TK_EQ_INT(requests_expire(requests, now, 60000000), 0);
    TK_EQ_INT(requests->size, 1);

    hashtable_destroy(requests);
}

int main(void) {
    TK_RUN(test_tcp_parses_a_complete_packet);
    TK_RUN(test_tcp_rejects_a_truncated_header);
    TK_RUN(test_tcp_rejects_a_header_longer_than_the_capture);
    TK_RUN(test_tcp_rejects_a_length_beyond_the_frame);
    TK_RUN(test_tcp_rejects_a_length_under_the_headers);
    TK_RUN(test_tcp_clamps_the_payload_to_the_captured_bytes);
    TK_RUN(test_udp_parses_a_complete_packet);
    TK_RUN(test_udp_rejects_a_truncated_header);
    TK_RUN(test_udp_rejects_a_length_beyond_the_frame);
    TK_RUN(test_udp_rejects_a_length_under_its_own_header);
    TK_RUN(test_expiry_drops_only_the_stale_requests);
    TK_RUN(test_expiry_keeps_requests_when_time_goes_backwards);
    return tk_report("packet");
}
