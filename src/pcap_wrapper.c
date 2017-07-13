#include <errno.h>
#include <pcap.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "tcpkit.h"
#include "stats.h"
#include "pcap_wrapper.h"

pcap_t*
open_pcap_by_device(const char *dev, char *errbuf) {
    pcap_t *pcap;
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    const int timeout = 100;
    const int capture_length = 65535;

    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        return NULL;
    }
    pcap = pcap_open_live(dev, capture_length, 0, timeout, errbuf);
    if(pcap || strcmp(dev, "any")) {
        return pcap;
    }
    dev = pcap_lookupdev(errbuf);
    if (dev) {
        struct options *opts = get_options();
        free(opts->device);
        opts->device = strdup(dev);
        pcap = pcap_open_live(dev, capture_length, 0, timeout, errbuf);
    }
    return pcap;
}

pcap_t*
open_pcap_by_offline(const char *filename, char *errbuf) {
    FILE *fp;

    if (!(fp = fopen(filename, "r"))) {
        strcpy(errbuf, strerror(errno));
        return NULL;
    }
    return pcap_fopen_offline(fp, errbuf);
}

void
close_pcap(pcap_t* pcap) {
    pcap_breakloop(pcap);
    pcap_close(pcap);
}

int
core_loop(pcap_t *pcap, const char *filter, pcap_handler handler) {
    struct bpf_program fp;

    if (!pcap || pcap_compile(pcap, &fp, filter, 0, 1) != 0
        || pcap_setfilter(pcap, &fp) != 0) {
        return -1;
    }
    return pcap_loop(pcap, -1, handler, (unsigned char *)pcap);
}
