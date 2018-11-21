# tcpkit

A tool to capture tcp packets and analyze the packets with LUA. 

## Usage

```
TCPKIT is a tool to capture tcp packets and analyze the packets with lua.
	-s which server ip to monitor, e.g. 192.168.1.2,192.168.1.3
	-p which n_latency to monitor, e.g. 6379,6380
	-P stats listen port, default is 33333
	-i network card interface, e.g. bond0, lo, em0... see 'ifconfig'
	-d daemonize, run process in background
	-r set offline file captured by tcpdump or tcpkit
	-t request latency threshold, unit is millisecond
	-m protocol mode, raw,redis,memcached,dns
	-w dump packets to 'savefile'
	-S lua script path, default is ../scripts/example.lua
	-B operating system capture buffer size, in units of KiB (1024 bytes)
	-o log output file
	-u udp
	-v version
	-h help
```

## Install

```
$ git clone https://github.com/git-hulk/tcpkit.git tcpkit
$ cd tcpkit
$ sudo make && make install
```

## Monitor latency

Supports Redis/Memcached/DNS protocol now, we take Redis as example here: 

```
$ sudo tcpkit -i em1 -p 6379,6380,6381 -t 10 -m redis
```

and the request latency more than 10ms would be printed, like below:

```
2018-11-16 18:38:36.873067 172.20.64.106:53499 => 192.168.1.2:6379 | 11 ms | set foo bar
2018-11-16 18:38:55.051167 172.20.64.106:53499 => 192.168.1.2:6379 | 14 ms | get foo
```

Tcpkit would print all requests if the `-t` option wasn't set.
If tcpkit was deployed client-side, use the `-s` option to specify the server host/ip.

## Use lua to analyze packets

If the protocol wasn't supported by tcpkit or to inspect the tcp stream data, we can use raw mode. 
The tcp packet would be passed to `lua VM`, and we can analyze the packet with lua script. e.g.

```
$ sudo tcpkit -i em1 -p 6379 -m raw -S ../scripts/example.lua 
```

## Fetch Stats Online

Tcpkit also exports latency stats to the user by tcp port (default is 33333), use the `-P` option to change it.
The output is a json string, like below: 

```
[{"6379":{"total_reqs": 7,"total_costs":76785, "slow_reqs":7,"latencies":[0,0,0,0,0,3,3,1,0,0,0,0,0,0,0,0,0,0]}},
{"6380":{"total_reqs": 0,"total_costs":0, "slow_reqs":0,"latencies":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]}}]
```

This means that the port `6379` received `7` total requests, and that total costs is `76785` us, with `7` slow requests (slow threshold is `5ms`):
`5ms-10ms`: 3 requests
`10ms-20ms`: 3 requests
`20ms-50ms`: 1 request

The `latencies` arrays above correspond to the following latency buckets:
`0.1ms`, `0.2ms`, `0.5ms`, `1ms`, `5ms`, `10ms`, `20ms`, `50ms`, `100ms`, `200ms`, `500ms`, `1s`, `2s`, `3s`, `5s`, `10s`, `20s`, `>20s` 
