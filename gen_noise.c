#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    float amp = (argc > 1) ? atof(argv[1]) : 0.2f; // Výchozí amplituda

    srand((unsigned int)time(NULL));

    while (1) {
        // white noise vynásobený amplitudou
        float sample = (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * amp;

        if (fwrite(&sample, sizeof(float), 1, stdout) != 1) {
            break; 
        }
    }

    return 0;
}
