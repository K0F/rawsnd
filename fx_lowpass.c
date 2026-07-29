#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main(int argc, char *argv[]) {
    float cutoff = (argc > 1) ? atof(argv[1]) : 1000.0f; // Výchozí 1 kHz
    float sample_rate = 44100.0f;

    float dt = 1.0f / sample_rate;
    float rc = 1.0f / (cutoff * 2.0f * (float)M_PI);
    float alpha = dt / (rc + dt);

    float y = 0.0f; // Stav filtru
    float x;

    while (fread(&x, sizeof(float), 1, stdin) == 1) {
        y = y + alpha * (x - y);
        if (fwrite(&y, sizeof(float), 1, stdout) != 1) break;
    }

    return 0;
}
