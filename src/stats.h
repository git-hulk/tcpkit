#ifndef _STATS_H_
#define _STATS_H_

#include <sys/time.h>
#include <netinet/ip.h>

struct stats {
    uint64_t rx_bytes;
    uint64_t rx_packets;
    uint64_t tx_bytes;
    uint64_t tx_packets;
    uint64_t last_time;
};

struct stats *create_stats(void);
void need_print_stats(struct stats *stats, int interval);
void update_stats(struct stats *stats, int direction, int size);
#endif
