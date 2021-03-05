# encoding: utf8

"""
Read Trace Logs
~~~~~~~~~~~~~~~

This script is used to view trace logs.
"""

import datetime
import re
import socket
import subprocess
import sys
import time

# TCP states
# https://elixir.bootlin.com/linux/latest/source/include/net/tcp_states.h
TCP_STATE = {
    1: 'TCP_ESTABLISHED',
    2: 'TCP_SYN_SENT',
    3: 'TCP_SYN_RECV',
    4: 'TCP_FIN_WAIT1',
    5: 'TCP_FIN_WAIT2',
    6: 'TCP_TIME_WAIT',
    7: 'TCP_CLOSE',
    8: 'TCP_CLOSE_WAIT',
    9: 'TCP_LAST_ACK',
    10: 'TCP_LISTEN',
    11: 'TCP_CLOSING',
    12: 'TCP_NEW_SYN_RECV',
}

log_path = '/sys/kernel/debug/tracing/trace'
if len(sys.argv) > 1:
    log_path = sys.argv[1]

status, output = subprocess.getstatusoutput('cat /proc/uptime|cut -f 1 -d" "')
if status != 0:
    sys.exit(-1)

base_time = time.time() - float(output)

with open(log_path, 'r') as file:
    for line in file:
        match = re.findall(r'(\d+\.\d+):.*:.*(tcp_\S+) port=(\d+) dst=(\d+) state=(\d+)', line)
        if not match or len(match[0]) != 5:
            continue

        matches = match[0]

        # Boot time
        boot_time = matches[0]

        # Tracepoint
        tracepoint = matches[1]

        # Datetime
        t = base_time + float(boot_time)
        dt = datetime.datetime.fromtimestamp(t).strftime('%Y-%m-%d %H:%M:%S.%f')

        # Port, in network byteorder
        raw_port = matches[2]
        port = int.from_bytes(int(raw_port).to_bytes(2, 'big'), 'little')

        # IP, in network byteorder?
        raw_ip = matches[3]
        ip = socket.inet_ntop(socket.AF_INET, int(raw_ip).to_bytes(4, 'little'))

        # TCP State
        state = int(matches[4])
        tcp_state = TCP_STATE[state] if 1 <= state <= 12 else 'INVALID'

        print('{} {}\t{}\t{:16}\t{:5}\t{:16}'.format(boot_time, dt, tracepoint, ip, port, tcp_state))
