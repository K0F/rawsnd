#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAMPLE_RATE 44100
#define MAX_DELAY_SAMPLES (SAMPLE_RATE * 2) // Maximální délka zpoždění 2 sekundy

int main(int argc, char *argv[]) {
    float delay_ms = 110.0f; // Výchozí zpoždění v ms
    float feedback = 0.85f;  // Intenzita dozvuku (0.0 až < 1.0)
    float wet_dry = 0.4f;    // Poměr efektu (0.0 = čistý zvuk, 1.0 = plný reverb)

    if (argc > 1) delay_ms = atof(argv[1]);
    if (argc > 2) feedback = atof(argv[2]);
    if (argc > 3) wet_dry = atof(argv[3]);

    int delay_samples = (int)(SAMPLE_RATE * (delay_ms / 1000.0f));
    if (delay_samples < 1) delay_samples = 1;
    if (delay_samples >= MAX_DELAY_SAMPLES) delay_samples = MAX_DELAY_SAMPLES - 1;

    float *buffer = (float *)calloc(MAX_DELAY_SAMPLES, sizeof(float));
    if (!buffer) {
        fprintf(stderr, "Chyba alokace paměti pro reverb buffer\n");
        return 1;
    }

    int write_idx = 0;
    float in_sample, delayed_sample, out_sample;
    float lowpass_state = 0.0f; // Jednoduché tlumení výšek ve zpětné vazbě

    while (fread(&in_sample, sizeof(float), 1, stdin) == 1) {
        int read_idx = (write_idx - delay_samples + MAX_DELAY_SAMPLES) % MAX_DELAY_SAMPLES;
        delayed_sample = buffer[read_idx];

        lowpass_state = lowpass_state * 0.4f + delayed_sample * 0.6f;

        buffer[write_idx] = in_sample + (lowpass_state * feedback);

        out_sample = in_sample * (1.0f - wet_dry) + delayed_sample * wet_dry;

        if (fwrite(&out_sample, sizeof(float), 1, stdout) != 1) break;

        write_idx = (write_idx + 1) % MAX_DELAY_SAMPLES;
    }

    free(buffer);
    return 0;
}
