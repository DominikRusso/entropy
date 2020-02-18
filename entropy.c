#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static double calc_kolmogorov(double entropy, unsigned int len);
static void usage(void);

int
main(int argc, char *argv[])
{
    char *prog_name = argv[0];

    int opt;
    int sflag = 0;
    int kflag = 0;
    while ((opt = getopt(argc, argv, "sk")) != -1) {
        switch (opt) {
        case 'h':
            usage();
            return EXIT_SUCCESS;
        case 's':
            sflag = 1;
            break;
        case 'k':
            kflag = 1;
            break;
        case '?':   /* FALLTHROUGH */
        default:
            usage();
            return EXIT_FAILURE;
        }
    }

    FILE *fp;
    if (argc == optind) {
        fp = stdin;
    } else if (argc == optind + 1) {
        fp = fopen(argv[optind], "rb");
        if (fp == NULL) {
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
    unsigned int len = 0;
    unsigned long read_count = 0;

    while ((read_count = fread(buffer, 1, BUFSIZ, fp)) > 0) {
        for (unsigned int i = 0; i < read_count; i++) {
            bytes[buffer[i]]++;
            len++;
        }
    }

    fclose(fp);

    double probability = 0;
    double entropy = 0;

    for (int i = 0; i < 256; i++) {
        probability = bytes[i] / (double)len;
        if (probability > 0) {
            entropy -= probability * log2(probability);
        }
    }

    if (sflag) {
        printf("%f\n", entropy);
    }
    if (kflag) {
        printf("%f\n", calc_kolmogorov(entropy, len));
    }
    if (!sflag && !kflag) {
        printf("Shannon Entropy: %f\n", entropy);
        printf("Kolmogorov Entropy: %f\n", calc_kolmogorov(entropy, len));
    }
}

static double
calc_kolmogorov(double entropy, unsigned int len)
{
    if (entropy == 0) {
        return 0;
    } else {
        return entropy/len;
    }
}

static void
usage(void)
{
    fprintf(stderr, "usage: entropy [-hsk] [file]\n");
}

