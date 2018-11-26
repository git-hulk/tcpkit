array.o: array.c array.h
hashtable.o: hashtable.c hashtable.h
logger.o: logger.c logger.h
packet.o: packet.c ../deps/libpcap/pcap/sll.h \
  ../deps/libpcap/pcap/pcap-inttypes.h packet.h \
  ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/funcattrs.h \
  ../deps/libpcap/pcap/compiler-tests.h ../deps/libpcap/pcap/bpf.h \
  ../deps/libpcap/pcap/dlt.h tcpikt.h ../deps/lua/src/lua.h \
  ../deps/lua/src/luaconf.h stats.h hashtable.h array.h redis.h logger.h \
  vm.h ../deps/lua/src/lualib.h ../deps/lua/src/lauxlib.h
redis.o: redis.c redis.h
server.o: server.c ../deps/lua/src/lua.h ../deps/lua/src/luaconf.h \
  ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/funcattrs.h \
  ../deps/libpcap/pcap/compiler-tests.h \
  ../deps/libpcap/pcap/pcap-inttypes.h ../deps/libpcap/pcap/bpf.h \
  ../deps/libpcap/pcap/dlt.h server.h tcpikt.h stats.h hashtable.h \
  sniffer.h ../deps/libpcap/pcap.h array.h util.h logger.h
sniffer.o: sniffer.c ../deps/libpcap/pcap/pcap.h \
  ../deps/libpcap/pcap/funcattrs.h ../deps/libpcap/pcap/compiler-tests.h \
  ../deps/libpcap/pcap/pcap-inttypes.h ../deps/libpcap/pcap/bpf.h \
  ../deps/libpcap/pcap/dlt.h logger.h sniffer.h ../deps/libpcap/pcap.h
stats.o: stats.c stats.h
tcpkit.o: tcpkit.c ../deps/lua/src/lua.h ../deps/lua/src/luaconf.h vm.h \
  ../deps/lua/src/lualib.h ../deps/lua/src/lauxlib.h util.h array.h \
  packet.h ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/funcattrs.h \
  ../deps/libpcap/pcap/compiler-tests.h \
  ../deps/libpcap/pcap/pcap-inttypes.h ../deps/libpcap/pcap/bpf.h \
  ../deps/libpcap/pcap/dlt.h tcpikt.h stats.h hashtable.h sniffer.h \
  ../deps/libpcap/pcap.h server.h logger.h
util.o: util.c util.h array.h
vm.o: vm.c vm.h ../deps/lua/src/lua.h ../deps/lua/src/luaconf.h \
  ../deps/lua/src/lualib.h ../deps/lua/src/lauxlib.h tcpikt.h \
  ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/funcattrs.h \
  ../deps/libpcap/pcap/compiler-tests.h \
  ../deps/libpcap/pcap/pcap-inttypes.h ../deps/libpcap/pcap/bpf.h \
  ../deps/libpcap/pcap/dlt.h stats.h hashtable.h
