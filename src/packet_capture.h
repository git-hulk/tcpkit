#ifndef _PACKET_CAPTURE_H_
#define _PACKET_CAPTURE_H_

#include <pcap.h>

#define CAPTURE_LENGTH 65535
#define READ_TIMEOUT 1000 // ms

typedef struct {
    pcap_t *pcap;
    bpf_u_int32 mask;
    bpf_u_int32 net;
} pcap_wrapper;

pcap_wrapper* pw_create(char *dev);
void pw_release (pcap_wrapper* pw);
int pcap_set_filter (pcap_wrapper* pw, char *filter);
int core_loop(pcap_wrapper *pw, char *filter, pcap_handler handler); 

#endif
