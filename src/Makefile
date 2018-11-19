CC = gcc
CFLAGS = -g -Wall
CINCLUDES = -I../deps/libpcap/ -I../deps/lua/src
CLIBS = ../deps/libpcap/libpcap.a ../deps/lua/src/liblua.a
LOAD_LIBS=-lm -ldl -lpthread

INSTALL=/usr/bin/install
INSTALLDIR=/usr/local
BINDIR=$(INSTALLDIR)/bin

TCPKIT_CC = $(CC) $(CFLAGS) $(CINCLUDES)

PROG = tcpkit


OBJS = util.o array.o hashtable.o logger.o packet.o redis.o server.o sniffer.o stats.o tcpkit.o vm.o

all: deps tcpkit
.PHONY: all

include dep.mk

tcpkit: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(CINCLUDES) $(CLIBS) $(LOAD_LIBS)

%.o: %.c
	$(TCPKIT_CC) -c $<

dep.mk:
	$(TCPKIT_CC) -MM *.c > dep.mk

deps: nop
	@cd ../deps/libpcap/  && ./configure --enable-dbus=no && make
	@cd ../deps/lua/src && make a

clean:
	- rm -rf $(PROG) && rm -rf  *.o
	@cd ../deps/libpcap/ && make clean
	@cd ../deps/lua/src/ && make clean
nop:

install:
	mkdir -p $(BINDIR)
	$(INSTALL) $(PROG) $(BINDIR)
