#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[]) {
    float out_min = (argc > 1) ? atof(argv[1]) : 0.0f;
    float out_max = (argc > 2) ? atof(argv[2]) : 1.0f;

    float in_min = -1.0f;
    float in_max = 1.0f;

    float buffer[BUFFER_SIZE];
    size_t n;

    while ((n = fread(buffer, sizeof(float), BUFFER_SIZE, stdin)) > 0) {
        for (size_t i = 0; i < n; i++) {
            float norm = (buffer[i] - in_min) / (in_max - in_min);
            if (norm < 0.0f) norm = 0.0f;
            if (norm > 1.0f) norm = 1.0f;

            buffer[i] = out_min + norm * (out_max - out_min);
        }
        fwrite(buffer, sizeof(float), n, stdout);
        fflush(stdout);
    }

    return 0;
}
