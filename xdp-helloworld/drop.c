#include <linux/bpf.h>
#include "bpf_helpers.h"

#define SEC(NAME) __attribute__((section(NAME), used))

SEC("xdp")
int xdp_drop(struct xdp_md *ctx) {
   bpf_printk("xdp_drop get called\n");
   return XDP_DROP;
}

char __license[] SEC("license") = "GPL";
