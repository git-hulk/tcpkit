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

#ifndef TCPKIT_SERVER_H
#define TCPKIT_SERVER_H
#include "tcpkit.h"

int server_init(server *srv);
void server_deinit(server *srv);
int server_listen(int port);
void server_print_stats(server *srv);
char *server_stats_to_json(server *svr);
void server_create_stats_thread(server *srv);
#endif //TCPKIT_SERVER_H
