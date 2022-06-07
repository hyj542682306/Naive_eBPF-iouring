set -x

set -e

sudo mount -t bpf bpf /sys/fs/bpf/

clang -O2 -g -target bpf -c bpf_sockops.c -o bpf_sockops.o

sudo bpftool prog load bpf_sockops.o /sys/fs/bpf/bpf_sockops
sudo bpftool cgroup attach /sys/fs/cgroup sock_ops pinned /sys/fs/bpf/bpf_sockops

MAP_ID=$(sudo bpftool prog show pinned /sys/fs/bpf/bpf_sockops | grep -o -E 'map_ids [0-9]+' | cut -d ' ' -f2-)
sudo bpftool map pin id $MAP_ID /sys/fs/bpf/sock_ops_map

clang -O2 -g -Wall -target bpf -c bpf_redir.c -o bpf_redir.o

sudo bpftool prog load bpf_redir.o /sys/fs/bpf/bpf_redir map name sock_ops_map pinned /sys/fs/bpf/sock_ops_map
sudo bpftool prog attach pinned /sys/fs/bpf/bpf_redir msg_verdict pinned /sys/fs/bpf/sock_ops_map
