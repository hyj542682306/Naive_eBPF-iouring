set -x

sudo bpftool prog detach pinned /sys/fs/bpf/bpf_redir msg_verdict pinned /sys/fs/bpf/sock_ops_map
sudo rm /sys/fs/bpf/bpf_redir

sudo bpftool cgroup detach /sys/fs/cgroup/ sock_ops pinned /sys/fs/bpf/bpf_sockops
sudo rm /sys/fs/bpf/bpf_sockops

sudo rm /sys/fs/bpf/sock_ops_map
