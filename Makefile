CC = gcc
CFLAGS = -O3 -Wall -Wextra

TARGETS = gen_sine gen_lfo gen_noise map_lfo fx_mul fx_add gen_env fx_mix fx_mix_pool gen_harm fx_reverb fx_corrode fx_lowpass fx_highpass dac

all: $(TARGETS)

gen_sine: gen_sine.c
	$(CC) $(CFLAGS) gen_sine.c -o gen_sine -lm

gen_lfo: gen_lfo.c
	$(CC) $(CFLAGS) gen_lfo.c -o gen_lfo -lm

map_lfo: map_lfo.c
	$(CC) $(CFLAGS) map_lfo.c -o map_lfo

fx_mix: fx_mix.c
	$(CC) $(CFLAGS) fx_mix.c -o fx_mix

fx_mix_pool: fx_mix_pool.c
	$(CC) $(CFLAGS) fx_mix_pool.c -o fx_mix_pool

fx_mul: fx_mul.c
	$(CC) $(CFLAGS) fx_mul.c -o fx_mul

fx_add: fx_add.c
	$(CC) $(CFLAGS) fx_add.c -o fx_add

fx_corrode: fx_corrode.c
	$(CC) $(CFLAGS) fx_corrode.c -o fx_corrode -lm

gen_env: gen_env.c
	$(CC) $(CFLAGS) gen_env.c -o gen_env -lm

gen_harm: gen_harm.c
	$(CC) $(CFLAGS) gen_harm.c -o gen_harm -lm

gen_noise: gen_noise.c
	$(CC) $(CFLAGS) gen_noise.c -o gen_noise -lm

fx_reverb: fx_reverb.c
	$(CC) $(CFLAGS) fx_reverb.c -o fx_reverb -lm

fx_lowpass: fx_lowpass.c
	$(CC) $(CFLAGS) fx_lowpass.c -o fx_lowpass -lm

fx_highpass: fx_highpass.c
	$(CC) $(CFLAGS) fx_highpass.c -o fx_highpass -lm

dac: dac.c
	$(CC) $(CFLAGS) dac.c -o dac

clean:
	rm -f $(TARGETS)

.PHONY: all clean
