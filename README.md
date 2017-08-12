# tcpkit
A tool analyze tcp packets with Lua.

## How to use

```
./tcpkit is a tool to capature the tcp packets, and analyze the packets with lua
	-s server ip
	-p port
	-i device
	-r offline file
	-w write the raw packets to file
	-S lua script path, default is ../scripts/example.lua
	-l local address
	-d interval to print stats, unit is second 
	-B operating system capture buffer size, in units of KiB (1024 bytes). 
	-f log file
	-t only tcp
	-u only udp
	-v version
	-h help
```

### install 

```shell
$ cd tcpkit/src
$ make
```

### calculate port bandwidth

```shell
$ ./tcpkit -p 11211 -C -d 10
```

calculate server bandwidth every 10 seconds.

### Common Error

```
[2016-05-13 16:15:49] [ERROR] You should use -l to assign local ip.
```

Just use -l option to set your local ip, for example `sudo ./tcpkit -p 80 -l 192.168.4.1`.


```
[2016-05-13 16:18:46] [ERROR] may be you should assign device use -i and swith to root.
```

Just use -i option to set network device, for example `sudo ./tcpkit -p 80 -l 192.168.4.1 -i bond0`. 

See how to aquire device? use `ifconfig`.

### how to acquire device

use `ifconfig`

### capture packet

```
sudo ./tcpkit -S ../scripts/example.lua -p 5555 -s 192.168.1.2
```

### Monitor redis or memcached latency 

```
sudo ./tcpkit -S ../scripts/redis_mc_monitor.lua -p 6379 -s 192.168.1.2
```
![image](https://raw.githubusercontent.com/git-hulk/tcpkit/master/snapshot/redis_mc_monitor.png)

### monitor dns lanteny
```
sudo ./tcpkit -S ../scripts/dns_monitor.lua -p 53
```

### Monitor Kafka 

```
sudo ./tcpkit -S ../scripts/kafka_monitor.lua -p 6379 -s 192.168.1.2
```
