#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parseargs.h"

static const struct option opts[] = {
    {"size",        1, NULL, 'n'},
    {"step",        1, NULL, 'm'},
    {"direction",   1, NULL, 'd'},
    {"help",        0, NULL, 'h'},
    {0, 0, 0, 0}
};
extern char *optarg;

int printHelpInfo(const char *baseName) {
    printf("Usage:\t%s OPTIONS...\n\n", baseName);
    puts("Mandatory arguments to long options are mandatory for short options too.");
    puts("  -d, --direction    Optional. The direction of the game. 0 for counterclockwise, 1 for clockwise. 0 by default.");
    puts("  -h, --help         Display this help and exit.");
    puts("  -m, --step         The number of people to be skipped in each round. It must be an integer in range [0, 5000].");
    puts("  -n, --size         The number of people to play the Josephus game. It must be an integer in range [0, 5000].");
    return 0;
}

void parseArgs(int argc, char **argv, int *size, int *step, int *dir) {
    const char *optstr = "n:m:d:h";
    int opt;
    
    while((opt = getopt_long(argc, argv, optstr, opts, NULL)) != -1) {
        switch(opt){
            case 'n':
                if(sscanf(optarg, "%d", size) != 1) {
                    fputs("Argument of option '-n'/'--size' must be an integer.\n", stderr);
                    exit(-1);
                } else if(*size < 0 || *size > 5000) {
                    fputs("Argument of option '-n'/'--size' must be in range [0, 5000].\n", stderr);
                    exit(-1);
                }
                break;
            case 'm':
                if(sscanf(optarg, "%d", step) != 1) {
                    fputs("Argument of option '-m'/'--step' must be an integer.\n", stderr);
                    exit(-1);
                } else if(*step < 0 || *step > 5000) {
                    fputs("Argument of option '-m'/'--step' must be in range [0, 5000].\n", stderr);
                    exit(-1);
                }
                break;
            case 'd':
                if(sscanf(optarg, "%d", dir) != 1 || *dir < 0 || *dir > 1) {
                    fputs("Argument of option '-d'/'--direction' must be 0 or 1.\n", stderr);
                    exit(-1);
                }
                break;
            case 'h':
                exit(printHelpInfo(argv[0]));
            case '?':
                if(strchr(optstr, optopt) == NULL){
                    fprintf(stderr, "Unknown option '-%c'\n", optopt);
                } else {
                    fprintf(stderr, "Option '-%c' requires an argument\n", optopt);
                }
                exit(-1);
        }
    }

    if(*size == -1) {
        fputs("Option '-n'/'--size' not found.\n", stderr);
        exit(-1);
    } else if(*step == -1) {
        fputs("Option '-m'/'--step' not found.\n", stderr);
        exit(-1);
    }
}
