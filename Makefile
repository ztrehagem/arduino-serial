all: c

c: arduino_recv_sample

arduino_recv_sample: arduino_recv_sample.c
	gcc $< -o $@
