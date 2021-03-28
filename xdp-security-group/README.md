# Network Traffic Stat

## Create a new network device

```
$ sudo docker network create my-tc-net
```

## Create a file for download

```
$ dd if=/dev/zero of=/tmp/testfile bs=4096 count=131072
```

## Run a nginx server that serves file download

```
$ sudo docker run -d --rm \
        -p 10086:80 \
        -v /tmp/testfile:/home/data/testfile \
        -v $(PWD)/default.conf:/etc/nginx/conf.d/default.conf \
        --name my-nginx \
        --network my-tc-net \
        nginx:alpine
```

## Download file and stat network traffic using libpcap

On a terminal:
```
$ sudo ./pcap-stat
```

On another terminal:
```
$ curl http://localhost:10086/downloads/testfile --output testfile
```

## Download file and stat network traffic using BPF

On a terminal:
```
$ sudo ./bpf-stat
```

On another terminal:
```
$ curl http://localhost:10086/downloads/testfile --output testfile
```
