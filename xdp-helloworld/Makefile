BPF_HELPERS_URL=https://raw.githubusercontent.com/torvalds/linux/v5.4/tools/testing/selftests/bpf/bpf_helpers.h
BPF_HELPERS=bpf_helpers.h
DEV_NAME=br-$(shell sudo docker network inspect my-xdp-net -f '{{.Id}}' | cut -c1-12)

build: download
	clang -Wall -target bpf -O2 -g -c drop.c -o drop.o

load:
	sudo ip link set dev $(DEV_NAME) xdp obj drop.o sec xdp verbose

unload:
	sudo ip link set dev $(DEV_NAME) xdp off

download:
	if [ ! -f $(BPF_HELPERS) ]; then curl $(BPF_HELPERS_URL) --output $(BPF_HELPERS); fi

debug:
	clang -Wall -target bpf -g -c drop.c -o debug-drop.o
	llvm-objdump-11 -S --no-show-raw-insn debug-drop.o
	clang -Wall -target bpf -O2 -g -c drop.c -o debug-drop-O2.o
	llvm-objdump-11 -S --no-show-raw-insn debug-drop-O2.o

clean:
	rm -f *.o
