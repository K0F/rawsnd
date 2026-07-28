#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>

#define MAX_INPUTS 16

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Použití: %s <fifo1> [fifo2] [fifo3] ...\n", argv[0]);
        return 1;
    }

    int num_inputs = argc - 1;
    if (num_inputs > MAX_INPUTS) {
        fprintf(stderr, "Příliš mnoho vstupů (maximum je %d)\n", MAX_INPUTS);
        num_inputs = MAX_INPUTS;
    }

    int fds[MAX_INPUTS];
    struct pollfd pfds[MAX_INPUTS];

    // FIFO in non-blocking mode
    for (int i = 0; i < num_inputs; i++) {
        fds[i] = open(argv[i + 1], O_RDONLY | O_NONBLOCK);
        if (fds[i] < 0) {
            perror("Chyba při otevírání FIFO");
            
            for (int j = 0; j < i; j++) close(fds[j]);
            return 1;
        }
        pfds[i].fd = fds[i];
        pfds[i].events = POLLIN;
    }

    // audio loop
    while (1) {
        // 10ms for data
        int ret = poll(pfds, num_inputs, 10);
        if (ret < 0) {
            if (errno == EINTR) continue;
            break; // Kritická chyba
        }

        float mixed_sample = 0.0f;

        
        for (int i = 0; i < num_inputs; i++) {
            float sample = 0.0f;
            ssize_t n = read(fds[i], &sample, sizeof(float));
            
            if (n == sizeof(float)) {
                // add to mix
                mixed_sample += sample;
            } else if (n < 0) {
                // or silent (0.0f)
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                
                }
            }
        }

        mixed_sample /= (float)num_inputs;

        if (fwrite(&mixed_sample, sizeof(float), 1, stdout) != 1) {
            break; 
        }
    }

    for (int i = 0; i < num_inputs; i++) {
        close(fds[i]);
    }

    return 0;
}
