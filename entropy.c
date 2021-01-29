#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Options {
	char kflag;
	char pflag;
	char sflag;
} Options;

static double kolmogorov(double entropy, unsigned int len);
static void entropy(FILE *inf, Options options);
static void usage(void);

int
main(int argc, char *argv[])
{
	char *prog_name = argv[0];
	Options options = {0};

	int opt;
	while ((opt = getopt(argc, argv, "hkps")) != -1) {
		switch (opt) {
		case 'h':
			usage();
			return EXIT_SUCCESS;
		case 'k':
			options.kflag = 1;
			break;
		case 'p':
			options.pflag = 1;
			break;
		case 's':
			options.sflag = 1;
			break;
		case '?':   /* FALLTHROUGH */
		default:
			usage();
			return EXIT_FAILURE;
		}
	}

	if (argc > optind) {
		for (int i = optind; i < argc; i++) {
			FILE *inf = fopen(argv[i], "rb");
			if (inf == NULL) {
				perror(prog_name);
				exit(EXIT_FAILURE);
			}
			if (options.pflag) {
				printf("%s\n", argv[i]);
			}
			entropy(inf, options);
			fclose(inf);
		}
	} else {
		entropy(stdin, options);
	}
	exit(EXIT_SUCCESS);
}

void
entropy(FILE *inf, Options options)
{
	int bytes[256] = {0};
	unsigned char buffer[BUFSIZ];
	unsigned int len = 0;
	unsigned long read_count = 0;

	while ((read_count = fread(buffer, 1, BUFSIZ, inf)) > 0) {
		for (unsigned int i = 0; i < read_count; i++) {
			bytes[buffer[i]]++;
			len++;
		}
	}

	double prob = 0;
	double ent = 0;

	if (len > 0) {
		for (int i = 0; i < 256; i++) {
			prob = bytes[i] / (double)len;
			if (prob > 0) {
				ent -= prob* log2(prob);
			}
		}
	}

	if (options.sflag) {
		printf("%f\n", ent);
	}

	if (options.kflag) {
		printf("%f\n", kolmogorov(ent, len));
	}

	if (!options.sflag && !options.kflag) {
		printf("Shannon Entropy: %f\n", ent);
		printf("Kolmogorov Entropy: %f\n", kolmogorov(ent, len));
	}
}

double
kolmogorov(double entropy, unsigned int len)
{
	if (len == 0) {
		return 0;
	} else {
		return entropy/len;
	}
}

void
usage(void)
{
	fputs("usage: entropy [-ks] [file...]\n       entropy [-kps] file [file...]\n       entropy -h", stderr);
}

