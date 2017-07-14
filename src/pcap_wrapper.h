#ifndef _PCAP_WRAPPER_H_
#define _PCAP_WRAPPER_H_

#include <pcap.h>

pcap_t* open_pcap_by_device(const char *dev, char *errbuf);
pcap_t* open_pcap_by_offline(const char *filename, char *buf);
int core_loop(pcap_t *pcap, const char *filter, pcap_handler handler, void *private);
#endif
