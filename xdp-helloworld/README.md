# Steps

## Create a new network device

```
$ sudo docker network create my-xdp-net
```

## Run a container using the network created

```
$ sudo docker run -d --rm -p 10086:80 --name my-nginx --network my-xdp-net nginx:alpine
```

## Test connnectivity

```
$ curl http://localhost:10086
```

## Load XDP program

```
$ sudo ip link set dev br-5cee99d90a59 xdp obj drop.o sec xdp verbose
```

## Test connnectivity again

```
$ curl http://localhost:10086
```

## Unload XDP program

```
$ sudo ip link set dev br-5cee99d90a59 xdp off
```

## Test connnectivity again

```
$ curl http://localhost:10086
```
