CC = gcc
CFLAGS = -O3 -Wall -Wextra

TARGETS = gen_sine fx_gain gen_lfo map_lfo

all: $(TARGETS)

gen_sine: gen_sine.c
	$(CC) $(CFLAGS) gen_sine.c -o gen_sine -lm

fx_gain: fx_gain.c
	$(CC) $(CFLAGS) fx_gain.c -o fx_gain

gen_lfo: gen_lfo.c
	$(CC) $(CFLAGS) gen_lfo.c -o gen_lfo -lm

map_lfo: map_lfo.c
	$(CC) $(CFLAGS) map_lfo.c -o map_lfo

clean:
	rm -f $(TARGETS)

.PHONY: all clean
