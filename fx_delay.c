#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAMPLE_RATE 44100.0f
#define MAX_DELAY_SECONDS 5.0f

int main(int argc, char *argv[]) {
    // Parametry: <cas_v_sekundách> <feedback (0.0 - 0.99)> <wet_mix (0.0 - 1.0)>
    float delay_time = (argc > 1) ? atof(argv[1]) : 0.3f;   // Výchozí 300 ms
    float feedback   = (argc > 2) ? atof(argv[2]) : 0.4f;   // Výchozí 40% feedback
    float wet_mix    = (argc > 3) ? atof(argv[3]) : 0.5f;   // Výchozí 50% wet/dry

    int delay_samples = (int)(delay_time * SAMPLE_RATE);
    int max_buffer_size = (int)(MAX_DELAY_SECONDS * SAMPLE_RATE);

    if (delay_samples < 1) delay_samples = 1;
    if (delay_samples >= max_buffer_size) delay_samples = max_buffer_size - 1;

    // Alokace kruhového bufferu pro paměť zpoždění
    float *buffer = (float *)calloc(max_buffer_size, sizeof(float));
    if (!buffer) {
        perror("[CHYBA] Alokace paměti pro delay selhala");
        return 1;
    }

    int write_idx = 0;
    float x, y;

    while (fread(&x, sizeof(float), 1, stdin) == 1) {
        // Výpočet pozice pro čtení zpožděného vzorku
        int read_idx = (write_idx - delay_samples + max_buffer_size) % max_buffer_size;
        float delayed_sample = buffer[read_idx];

        // Do bufferu jde vstup + utlumený zpožděný signál (feedback)
        float input_to_delay = x + (delayed_sample * feedback);
        buffer[write_idx] = input_to_delay;

        // Výsledný mix (dry/wet)
        y = (x * (1.0f - wet_mix)) + (delayed_sample * wet_mix);

        if (fwrite(&y, sizeof(float), 1, stdout) != 1) {
            break;
        }

        write_idx = (write_idx + 1) % max_buffer_size;
    }

    free(buffer);
    return 0;
}
