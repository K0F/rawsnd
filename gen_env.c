#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Použití: %s <cesta_k_fifo_druhe_linky>\n", argv[0]);
        return 1;
    }

    FILE *f2 = fopen(argv[1], "rb");
    if (!f2) {
        perror("Chyba při otevírání druhého streamu");
        return 1;
    }

    float buf1[BUFFER_SIZE];
    float buf2[BUFFER_SIZE];
    size_t n1, n2;

    while (1) {
        n1 = fread(buf1, sizeof(float), BUFFER_SIZE, stdin);
        n2 = fread(buf2, sizeof(float), BUFFER_SIZE, f2);

        if (n1 == 0 && n2 == 0) break;

        size_t min_n = (n1 < n2) ? n1 : n2;
        for (size_t i = 0; i < min_n; i++) {
            buf1[i] = (buf1[i] + buf2[i]) * 0.5f;
        }

        fwrite(buf1, sizeof(float), min_n, stdout);
        fflush(stdout);

        if (n1 == 0 || n2 == 0) break;
    }

    fclose(f2);
    return 0;
}
