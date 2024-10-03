#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "parse_args.h"

typedef struct option t_getopt_option;

void print_usage(const char* const prog_name) {
  printf("Usage: %s [OPTION...] HOST\n", prog_name);
  printf("Options:\n");
  printf("  -v, --verbose    print verbose messages\n");
  printf("  -?, --help       display this help and exit\n");
  printf("      --ttl=NUM    set time-to-live for the cache\n");
}

int panic(const char* const prog_name, const char* const detail) {
  if (detail)
    fprintf(stderr, "%s: %s\n", prog_name, detail);
  fprintf(stderr, "Try '%1$s -?' or '%1$s --help' for more information.\n", prog_name);

  return EXIT_FAILURE;
}

int parse_ttl(const char* const ttl_str, unsigned long int* const ttl) {
  char *endptr;

  *ttl = strtoul(ttl_str, &endptr, 10);
  if (*endptr != '\0' || errno != 0 || *ttl > 255)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int parse_args(int argc, char *argv[], t_flags *flags) {
  int opt;
  int option_index = 0;
  const t_getopt_option long_options[] = {
      {"verbose", no_argument,       NULL, 'v'},
      {"help",    no_argument,       NULL, '?'},
      {"ttl",     required_argument, NULL,  0 },
      {0, 0, 0, 0},
  };

  while ((opt = getopt_long(argc, argv, "v?", long_options, &option_index)) != -1) {
    switch (opt) {
    case 0:
      if (strcmp(long_options[option_index].name, "ttl") == 0)
        if (parse_ttl(optarg, &flags->ttl) == EXIT_FAILURE)
          return panic(argv[0], "Invalid TTL value");
      break;

    case 'v':
      flags->verbose = 1;
      break;

    case '?':
      if (
        strcmp(argv[optind - 1], "-?") == 0 ||
        strcmp(argv[optind - 1], "--help") == 0
        ) {
        flags->help = 1;
        print_usage(argv[0]);
        return EXIT_FAILURE;
      }
      __attribute__((fallthrough));

    default:
      return panic(argv[0], NULL);
    }
  }

  if (optind + 1 != argc) {
    return panic(
      argv[0],
      optind == argc ?
      "Missing address or hostname after options" :
      "Too many arguments were provided"
    );
  }

  return EXIT_SUCCESS;
}
