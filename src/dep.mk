cJSON.o: cJSON.c cJSON.h
dumper.o: dumper.c dumper.h ../deps/libpcap/pcap.h \
  ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/funcattrs.h \
  ../deps/libpcap/pcap/compiler-tests.h \
  ../deps/libpcap/pcap/pcap-inttypes.h ../deps/libpcap/pcap/bpf.h \
  ../deps/libpcap/pcap/dlt.h tcpkit.h sniffer.h ../deps/lua/src/lua.h \
  ../deps/lua/src/luaconf.h stats.h cJSON.h hashtable.h server.h
hashtable.o: hashtable.c hashtable.h
log.o: log.c log.h
lua.o: lua.c lua.h ../deps/lua/src/lua.h ../deps/lua/src/luaconf.h \
  ../deps/lua/src/lualib.h ../deps/lua/src/lauxlib.h tcpkit.h
packet.o: packet.c ../deps/libpcap/pcap.h ../deps/libpcap/pcap/pcap.h \
  ../deps/libpcap/pcap/funcattrs.h ../deps/libpcap/pcap/compiler-tests.h \
  ../deps/libpcap/pcap/pcap-inttypes.h ../deps/libpcap/pcap/bpf.h \
  ../deps/libpcap/pcap/dlt.h log.h lua.h ../deps/lua/src/lua.h \
  ../deps/lua/src/luaconf.h ../deps/lua/src/lualib.h \
  ../deps/lua/src/lauxlib.h packet.h sniffer.h tcpkit.h stats.h cJSON.h \
  hashtable.h protocol.h
protocol.o: protocol.c protocol.h
server.o: server.c tcpkit.h server.h sniffer.h ../deps/libpcap/pcap.h \
  ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/funcattrs.h \
  ../deps/libpcap/pcap/compiler-tests.h \
  ../deps/libpcap/pcap/pcap-inttypes.h ../deps/libpcap/pcap/bpf.h \
  ../deps/libpcap/pcap/dlt.h ../deps/lua/src/lua.h \
  ../deps/lua/src/luaconf.h stats.h cJSON.h hashtable.h dumper.h log.h
sniffer.o: sniffer.c ../deps/libpcap/pcap.h ../deps/libpcap/pcap/pcap.h \
  ../deps/libpcap/pcap/funcattrs.h ../deps/libpcap/pcap/compiler-tests.h \
  ../deps/libpcap/pcap/pcap-inttypes.h ../deps/libpcap/pcap/bpf.h \
  ../deps/libpcap/pcap/dlt.h ../deps/libpcap/pcap/sll.h lua.h \
  ../deps/lua/src/lua.h ../deps/lua/src/luaconf.h \
  ../deps/lua/src/lualib.h ../deps/lua/src/lauxlib.h packet.h sniffer.h \
  tcpkit.h stats.h cJSON.h hashtable.h
stats.o: stats.c cJSON.h stats.h
tcpkit.o: tcpkit.c ../deps/libpcap/pcap.h ../deps/libpcap/pcap/pcap.h \
  ../deps/libpcap/pcap/funcattrs.h ../deps/libpcap/pcap/compiler-tests.h \
  ../deps/libpcap/pcap/pcap-inttypes.h ../deps/libpcap/pcap/bpf.h \
  ../deps/libpcap/pcap/dlt.h log.h tcpkit.h server.h
