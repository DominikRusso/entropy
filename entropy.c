#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static void usage(void);

char *prog_name;


int main(int argc, char *argv[]) {

    prog_name = argv[0];

    if (argc != 2) {
        usage();
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (f == NULL) {
        fprintf(stderr, "%s: %s", prog_name, strerror(errno));
        return 1;
    }

    int bytes[256] = {0};
    unsigned char buffer[BUFSIZ];
    int len = 0;
    int read_count = 0;

    while ((read_count = fread(buffer, 1, BUFSIZ, f)) > 0) {
        for (int i = 0; i < read_count; i++) {
            bytes[buffer[i]]++;
            len++;
        }
    }

    fclose(f);

    double probability = 0;
    double entropy = 0;

    for (int i = 0; i < 256; i++) {
        probability = bytes[i] / (float)len;
        if (probability > 0) {
            entropy -= probability * log2(probability);
        }
    }

    printf("Shannon Entropy: %f\n", entropy);
    printf("Kolmogorov Entropy: %f\n", entropy / len);

}


void usage(void) {
    fprintf(stderr, "%s: usage: %s input_file\n", prog_name, prog_name);
}
