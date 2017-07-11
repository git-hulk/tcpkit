#ifndef _BANDWIDTH_H_
#define _BANDWIDTH_H_
#include <sys/time.h>
#include <netinet/ip.h>
void need_report_bandwidth();
void calc_bandwidth(const struct ip *ip, const struct timeval *tv);
#endif
