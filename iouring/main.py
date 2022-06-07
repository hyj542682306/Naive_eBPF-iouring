from __future__ import print_function
from bcc import BPF, USDT
from time import sleep

bpf_text = open('bpf_time.c', 'r').read()
pid = open('bpf_pid', 'r').readline()

u = USDT(pid = int(pid))

u.enable_probe(probe = "normal_in", fn_name = "bpf_normal_in")
u.enable_probe(probe = "normal_out", fn_name = "bpf_normal_out")

u.enable_probe(probe = "io_uring_in", fn_name = "bpf_io_uring_in")
u.enable_probe(probe = "io_uring_out", fn_name = "bpf_io_uring_out")

b = BPF(text = bpf_text, usdt_contexts = [u])

try:
	sleep(999999)
except KeyboardInterrupt:
	pass

tmp1 = b["normal_in_hash"]
normal_start = []
for _, it in tmp1.items():
	normal_start.append(it)

tmp2 = b["normal_out_hash"]
normal_finish = []
for _, it in tmp2.items():
	normal_finish.append(it)

tmp3 = b["io_uring_in_hash"]
io_uring_start = []
for _, it in tmp3.items():
	io_uring_start.append(it)

tmp4 = b["io_uring_out_hash"]
io_uring_finish = []
for _, it in tmp4.items():
	io_uring_finish.append(it)

s = 0
for i in range(0, len(normal_start)):
	s = s + normal_finish[i].value - normal_start[i].value
normal_res = s / (len(normal_start) + 1)
print("normal: ", normal_res)

s = 0
for i in range(0, len(io_uring_start)):
	s = s + io_uring_finish[i].value - io_uring_start[i].value
io_uring_res = s / (len(io_uring_start) + 1)
print("io_uring: ", io_uring_res)
