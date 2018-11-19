array.o: array.c array.h
hashtable.o: hashtable.c hashtable.h
logger.o: logger.c logger.h
packet.o: packet.c ../deps/libpcap/pcap/sll.h packet.h \
 ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/bpf.h tcpikt.h \
 ../deps/lua/src/lua.h ../deps/lua/src/luaconf.h stats.h hashtable.h \
 array.h redis.h logger.h vm.h ../deps/lua/src/lualib.h \
 ../deps/lua/src/lua.h ../deps/lua/src/lauxlib.h
redis.o: redis.c redis.h
server.o: server.c ../deps/lua/src/lua.h ../deps/lua/src/luaconf.h \
 ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/bpf.h server.h tcpikt.h \
 stats.h hashtable.h sniffer.h ../deps/libpcap/pcap.h array.h logger.h
sniffer.o: sniffer.c ../deps/libpcap/pcap/pcap.h \
 ../deps/libpcap/pcap/bpf.h sniffer.h ../deps/libpcap/pcap.h
stats.o: stats.c stats.h
tcpkit.o: tcpkit.c ../deps/lua/src/lua.h ../deps/lua/src/luaconf.h vm.h \
 ../deps/lua/src/lualib.h ../deps/lua/src/lua.h ../deps/lua/src/lauxlib.h \
 util.h packet.h ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/bpf.h \
 array.h tcpikt.h stats.h hashtable.h sniffer.h ../deps/libpcap/pcap.h \
 server.h logger.h
util.o: util.c util.h array.h
vm.o: vm.c vm.h ../deps/lua/src/lua.h ../deps/lua/src/luaconf.h \
 ../deps/lua/src/lualib.h ../deps/lua/src/lua.h ../deps/lua/src/lauxlib.h \
 tcpikt.h ../deps/libpcap/pcap/pcap.h ../deps/libpcap/pcap/bpf.h stats.h \
 hashtable.h
