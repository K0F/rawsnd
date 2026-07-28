#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Použití: %s <cesta_k_fifo_obalky>\n", argv[0]);
        return 1;
    }

    FILE *f_env = fopen(argv[1], "rb");
    if (!f_env) {
        perror("Chyba při otevírání modulačního FIFO");
        return 1;
    }

    float buf_audio[BUFFER_SIZE];
    float buf_env[BUFFER_SIZE];
    size_t n1, n2;

    while (1) {
        n1 = fread(buf_audio, sizeof(float), BUFFER_SIZE, stdin);
        n2 = fread(buf_env, sizeof(float), BUFFER_SIZE, f_env);

        if (n1 == 0 && n2 == 0) break;

        size_t min_n = (n1 < n2) ? n1 : n2;
        for (size_t i = 0; i < min_n; i++) {
            buf_audio[i] *= buf_env[i]; // mul each sample 
        }

        fwrite(buf_audio, sizeof(float), min_n, stdout);
        fflush(stdout);

        if (n1 == 0 || n2 == 0) break;
    }

    fclose(f_env);
    return 0;
}
