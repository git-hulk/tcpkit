cJSON.o: cJSON.c cJSON.h
local_addresses.o: local_addresses.c ../deps/libpcap/pcap.h \
  ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/bpf.h util.h
packet.o: packet.c packet.h tcpkit.h pcap_wrapper.h \
  ../deps/libpcap/pcap.h ../deps/libpcap/pcap/pcap.h \
  ../deps/libpcap/pcap/bpf.h script.h ../deps/lua/src/lua.h \
  ../deps/lua/src/luaconf.h ../deps/lua/src/lauxlib.h \
  ../deps/lua/src/lualib.h util.h ../deps/libpcap/pcap/sll.h \
  local_addresses.h
pcap_wrapper.o: pcap_wrapper.c ../deps/libpcap/pcap.h \
  ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/bpf.h pcap_wrapper.h \
  util.h tcpkit.h script.h ../deps/lua/src/lua.h \
  ../deps/lua/src/luaconf.h ../deps/lua/src/lauxlib.h \
  ../deps/lua/src/lualib.h
script.o: script.c tcpkit.h pcap_wrapper.h ../deps/libpcap/pcap.h \
  ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/bpf.h script.h \
  ../deps/lua/src/lua.h ../deps/lua/src/luaconf.h \
  ../deps/lua/src/lauxlib.h ../deps/lua/src/lualib.h util.h \
  local_addresses.h
tcpkit.o: tcpkit.c tcpkit.h pcap_wrapper.h ../deps/libpcap/pcap.h \
  ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/bpf.h script.h \
  ../deps/lua/src/lua.h ../deps/lua/src/luaconf.h \
  ../deps/lua/src/lauxlib.h ../deps/lua/src/lualib.h util.h packet.h \
  ../deps/libpcap/pcap/sll.h local_addresses.h
util.o: util.c util.h
