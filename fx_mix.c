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

    while (1) {
        size_t n1 = fread(buf1, sizeof(float), BUFFER_SIZE, stdin);
        if (n1 == 0) break;

        size_t n2 = fread(buf2, sizeof(float), BUFFER_SIZE, f2);
        
        // Pokud druhá linka nestíhá nebo dočasně nemá data, doplníme ticho (nuly)
        if (n2 < n1) {
            for (size_t i = n2; i < n1; i++) {
                buf2[i] = 0.0f;
            }
        }

        for (size_t i = 0; i < n1; i++) {
            buf1[i] = (buf1[i] + buf2[i]) * 0.5f;
        }

        if (fwrite(buf1, sizeof(float), n1, stdout) != n1) break;
        fflush(stdout);
    }

    fclose(f2);
    return 0;
}
