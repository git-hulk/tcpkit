#include <errno.h>
#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include "pcap_wrapper.h"
#include "util.h"
#include "tcpkit.h"
#include "bandwidth.h"

pcap_wrapper* 
pw_create(char *dev) 
{
    pcap_t *pcap;
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        return NULL;
    }

    // promisc = 0, don't put into promiscuous mode
    pcap = pcap_open_live(dev, CAPTURE_LENGTH, 0, READ_TIMEOUT, errbuf);
    if(!pcap) {
        if (strcmp(dev, "any") == 0) {
            dev = pcap_lookupdev(errbuf);
            pcap = pcap_open_live(dev, CAPTURE_LENGTH, 0, READ_TIMEOUT, errbuf);
        }
        if (!pcap) return NULL;
    }
 
    logger(INFO, "listening on device %s", dev);
    pcap_wrapper *pw = malloc(sizeof(*pw));
    pw->pcap = pcap;
    pw->net = net;
    pw->mask = mask;
    return pw; 
}

pcap_wrapper *
pw_create_offline(const char *filename)
{
    FILE *fp;
    pcap_t *pcap;
    pcap_wrapper *pw;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (!filename)  return NULL;

    fp = fopen(filename, "r");
    if (!fp) {
        logger(ERROR, "open offline file %s error as %s", filename, strerror(errno));
        return NULL;
    }
    pcap = pcap_fopen_offline(fp, errbuf);
    if (!pcap) {
        logger(ERROR, "pcap: %s\n", errbuf);
        return NULL;
    }
    pw = malloc(sizeof(*pw));
    pw->pcap = pcap;
    return pw;
}

void
pw_release (pcap_wrapper* pw) {
    pcap_breakloop(pw->pcap);
    pcap_close(pw->pcap);
    free(pw);
}

int 
pcap_set_filter (pcap_wrapper* pw, char *filter)
{
    struct bpf_program fp;

    if (pcap_compile(pw->pcap, &fp, filter, 0, 1) == -1) {
        // Can't parse filter.
        return -2;
    }
    
    // return 0 when sucess, return -1 when failed.
    return pcap_setfilter(pw->pcap, &fp); 
}

int
core_loop(pcap_wrapper *pw, char *filter, pcap_handler handler) {
    int ret;
    struct options *opts;

    if(! pw) return -1;

    if(filter && pcap_set_filter(pw, filter) != 0) {
        // install filter failed.
        return -1;    
    }
    
    logger(INFO, "start capturing, filter is = [%s]", filter);
    opts = get_options();
    if (opts->is_calc_mode) {
        // default calculate bandwidth every 30 second.
        opts->duration = opts->duration >= 1 ? opts->duration : 30;
        while((ret = pcap_dispatch(pw->pcap, -1, handler, (unsigned char *) pw)) >= 0) {
            need_report_bandwidth();
        }
        return ret;
    } else {
        return pcap_loop(pw->pcap, -1, handler, (unsigned char *) pw); 
    }
}
