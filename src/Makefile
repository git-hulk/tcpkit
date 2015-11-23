CC = gcc
CFLAGS = -g -Wall
CINCLUDES = -I../deps/libpcap/ -I../deps/lua/src
CLIBS = ../deps/libpcap/libpcap.a ../deps/lua/src/liblua.a

INSTALL=/usr/bin/install
INSTALLDIR=/usr/local
BINDIR=$(INSTALLDIR)/bin

OBJS = local_addresses.o util.o cJSON.o packet_capture.o packet.o script.o tcpkit.o
PROG = tcpkit
all: deps tcpkit

tcpkit: $(OBJS) 
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(CINCLUDES) $(CLIBS)

cJSON.o: cJSON.c cJSON.h
local_addresses.o: local_addresses.c util.h
packet.o: packet.c packet.h tcpkit.h packet_capture.h script.h util.h \
  cJSON.h local_addresses.h
packet_capture.o: packet_capture.c packet_capture.h util.h
script.o: script.c script.h
tcpkit.o: tcpkit.c tcpkit.h packet_capture.h script.h util.h packet.h \
  local_addresses.h
util.o: util.c util.h

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
ifeq ($(uname_S),SunOS)
	os_Platform := solaris 
else
ifeq ($(uname_S),Darwin)
	os_Platform := macosx
else
ifeq ($(uname_S),AIX)
	os_Platform := aix
else
	os_Platform := linux
endif
endif
endif
	
deps: nop 
	#@cd ../deps/libpcap/  && ./configure && make
	#@cd ../deps/lua/src && make $(os_Platform)

clean:
	- rm -rf $(PROG) && rm -rf  *.o 
	#@cd ../deps/libpcap/ && make clean
	#@cd ../deps/lua/src/ && make clean
nop:

install:
	mkdir -p $(BINDIR)
	$(INSTALL) $(PROG) $(BINDIR)
