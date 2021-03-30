ROOT_DIR=$(realpath ..)
LIBBPF_HEADERS_DIR=$(ROOT_DIR)/tc-traffic-stat/bpf/libbpf/src

DEV_NAME=eth0
OBJ=sg.bpf.o

build:
	clang -I$(LIBBPF_HEADERS_DIR) -Wall -g -O2 -target bpf -c sg.bpf.c -o $(OBJ)

load:
	sudo ip link set dev $(DEV_NAME) xdpgeneric obj $(OBJ) sec xdp verbose

unload:
	sudo ip link set dev $(DEV_NAME) xdpgeneric off

dump:
	llvm-objdump-10 -S --no-show-raw-insn $(OBJ)

clean:
	rm -f $(OBJ)
