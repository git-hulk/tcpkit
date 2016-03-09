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

### capture packet

```
./tcpkit -S ../scripts/example.lua -p 5555 -i en5 -s 192.168.1.2
```

### Redis cost time 

```
./tcpkit -S ../scripts/redis.lua -p 6379 -i en5 -s 192.168.1.2
```
