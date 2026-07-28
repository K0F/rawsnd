#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define SAMPLE_RATE 44100.0f
#define NUM_HARMONICS 20

int main(int argc, char *argv[]) {
    float base_freq = 110.0f; // Výchozí tón A2
    if (argc > 1) {
        base_freq = atof(argv[1]);
    }

    int has_stdin = !isatty(fileno(stdin));

    float phases[NUM_HARMONICS] = {0};
    float current_freq = base_freq;

    float amp_sum = 0.0f;
    for (int i = 1; i <= NUM_HARMONICS; i++) {
        amp_sum += 1.0f / (float)i;
    }

    while (1) {
        if (has_stdin) {
            float freq_in;
            if (fread(&freq_in, sizeof(float), 1, stdin) == 1) {
                current_freq = freq_in;
            } else {
                break;
            }
        }

        float sample = 0.0f;
        
        for (int h = 1; h <= NUM_HARMONICS; h++) {
            float h_freq = current_freq * (float)h;
            
            if (h_freq < SAMPLE_RATE * 0.5f) {
                float amp = (1.0f / (float)h) / amp_sum;
                sample += sinf(phases[h - 1]) * amp;
                
                phases[h - 1] += 2.0f * (float)M_PI * h_freq / SAMPLE_RATE;
                if (phases[h - 1] >= 2.0f * (float)M_PI) {
                    phases[h - 1] -= 2.0f * (float)M_PI;
                }
            }
        }

        if (fwrite(&sample, sizeof(float), 1, stdout) != 1) break;
    }

    return 0;
}
