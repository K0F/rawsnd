#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SAMPLE_RATE 44100.0f
#define BUFFER_SIZE 512

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

int main(int argc, char *argv[]) {
    float freq = (argc > 1) ? atof(argv[1]) : 1.0f;        // default 1 Hz
    char *type = (argc > 2) ? argv[2] : "saw";            // saw, tri nebo sine

    float phase = 0.0f;
    float phase_inc = freq / SAMPLE_RATE;
    float buffer[BUFFER_SIZE];

    while (1) {
        for (int i = 0; i < BUFFER_SIZE; i++) {
            float val = 0.0f;

            if (strcmp(type, "tri") == 0) {
                if (phase < 0.5f) {
                    val = phase * 4.0f - 1.0f;
                } else {
                    val = (1.0f - phase) * 4.0f - 1.0f;
                }
            } else if (strcmp(type, "sine") == 0) {
                val = sinf(phase * 2.0f * (float)M_PI);
            } else {
                val = phase * 2.0f - 1.0f;
            }

            buffer[i] = val;
            phase += phase_inc;
            if (phase >= 1.0f) {
                phase -= 1.0f;
            }
        }

        if (fwrite(buffer, sizeof(float), BUFFER_SIZE, stdout) != BUFFER_SIZE) {
            break;
        }
        fflush(stdout);
    }

    return 0;
}
