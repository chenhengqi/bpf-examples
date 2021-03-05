package main

import (
	"fmt"

	"github.com/iovisor/gobpf/bcc"
)

const bpfProgram = `
#include <uapi/linux/ptrace.h>
#include <net/sock.h>

int log_tcp_retransmit(struct pt_regs *ctx, struct sock *sk) {
    bpf_trace_printk("tcp_retransmit\n");
    return 0;
}

int log_tcp_reset(struct pt_regs *ctx, struct sock *sk) {
    bpf_trace_printk("tcp_reset port=%d dst=%d state=%d\n", sk->__sk_common.skc_dport, sk->__sk_common.skc_daddr, sk->__sk_common.skc_state);
    return 0;
}
`

const (
	funcNameTCPRetransmit  = "log_tcp_retransmit"
	funcNameTCPReset       = "log_tcp_reset"
	eventNameTCPRetransmit = "tcp_retransmit_skb"
	eventNameTCPReset      = "tcp_reset"
	maxActive              = 0
)

func main() {
	bpfModule := bcc.NewModule(bpfProgram, []string{})
	defer bpfModule.Close()

	uprobeFD, err := bpfModule.LoadKprobe(funcNameTCPRetransmit)
	if err != nil {
		panic(err)
	}

	err = bpfModule.AttachKprobe(eventNameTCPRetransmit, uprobeFD, maxActive)
	if err != nil {
		panic(err)
	}

	uprobeFD, err = bpfModule.LoadKprobe(funcNameTCPReset)
	if err != nil {
		panic(err)
	}

	err = bpfModule.AttachKprobe(eventNameTCPReset, uprobeFD, maxActive)
	if err != nil {
		panic(err)
	}

	fmt.Println("attached!")
	select {}
}
