PROG = tcpkit

test: unit e2e
.PHONY: test

CC = gcc
SRC_DIR = ../src
LUA_DIR = ../deps/lua/src

include ../mk/pcap.mk

# The unit suites run under the sanitizers by default: most of what they guard
# against are out-of-bounds reads that an ordinary build tolerates silently.
SANITIZE ?= address,undefined
CFLAGS = -g -O1 -Wall -I$(SRC_DIR) -Iunit $(PCAP_INCLUDES) -I$(LUA_DIR)
ifneq ($(SANITIZE),)
CFLAGS += -fsanitize=$(SANITIZE) -fno-omit-frame-pointer
endif

SUITES = test_hashtable test_protocol test_stats test_packet

unit: $(SUITES)
	@for t in $(SUITES); do echo "== $$t"; ./$$t || exit 1; done

e2e: $(SRC_DIR)/$(PROG)
	@sh e2e/run.sh

$(SRC_DIR)/$(PROG):
	$(MAKE) -C $(SRC_DIR)

test_hashtable: unit/test_hashtable.c $(SRC_DIR)/hashtable.c
	$(CC) $(CFLAGS) -o $@ $^

test_protocol: unit/test_protocol.c $(SRC_DIR)/protocol.c
	$(CC) $(CFLAGS) -o $@ $^

test_stats: unit/test_stats.c $(SRC_DIR)/stats.c $(SRC_DIR)/cJSON.c
	$(CC) $(CFLAGS) -o $@ $^

PACKET_SRCS = $(SRC_DIR)/packet.c $(SRC_DIR)/protocol.c $(SRC_DIR)/hashtable.c \
	      $(SRC_DIR)/stats.c $(SRC_DIR)/cJSON.c $(SRC_DIR)/log.c $(SRC_DIR)/lua.c

test_packet: unit/test_packet.c $(PACKET_SRCS) $(LUA_DIR)/liblua.a
	$(CC) $(CFLAGS) -o $@ $^ -lm -ldl -lpthread

# Opt-in: rebuilds the binary under ThreadSanitizer, so it is kept out of
# `make test` and clobbers the ordinary build.
test-race:
	$(MAKE) -C $(SRC_DIR) clean
	$(MAKE) -C $(SRC_DIR) SANITIZE=thread
	python3 e2e/race.py

fixtures:
	cd fixtures && python3 gen_fixtures.py

clean:
	- rm -rf $(SUITES) *.dSYM

.PHONY: unit e2e test-race fixtures clean
