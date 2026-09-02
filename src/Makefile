PROG = tcpkit

all: $(PROG)
.PHONY: all

CC = gcc
OPTIMIZATION ?= -O2
CFLAGS = -g $(OPTIMIZATION) -Wall

# SANITIZE=address builds the binary and the test suite under ASan/UBSan.
ifdef SANITIZE
CFLAGS += -fsanitize=$(SANITIZE) -fno-omit-frame-pointer
endif

LUA_DIR = ../deps/lua/src
LUA_LIB = $(LUA_DIR)/liblua.a

include ../mk/pcap.mk

CINCLUDES = $(PCAP_INCLUDES) -I$(LUA_DIR)
CLIBS = $(PCAP_LIBS) $(LUA_LIB)
LOAD_LIBS = -lm -ldl -lpthread

INSTALL=/usr/bin/install
INSTALLDIR=/usr/local
BINDIR=$(INSTALLDIR)/bin

TCPKIT_CC = $(CC) $(CFLAGS) $(CINCLUDES)

OBJS = tcpkit.o packet.o sniffer.o server.o lua.o \
	   hashtable.o protocol.o dumper.o log.o cJSON.o stats.o

include dep.mk

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROG) $(OBJS) $(CLIBS) $(LOAD_LIBS)

$(OBJS): | $(PCAP_DEPS) $(LUA_LIB)

%.o: %.c
	$(TCPKIT_CC) -c $<

dep.mk:
	$(TCPKIT_CC) -MM *.c > dep.mk

$(LUA_LIB):
	cd $(LUA_DIR) && $(MAKE) a

lint:
	sh cppcheck.sh

clean:
	- rm -rf $(PROG) dep.mk *.o

clean-deps:
	- cd $(PCAP_DIR) && test -f Makefile && $(MAKE) clean
	- cd $(LUA_DIR) && $(MAKE) clean

install:
	mkdir -p $(BINDIR)
	$(INSTALL) $(PROG) $(BINDIR)

.PHONY: lint clean clean-deps install
