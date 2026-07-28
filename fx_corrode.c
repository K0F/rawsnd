#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    int downsample = 3;       
    float crush_bits = 8.0f; 

    if (argc > 1) downsample = atoi(argv[1]);
    if (argc > 2) crush_bits = atof(argv[2]);
    if (downsample < 1) downsample = 1;

    float sample;
    float held_sample = 0.0f;
    long count = 0;

    // Výpočet počtu kvantizačních kroků
    float steps = powf(2.0f, crush_bits);

    while (fread(&sample, sizeof(float), 1, stdin) == 1) {
        // Sample-rate reduction 
        if (count % downsample == 0) {
            // Bit-crush
            if (steps > 1.0f) {
                sample = floorf(sample * steps + 0.5f) / steps;
            }
            held_sample = sample;
        } else {
            sample = held_sample;
        }

        if (fwrite(&sample, sizeof(float), 1, stdout) != 1) break;
        count++;
    }

    return 0;
}
