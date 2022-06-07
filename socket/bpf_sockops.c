#include <linux/bpf.h>
#include "bpf_sockops.h"

static inline void record(struct bpf_sock_ops *skops)
{
	struct sock_key key = {};
	key.family = 1;
	key.dip4 = skops->remote_ip4;
	key.sip4 = skops->local_ip4;
	key.sport = (bpf_htonl(skops->local_port) >> 16);
	key.dport = FORCE_READ(skops->remote_port) >> 16;
	sock_hash_update(skops, &sock_ops_map, &key, BPF_NOEXIST);
	//printk("sockmap: op %d, port %d --> %d\n", skops->op, skops->local_port, bpf_ntohl(skops->remote_port));
}

__section("sockops")
int bpf_sockmap(struct bpf_sock_ops *skops)
{
	if(skops->op == BPF_SOCK_OPS_PASSIVE_ESTABLISHED_CB || skops->op == BPF_SOCK_OPS_ACTIVE_ESTABLISHED_CB)
		record(skops);
	return 0;
}

char ____license[] __section("license") = "GPL";
int _version __section("version") = 1;
