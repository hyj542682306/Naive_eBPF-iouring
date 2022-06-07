#include <uapi/linux/ptrace.h>
#include <linux/sched.h>

BPF_HASH(normal_in_hash);
int bpf_normal_in(struct pt_regs *ctx) {
	u64 time = bpf_ktime_get_ns(), key = 0, *sz, nsz;
	sz = normal_in_hash.lookup(&key);
	if(sz == NULL) nsz = 1;
	else nsz = (*sz) + 1;
	normal_in_hash.update(&key, &nsz);
	normal_in_hash.update(&nsz, &time);
	return 0;
}

BPF_HASH(normal_out_hash);
int bpf_normal_out(struct pt_regs *ctx) {
	u64 time = bpf_ktime_get_ns(), key = 0, *sz, nsz;
	sz = normal_out_hash.lookup(&key);
	if(sz == NULL) nsz = 1;
	else nsz = (*sz) + 1;
	normal_out_hash.update(&key, &nsz);
	normal_out_hash.update(&nsz, &time);
	return 0;
}

BPF_HASH(io_uring_in_hash);
int bpf_io_uring_in(struct pt_regs *ctx) {
	u64 time = bpf_ktime_get_ns(), key = 0, *sz, nsz;
	sz = io_uring_in_hash.lookup(&key);
	if(sz == NULL) nsz = 1;
	else nsz = (*sz) + 1;
	io_uring_in_hash.update(&key, &nsz);
	io_uring_in_hash.update(&nsz, &time);
	return 0;
}

BPF_HASH(io_uring_out_hash);
int bpf_io_uring_out(struct pt_regs *ctx) {
	u64 time = bpf_ktime_get_ns(), key = 0, *sz, nsz;
	sz = io_uring_out_hash.lookup(&key);
	if(sz == NULL) nsz = 1;
	else nsz = (*sz) + 1;
	io_uring_out_hash.update(&key, &nsz);
	io_uring_out_hash.update(&nsz, &time);
	return 0;
}
