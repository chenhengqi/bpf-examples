#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

int ifindex = 0;
__u64 traffic = 0;

SEC("tp_btf/netif_receive_skb")
int BPF_PROG(netif_receive_skb, struct sk_buff *skb)
{
    if (skb->dev->ifindex == ifindex) {
        traffic += skb->data_len;
    }
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
