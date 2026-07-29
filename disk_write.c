#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SAMPLE_RATE 44100
#define CHANNELS 1
#define BITS_PER_SAMPLE 32

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Použití: %s <vystupni_soubor.wav>\n", argv[0]);
        fprintf(stderr, "Příklad: ./gen_sine 440 | %s vystup.wav\n", argv[0]);
        return 1;
    }

    FILE *fout = fopen(argv[1], "wb");
    if (!fout) {
        perror("Chyba při otevírání výstupního souboru");
        return 1;
    }

    // --- WAV header (44 bajtů placeholder) ---
    // RIFF chunk
    fwrite("RIFF", 1, 4, fout);
    uint32_t chunk_size = 0; // Bude přepsáno na konci
    fwrite(&chunk_size, 4, 1, fout);
    fwrite("WAVE", 1, 4, fout);

    // fmt subchunk
    fwrite("fmt ", 1, 4, fout);
    uint32_t subchunk1_size = 16; // 16 pro PCM / IEEE float
    fwrite(&subchunk1_size, 4, 1, fout);
    
    uint16_t audio_format = 3; // 3 = IEEE float (podpora pro 32-bit float data)
    fwrite(&audio_format, 2, 1, fout);
    
    uint16_t num_channels = CHANNELS;
    fwrite(&num_channels, 2, 1, fout);
    
    uint32_t sample_rate = SAMPLE_RATE;
    fwrite(&sample_rate, 4, 1, fout);
    
    uint32_t byte_rate = SAMPLE_RATE * CHANNELS * (BITS_PER_SAMPLE / 8);
    fwrite(&byte_rate, 4, 1, fout);
    
    uint16_t block_align = CHANNELS * (BITS_PER_SAMPLE / 8);
    fwrite(&block_align, 2, 1, fout);
    
    uint16_t bits_per_sample = BITS_PER_SAMPLE;
    fwrite(&bits_per_sample, 2, 1, fout);

    fwrite("data", 1, 4, fout);
    uint32_t subchunk2_size = 0; // Bude přepsáno na konci
    fwrite(&subchunk2_size, 4, 1, fout);

    float buffer[512];
    size_t n;
    uint32_t data_bytes_written = 0;

    // new header size
    while ((n = fread(buffer, sizeof(float), 512, stdin)) > 0) {
        size_t written = fwrite(buffer, sizeof(float), n, fout);
        data_bytes_written += written * sizeof(float);
        if (written != n) {
            break;
        }
    }

    chunk_size = 36 + data_bytes_written;
    subchunk2_size = data_bytes_written;

    if (fseek(fout, 0, SEEK_SET) == 0) {
        // RIFF chunk size (pozice 4)
        fseek(fout, 4, SEEK_SET);
        fwrite(&chunk_size, 4, 1, fout);

        // data subchunk size (pozice 40)
        fseek(fout, 40, SEEK_SET);
        fwrite(&subchunk2_size, 4, 1, fout);
    }

    fclose(fout);
    fprintf(stderr, "[+] Úspěšně uloženo do WAV: %s (%u bajtů audio dat)\n", argv[1], data_bytes_written);
    return 0;
}
