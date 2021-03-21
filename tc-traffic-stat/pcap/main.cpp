#include <pcap.h>
#include <linux/ip.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <future>
#include <thread>

#define ETHERNET_HEADER_LEN 14
#define MIN_IP_HEADER_LEN 20
#define IP_HL(ip) (((ip)->ihl) & 0x0f)

const char* dev = "br-7e20abc6df31";
const char* filter_expr = "src net 172.20.0.0/16";

int64_t traffic = 0;

// Reference:
//      https://www.tcpdump.org/manpages/
//      https://tools.ietf.org/html/rfc791
void traffic_stat() {
    // find the IPv4 network number and netmask for a device
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    int ret = pcap_lookupnet(dev, &net, &mask, errbuf);
    if (ret == PCAP_ERROR) {
        fprintf(stderr, "pcap_lookupnet failed: %s\n", errbuf);
        exit(EXIT_FAILURE);
    }

    // open a device for capturing
    int promisc = 1;
    int timeout = 1000;   // in milliseconds
    const int SNAP_LEN = 64;
    auto handle = pcap_open_live(dev, SNAP_LEN, promisc, timeout, errbuf);
    if (!handle) {
        fprintf(stderr, "pcap_open_live failed: %s\n", errbuf);
        exit(EXIT_FAILURE);
    }

    // compile a filter expression
    struct bpf_program fp;
    int optimize = 1;
    ret = pcap_compile(handle, &fp, filter_expr, optimize, net);
    if (ret == PCAP_ERROR) {
        fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    // set the filter
    ret = pcap_setfilter(handle, &fp);
    if (ret == PCAP_ERROR) {
        fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    // process packets from a live capture
    int packet_count = -1;  // -1 means infinity
    pcap_loop(handle, packet_count, [](u_char* args, const struct pcap_pkthdr* header, const u_char* bytes) {

        auto ip_header = reinterpret_cast<iphdr*>(const_cast<u_char*>(bytes) + ETHERNET_HEADER_LEN);
        const int ip_header_len = IP_HL(ip_header) * 4;
        if (ip_header_len < MIN_IP_HEADER_LEN) {
            return;
        }

        auto len = ntohs(ip_header->tot_len);
        if (len <= 0) {
            return;
        }

        auto traffic = reinterpret_cast<int64_t*>(args);
        *traffic += len;

    }, reinterpret_cast<u_char*>(&traffic));

    // free a BPF program
    pcap_freecode(&fp);
    // close the capture device
    pcap_close(handle);
}

int main() {
    auto task = std::async(std::launch::async, traffic_stat);
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        printf("traffic in bytes: %ld\n", traffic);
    }
    task.wait();
    return 0;
}
