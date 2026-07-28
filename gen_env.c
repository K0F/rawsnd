#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SAMPLE_RATE 44100.0f
#define BUFFER_SIZE 512

int main(int argc, char *argv[]) {
    float trig_freq = (argc > 1) ? atof(argv[1]) : 2.0f; // Údery za sekundu (Hz)
    float decay = (argc > 2) ? atof(argv[2]) : 4.0f;     // Rychlost doznívání

    float phase = 0.0f;
    float dt = trig_freq / SAMPLE_RATE;
    float buffer[BUFFER_SIZE];

    while (1) {
        for (int i = 0; i < BUFFER_SIZE; i++) {
            float t = fmodf(phase, 1.0f);
            float env = expf(-t * decay * 5.0f);
            
            buffer[i] = env;
            phase += dt;
        }

        if (fwrite(buffer, sizeof(float), BUFFER_SIZE, stdout) != BUFFER_SIZE) break;
        fflush(stdout);
    }

    return 0;
}
