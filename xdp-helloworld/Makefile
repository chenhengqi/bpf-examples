BPF_HELPERS_URL=https://raw.githubusercontent.com/torvalds/linux/v5.4/tools/testing/selftests/bpf/bpf_helpers.h
BPF_HELPERS=bpf_helpers.h

build: download
	clang -Wall -target bpf -O2 -c drop.c -o drop.o

load:
	sudo ip link set dev br-5cee99d90a59 xdp obj drop.o sec xdp verbose

unload:
	sudo ip link set dev br-5cee99d90a59 xdp off

download:
	if [ ! -f $(BPF_HELPERS) ]; then curl $(BPF_HELPERS_URL) --output $(BPF_HELPERS); fi
