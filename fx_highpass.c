#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main(int argc, char *argv[]) {
    float cutoff = (argc > 1) ? atof(argv[1]) : 200.0f; // Výchozí 200 Hz
    float sample_rate = 44100.0f;

    float dt = 1.0f / sample_rate;
    float rc = 1.0f / (cutoff * 2.0f * (float)M_PI);
    float alpha = rc / (rc + dt);

    float x_prev = 0.0f;
    float y_prev = 0.0f;
    float x, y;

    while (fread(&x, sizeof(float), 1, stdin) == 1) {
        y = alpha * (y_prev + x - x_prev);
        x_prev = x;
        y_prev = y;
        
        if (fwrite(&y, sizeof(float), 1, stdout) != 1) break;
    }

    return 0;
}
