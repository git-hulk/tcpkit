#include <stdlib.h>

#include "dumper.h"
#include "sniffer.h"
#include "server.h"

void dump_packet_handler(u_char *file, const struct pcap_pkthdr *header, const u_char *pkt_data) {
    pcap_dump(file, header, pkt_data);
}

struct dumper *dumper_create(struct options *opts, char *err) {
    struct dumper *d;
    pcap_t *pcap;
    pcap_dumper_t *file;
    struct bpf_program *bpf;

    d = malloc(sizeof(*d));
    d->pcap = NULL;
    d->file = NULL;
    d->bpf = NULL;

    pcap = sniffer_online(opts->dev, opts->snaplen, opts->buf_size, err);
    if (!pcap) goto error; 
    d->pcap = pcap;
    bpf = sniffer_compile(pcap, opts->filter, err);
    if (!bpf) goto error; 
    d->bpf = bpf;
    file = pcap_dump_open(pcap, opts->save_file);
    if (!file) goto error; 
    d->file = file;
    return d;

error:    
    dumper_destroy(d);
    return NULL;
}

void dumper_terminate(struct dumper *d) {
    pcap_breakloop(d->pcap);
}

int dumper_run(struct dumper *d) {
    return pcap_loop(d->pcap, 0, dump_packet_handler, (unsigned char *)d->file);
}

void dumper_destroy(struct dumper *d) {
    if (d->pcap) pcap_close(d->pcap);
    if (d->bpf) {
        pcap_freecode(d->bpf);
        free(d->bpf);
    }
    if (d->file) pcap_dump_close(d->file);
    free(d);
}
