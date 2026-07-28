#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define SAMPLE_RATE 44100.0f
#define BUFFER_SIZE 512

int main(int argc, char *argv[]) {
    float default_freq = (argc > 1) ? atof(argv[1]) : 440.0f;
    float current_freq = default_freq;
    
    int has_input = !isatty(fileno(stdin));

    float phase = 0.0f;
    float audio_buffer[BUFFER_SIZE];
    float freq_buffer[BUFFER_SIZE];

    if (!has_input) {
        float dt = 2.0f * M_PI * current_freq / SAMPLE_RATE;
        while (1) {
            for (int i = 0; i < BUFFER_SIZE; i++) {
                audio_buffer[i] = sinf(phase) * 0.2f;
                phase += dt;
                if (phase >= 2.0f * M_PI) phase -= 2.0f * M_PI;
            }
            if (fwrite(audio_buffer, sizeof(float), BUFFER_SIZE, stdout) != BUFFER_SIZE) break;
            fflush(stdout);
        }
    } else {
        size_t n;
        while ((n = fread(freq_buffer, sizeof(float), BUFFER_SIZE, stdin)) > 0) {
            for (size_t i = 0; i < n; i++) {
                current_freq = freq_buffer[i];
                if (current_freq < 1.0f) current_freq = 1.0f; 

                float dt = 2.0f * M_PI * current_freq / SAMPLE_RATE;
                
                audio_buffer[i] = sinf(phase) * 0.2f;
                phase += dt;
                if (phase >= 2.0f * M_PI) phase -= 2.0f * M_PI;
            }
            if (fwrite(audio_buffer, sizeof(float), n, stdout) != n) break;
            fflush(stdout);
        }
    }

    return 0;
}
