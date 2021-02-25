package main

import (
	"os"

	"github.com/iovisor/gobpf/bcc"
)

const bpfProgram = `
#include <uapi/linux/ptrace.h>

const int MAX_LEN = 0xFF;

int hack(struct pt_regs *ctx) {
    u64* sp = (u64*)ctx->sp;

    u64 a = 0;
    bpf_probe_read(&a, sizeof(a), sp + 0);

    char b[MAX_LEN] = {0};
    u64 addr = 0;
    bpf_probe_read(&addr, sizeof(addr), sp + 1);
    u64 size = 0;
    bpf_probe_read(&size, sizeof(size), sp + 2);
    bpf_probe_read_str(&b, ((size & MAX_LEN) + 1) & MAX_LEN, (u64*)addr);

    u64 c = 0;
    bpf_probe_read(&c, sizeof(c), sp + 3);

    bpf_trace_printk("I got it: %x %s %x\n", a, b, c);
    return 0;
}
`

const (
	bpfFuncName = "hack"
)

func main() {
	bpfModule := bcc.NewModule(bpfProgram, []string{})
	uprobeFD, err := bpfModule.LoadUprobe(bpfFuncName)
	if err != nil {
		panic(err)
	}

	hackedBinary := os.Args[1]
	hackedFuncName := os.Args[2]
	err = bpfModule.AttachUretprobe(hackedBinary, hackedFuncName, uprobeFD, -1)
	if err != nil {
		panic(err)
	}

	select {}
}
