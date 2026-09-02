# tcpkit [![CI](https://github.com/git-hulk/tcpkit/actions/workflows/ci.yml/badge.svg)](https://github.com/git-hulk/tcpkit/actions/workflows/ci.yml)

The tcpkit was designed to analyze network packets with lua script, can also be used to observe the request latency
of the service with simple protocol, like `redis`/`memcached`.

## Install

```
$ git clone https://github.com/git-hulk/tcpkit.git tcpkit
$ cd tcpkit
$ sudo make && make install
```

The build links against the system libpcap when one is installed, and falls
back to the vendored copy in `deps/`. Pass `USE_SYSTEM_PCAP=0` to always build
the vendored one.

## Usage

```shell
the tcpkit was designed to make network packets programable with LUA by @git-hulk
   -h, Print the tcpkit version strings, print a usage message, and exit
   -i interface, Listen on network card interface
   -r file, Read packets from file (which was created with the -w option or by other tools that write pcap)
   -A Print each packet (minus its link level header) in ASCII.  Handy for capturing web pages
   -B buffer_size, Set the operating system capture buffer size to buffer_size, in units of KiB (1024 bytes)
   -s snaplen, Snarf snaplen bytes of data from each packet rather than the default of 1500 bytes
   -S file, Push packets to lua state if the script was specified
   -t threshold, Print the request lantecy which slower than the threshold, in units of Millisecond
   -w file, Write the raw packets to file
   -p protocol, Parse the packet if the protocol was specified (supports: redis, memcached, http, raw)
   -P stats port, Listen port to fetch the latency stats, default is 33333


For example:

   `tcpkit -i eth0 tcp port 6379 -p redis` was used to monitor the redis reqeust latency

   `tcpkit -i eth0 tcp port 6379 -p redis -w 6379.pcap` would also dump the packets to `6379.pcap`

   `tcpkit -i eth0 tcp port 6379 -p redis -t 10` would only print the request latency slower than 10ms
```

## How To Observe The Latency Of Redis/Memcached 


```shell
$ tcpkit -i eth0 tcp port 6379 -p redis
```  

tcpkit would listen on NIC `eth0` and caputure the tcp port `6379`, then parse network packets with Redis protocol. 
The output was like below:

```
2020-03-08 19:23:06.253384 127.0.0.1:51137 => 127.0.0.1:6379 | 0.615 ms | COMMAND
2020-03-08 19:23:06.258761 127.0.0.1:51137 => 127.0.0.1:6379 | 0.059 ms | get a
```

Use the option `-t` would only show the request which the request latency was slower than threshold(in units of millisecond).

tcpkit works out which side of a connection is the server from the TCP
handshake when it captures one. For connections that were already established
when tcpkit started -- the usual case with pooled clients -- it assumes the
lower of the two ports is the service, since clients draw from the ephemeral
range above it. A captured handshake always takes precedence.


## How to Use Lua Script

```
$ tcpkit -i eth0 tcp port 6379 -p redis -S scripts/example.lua
```

the callback function `function process(packet)` in `scripts/example.lua` would be triggered if new packets reached.

### Predefine Scripts

1. [exmaple.lua](https://github.com/git-hulk/tcpkit/blob/master/scripts/example.lua) - example for user defined script
2. [dns.lua](https://github.com/git-hulk/tcpkit/blob/master/scripts/dns.lua) - print the dns latency
3. [tcp-connnect.lua](https://github.com/git-hulk/tcpkit/blob/master/scripts/tcp-connect.lua) - print connection with syn packet retransmit

## Request Latency

the stats port listen on `33333` by default, use the `-P` to change the listen port.

```json
➜  ~ telnet 127.0.0.1 33333
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.

{
	"127.0.0.1:6379":	{
		"requests":	1700,
		"request_bytes":	184100,
		"responses":	1700,
		"response_bytes":	1413764,
		"latency":	[{
				"<0.1ms":	326
			}, {
				"0.1ms~0.2ms":	371
			}, {
				"0.2~0.5ms":	589
			}, {
				"0.5ms~1ms":	291
			}, {
				"1ms~5ms":	123
			}]
	}
}
```

## Development

Run the test suite:

```shell
$ make test
```

`make test` builds the binary, then runs the unit suites in `tests/unit` (under
AddressSanitizer and UndefinedBehaviorSanitizer) followed by the end-to-end
checks in `tests/e2e`, which replay the pcap fixtures in `tests/fixtures`
through the binary and assert on its output.

The fixtures are assembled frame by frame by `tests/fixtures/gen_fixtures.py`,
so they need neither root nor a live capture. Regenerate them with:

```shell
$ make fixtures
```

## License

tcpkit is under the MIT license. See the LICENSE file for details.
