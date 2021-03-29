#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#include "bpf_helpers.h"
#include "bpf_endian.h"

static __always_inline int is_secure_source(void *data_begin, void *data_end)
{
	struct ethhdr *eth_header = data_begin;

	// ignore
	if ((void *)(eth_header + 1) > data_end) {
		return 1;
	}

	// ignore
	if (eth_header->h_proto != bpf_htons(ETH_P_IP)) {
		return 1;
	}

	struct iphdr *ip_header = (struct iphdr *)(eth_header + 1);

	// ignore
	if ((void *)(ip_header + 1) > data_end) {
		return 1;
	}

	// ignore
	if (ip_header->protocol != IPPROTO_TCP) {
		return 1;
	}

	struct tcphdr *tcp_header = (struct tcphdr *)(ip_header + 1);

	// ignore
	if ((void *)(tcp_header + 1) > data_end) {
		return 1;
	}

	if (tcp_header->dest == bpf_htons(12160)) {
		if (tcp_header->source != bpf_htons(10216)) {
			return 0;	// reject
		} else {
			return 1;	// accept
		}
	} else {
		return 1;
	}
}

SEC("xdp")
int xdp_secure_policy(struct xdp_md *ctx)
{
	void *data = (void *)(__u64)ctx->data;
	void *data_end = (void *)(__u64)ctx->data_end;
	if (is_secure_source(data, data_end)) {
		return XDP_PASS;
	} else {
		return XDP_DROP;
	}
}

char __license[] SEC("license") = "GPL";
