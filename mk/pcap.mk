# Shared libpcap discovery for src/ and tests/. Both live one level below the
# repository root, so the relative paths below resolve from either.

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

# Strip the forced darwin `-arch x86_64` so the archive matches the host.
$(PCAP_LIB):
	cd $(PCAP_DIR) && ./configure --enable-dbus=no --without-libnl \
		&& sed 's/-arch x86_64//g' Makefile > Makefile.tmp \
		&& mv Makefile.tmp Makefile \
		&& $(MAKE)
