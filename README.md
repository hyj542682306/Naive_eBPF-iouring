# Naive_eBPF-iouring

## Task 1

将两段eBPF程序attach到socket这一hook，在建立socket和sendmsg时进行截断，不通过之后TCP协议等步骤，而是直接将数据重定向到接受端，进行socket通信的加速.

该加速仅在同一机器的两个端口中生效.

使用本地socket通信的程序进行测试：从client到server发送 $1e6$ 次长度为 $50$ 的buf，重复十次，记录正常情况下和加速情况下的用时；将次数取 $1e3, 1e4, 1e5, 1e6, 1e7$ 分别进行实验查看结果.

## Task 2

使用bcc对normal的IO操作和io_uring的IO操作进行跟踪计时，比较两者的差异.

在IO字符总量为 $2^{16}$ 的情况下，调整batchsize大小进行重复实验.
