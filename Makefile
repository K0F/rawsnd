CC = gcc
CFLAGS = -O3 -Wall -Wextra

TARGETS = gen_sine gen_lfo map_lfo fx_mul fx_add gen_env

all: $(TARGETS)

gen_sine: gen_sine.c
	$(CC) $(CFLAGS) gen_sine.c -o gen_sine -lm

gen_lfo: gen_lfo.c
	$(CC) $(CFLAGS) gen_lfo.c -o gen_lfo -lm

map_lfo: map_lfo.c
	$(CC) $(CFLAGS) map_lfo.c -o map_lfo

fx_mul: fx_mul.c
	$(CC) $(CFLAGS) fx_mul.c -o fx_mul

fx_add: fx_add.c
	$(CC) $(CFLAGS) fx_add.c -o fx_add

gen_env: gen_env.c
	$(CC) $(CFLAGS) gen_env.c -o gen_env


clean:
	rm -f $(TARGETS)

.PHONY: all clean
