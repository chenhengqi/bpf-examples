ROOT_DIR=$(realpath ..)
LIBBPF_HEADERS_DIR=$(ROOT_DIR)/tc-traffic-stat/bpf/libbpf/src

DEV_NAME=br-$(shell sudo docker network inspect my-xdp-net -f '{{.Id}}' | cut -c1-12)
OBJ=sg.bpf.o

build:
	clang -I$(LIBBPF_HEADERS_DIR) -Wall -g -O2 -target bpf -c sg.bpf.c -o $(OBJ)

load:
	sudo ip link set dev $(DEV_NAME) xdp obj $(OBJ) sec xdp verbose

unload:
	sudo ip link set dev $(DEV_NAME) xdp off

run-server:
	sudo docker run -it --network my-xdp-net busybox sh -c 'nc -k -l -p 12160'

run-client:
	sudo docker run -it --network my-xdp-net busybox sh -c 'nc 172.18.0.2 12160'

dump:
	llvm-objdump-10 -S --no-show-raw-insn $(OBJ)

clean:
	rm -f $(OBJ)
