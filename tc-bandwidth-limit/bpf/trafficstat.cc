#include "trafficstat.skel.h"

#include <chrono>
#include <cstdio>
#include <thread>


const int ifindex = 7;

int main() {
    auto obj = trafficstat_bpf__open();
    if (!obj) {
        fprintf(stderr, "failed to open BPF object\n");
        return -1;
    }

    obj->bss->ifindex = ifindex;

    auto err = trafficstat_bpf__load(obj);
    if (err) {
        fprintf(stderr, "failed to load BPF object: %d\n", err);
        trafficstat_bpf__destroy(obj);
        return -1;
    }

    err = trafficstat_bpf__attach(obj);
    if (err) {
        fprintf(stderr, "failed to attach BPF object: %d\n", err);
        trafficstat_bpf__destroy(obj);
        return -1;
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        printf("traffic in bytes: %lld\n", obj->bss->traffic);
    }
    return 0;
}
