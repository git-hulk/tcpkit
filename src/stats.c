#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include "stats.h"
#include "tcpkit.h"

struct stats *create_stats(void) {
    struct timeval tv;
    struct stats *stats;

    gettimeofday(&tv, NULL);
    stats = malloc(sizeof(*stats));
    stats->rx_bytes = 0;
    stats->tx_bytes = 0;
    stats->rx_packets = 0;
    stats->tx_packets = 0;
    stats->last_time = tv.tv_sec * 1000000 + tv.tv_usec;
    return stats;
}

void need_print_stats(struct stats *stats, int interval) {
    uint64_t now, delta;
    struct timeval tv;
    char rx_speed_buf[16], tx_speed_buf[16];

    gettimeofday(&tv, NULL);
    now = tv.tv_sec * 1000000 + tv.tv_usec;
    delta = now - stats->last_time;
    if (stats->last_time > 0 && delta && delta >= interval * 1000000) {
        delta /= 1000000;
        speed_human(stats->rx_bytes / delta, rx_speed_buf, 16);
        speed_human(stats->tx_bytes / delta, tx_speed_buf, 16);
        printf("Incoming %s, %llu packets/s, Outgoing %s, %llu packets/s\n",
                rx_speed_buf,
                (unsigned long long)stats->rx_packets / delta,
                tx_speed_buf,
                (unsigned long long)stats->tx_packets / delta
        );
        stats->rx_bytes = 0;
        stats->tx_bytes = 0;
        stats->rx_packets = 0;
        stats->tx_packets = 0;
        stats->last_time = now;
    }
}

void
update_stats(struct stats *stats, int direction, int size) {
    if (direction == 1) {
        stats->rx_bytes += size;
        ++stats->rx_packets;
    } else {
        stats->tx_bytes += size;
        ++stats->tx_packets;
    }
}
