# tcpkit
A tool analyze tcp packets with Lua.

## How to use

```
./tcpkit
    -s server ip.
    -p port.
    -i device.
    -S lua script path, default is ../scripts/example.lua.
    -l local address.
    -f log file.
    -v version.
    -h help.
```

### install 

```shell
$ cd tcpkit/src
$ make
```

### how to acquire device

use `ifconfig`

### capture packet

```
sudo ./tcpkit -S ../scripts/example.lua -p 5555 -i en5 -s 192.168.1.2
```

### Monitor redis or memcached cost time 

```
sudo ./tcpkit -S ../scripts/redis_mc_monitor.lua -p 6379 -i en5 -s 192.168.1.2
```
![image](https://raw.githubusercontent.com/git-hulk/tcpkit/master/snapshot/redis_mc_monitor.png)
