#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static void usage(void);

static char *prog_name;


int main(int argc, char *argv[]) {

    prog_name = argv[0];

    int opt;
    int sflag = 0;
    int kflag = 0;
    while ((opt = getopt(argc, argv, "sk")) != -1) {
        switch (opt) {
            case 's':
                sflag = 1;
                break;
            case 'k':
                kflag = 1;
                break;
            case '?':
            default:
                usage();
                return EXIT_FAILURE;
        }
    }
    argc -= optind;
    argv += optind;

    FILE *f;
    if (argc == 0) {
        f = stdin;
    } else if (argc == 1) {
        f = fopen(argv[0], "rb");
        if (f == NULL) {
            fprintf(stderr, "%s: %s", prog_name, strerror(errno));
            fprintf(stderr, "\n");
            return EXIT_FAILURE;
        }
    } else {
        usage();
        return EXIT_FAILURE;
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

    if (sflag) {
        printf("%f\n", entropy);
    }
    if (kflag) {
        printf("%f\n", entropy/len);
    }
    if (!sflag && !kflag) {
        printf("Shannon Entropy: %f\n", entropy);
        printf("Kolmogorov Entropy: %f\n", entropy/len);
    }

}

void usage(void) {
    fprintf(stderr, "%s: usage: %s [-sk] input_file\n", prog_name, prog_name);
}
