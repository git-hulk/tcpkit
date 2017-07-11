#include <stdint.h>
#include "bandwidth.h"
#include "tcpkit.h"

void need_report_bandwidth()
{
    uint64_t now_ts, delta;
    struct timeval now;
    char in_speed_buf[16], out_speed_buf[16];
    struct options *opts;
    struct bandwidth *bw;

    opts = get_options();
    bw = get_bandwidth(); 
    gettimeofday(&now, NULL);
    now_ts = now.tv_sec * 1000000 + now.tv_usec;
    delta = now_ts - bw->last_calc_time;
    if (delta >= opts->duration * 1000000) {
        // trans to second
        delta /= 1000000;
        speed_human(bw->in_bytes / delta, in_speed_buf, 16);
        speed_human(bw->out_bytes / delta, out_speed_buf, 16);
        if (bw->last_calc_time) {
            printf("Incoming %s, %llu packets/s, Outgoing %s, %llu packets/s\n", 
                    in_speed_buf,
                    (unsigned long long)bw->in_packets / delta,
                    out_speed_buf,
                    (unsigned long long)bw->out_packets / delta
                  );
        }
        // reset bandwith
        bw->last_calc_time = now_ts;
        bw->in_bytes = bw->out_bytes = 0;
        bw->in_packets = bw->out_packets = 0;
    }
}

void
calc_bandwidth(const struct ip *ip, const struct timeval *tv) {
    struct bandwidth *bw;

    bw = get_bandwidth();
    need_report_bandwidth();
    if (is_local_address(ip->ip_dst)) {
        bw->in_bytes += htons(ip->ip_len);
        bw->in_packets += 1;
    } else {
        bw->out_bytes += htons(ip->ip_len);
        bw->out_packets += 1;
    }
}
