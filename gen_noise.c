#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    srand((unsigned int)time(NULL));

    while (1) {
        // white noise
        float sample = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;

        if (fwrite(&sample, sizeof(float), 1, stdout) != 1) {
            break; 
        }
    }

    return 0;
}
