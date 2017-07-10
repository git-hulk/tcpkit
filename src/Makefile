CC = gcc
CFLAGS = -g -Wall
CINCLUDES = -I../deps/libpcap/ -I../deps/lua/src
CLIBS = ../deps/libpcap/libpcap.a ../deps/lua/src/liblua.a
LOAD_LIBS=-lm -ldl

INSTALL=/usr/bin/install
INSTALLDIR=/usr/local
BINDIR=$(INSTALLDIR)/bin

TCPKIT_CC = $(CC) $(CFLAGS) $(CINCLUDES)

PROG = tcpkit


OBJS = local_addresses.o util.o cJSON.o pcap_wrapper.o packet.o script.o tcpkit.o array.o bandwidth.o

all: deps tcpkit
.PHONY: all

include dep.mk
tcpkit: $(OBJS) 
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(CINCLUDES) $(CLIBS) $(LOAD_LIBS)

%.o: %.c
	$(TCPKIT_CC) -c $<

dep:
	$(TCPKIT_CC) -MM *.c > dep.mk

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
	@cd ../deps/libpcap/  && ./configure && make
	@cd ../deps/lua/src && make $(os_Platform)

clean:
	- rm -rf $(PROG) && rm -rf  *.o 
	@cd ../deps/libpcap/ && make clean
	@cd ../deps/lua/src/ && make clean
nop:

install:
	mkdir -p $(BINDIR)
	$(INSTALL) $(PROG) $(BINDIR)
