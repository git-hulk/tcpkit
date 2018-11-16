/**
 *   tcpkit --  toolkit to analyze tcp packet
 *   Copyright (C) 2018  @git-hulk
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 **/

#ifndef TCPKIT_SNIFFER_H
#define TCPKIT_SNIFFER_H

#include <pcap.h>

pcap_t *sniffer_packet_online(char **device, int buffer_size, char *err_buf);
pcap_t *sniffer_packet_offline(const char *file, char *err_buf);
int sniffer_loop(pcap_t *sniffer, const char *filter, pcap_handler handler, void *user);
void sniffer_terminate(pcap_t *sniffer);

#endif //TCPKIT_SNIFFER_H
