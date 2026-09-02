CC = gcc
OPTIMIZATION ?= -O2
CFLAGS = -g $(OPTIMIZATION) -Wall

LUA_DIR = ../deps/lua/src
LUA_LIB = $(LUA_DIR)/liblua.a
PCAP_DIR = ../deps/libpcap
PCAP_LIB = $(PCAP_DIR)/libpcap.a

# The vendored libpcap (1.9.0-PRE-GIT) forces `-arch x86_64` on every darwin
# release, so prefer a system libpcap when one is installed. Set
# USE_SYSTEM_PCAP=0 to build against the vendored copy instead.
PCAP_PROBE = printf '%s\n' '\#include <pcap.h>' 'int main(void){pcap_lib_version();return 0;}' \
	| $(CC) -x c - -lpcap -o /dev/null
USE_SYSTEM_PCAP ?= $(shell $(PCAP_PROBE) >/dev/null 2>&1 && echo 1 || echo 0)

ifeq ($(USE_SYSTEM_PCAP),1)
PCAP_INCLUDES =
PCAP_LIBS = -lpcap
PCAP_DEPS =
else
PCAP_INCLUDES = -I$(PCAP_DIR)
PCAP_LIBS = $(PCAP_LIB)
PCAP_DEPS = $(PCAP_LIB)
endif

CINCLUDES = $(PCAP_INCLUDES) -I$(LUA_DIR)
CLIBS = $(PCAP_LIBS) $(LUA_LIB)
LOAD_LIBS = -lm -ldl -lpthread

INSTALL=/usr/bin/install
INSTALLDIR=/usr/local
BINDIR=$(INSTALLDIR)/bin

TCPKIT_CC = $(CC) $(CFLAGS) $(CINCLUDES)

PROG = tcpkit

OBJS = tcpkit.o packet.o sniffer.o server.o lua.o \
	   hashtable.o protocol.o dumper.o log.o cJSON.o stats.o

all: $(PROG)
.PHONY: all

include dep.mk

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(CLIBS) $(LOAD_LIBS)

$(OBJS): | $(PCAP_DEPS) $(LUA_LIB)

%.o: %.c
	$(TCPKIT_CC) -c $<

dep.mk:
	$(TCPKIT_CC) -MM *.c > dep.mk

$(LUA_LIB):
	cd $(LUA_DIR) && $(MAKE) a

# Strip the forced darwin `-arch x86_64` so the archive matches the host.
$(PCAP_LIB):
	cd $(PCAP_DIR) && ./configure --enable-dbus=no --without-libnl \
		&& sed 's/-arch x86_64//g' Makefile > Makefile.tmp \
		&& mv Makefile.tmp Makefile \
		&& $(MAKE)

lint:
	sh cppcheck.sh
test:

clean:
	- rm -rf $(PROG) dep.mk *.o

clean-deps:
	- cd $(PCAP_DIR) && test -f Makefile && $(MAKE) clean
	- cd $(LUA_DIR) && $(MAKE) clean

install:
	mkdir -p $(BINDIR)
	$(INSTALL) $(PROG) $(BINDIR)

.PHONY: lint test clean clean-deps install
