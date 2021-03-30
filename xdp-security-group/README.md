# Implement your own security group

## Run a simple TCP echo server

```
$ cd server
$ make
$ make run
```

## Interact with the server using netcat

run `netcat` from another host
```
$ nc $(SERVER_IP) 12160
hello world
hello world
```

## Apply our own security policy

build BPF program and attach to `eth0`
```
$ sudo ip link set dev eth0 xdpgeneric obj sg.bpf.o sec xdp verbose
```

## Now client should fail to send echo request to the server

```
$ nc $(SERVER_IP) 12160
hello world
hello world
...
ping
```

## Interact with the server using netcat from port 10216

run `netcat` again, this time we specify client port
```
$ nc $(SERVER_IP) 12160 -p 10216
hello world
hello world
```

it works
