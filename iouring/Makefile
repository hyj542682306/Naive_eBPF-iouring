INCLUDE_PATH = -I/usr/share/liburing/src/include
LIBRARY_PATH = -L/usr/lib/x86_64-linux-gnu
LIB = -luring -lrt -laio



build:	bpf_io.c
	clang -o bpf_io $(INCLUDE_PATH) $(LIBRARY_PATH) $(LIB) $?

clean:
	rm -f test bpf_pid testIOuring testNormal
run:
	sudo ./bpf_io & sleep 1 && sudo python3 main.py
.DEFAULT_GOAL := build
