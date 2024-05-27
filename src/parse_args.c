#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int parse_args(int argc, char *argv[]) {
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
      printf("option %s", long_options[option_index].name);
      if (optarg)
        printf(" with arg %s", optarg);
      printf("\n");
      break;

    case 'v':
      printf("option -v\n");
      break;

    case '?':
      if (
        strcmp(argv[optind - 1], "-?") == 0 ||
        strcmp(argv[optind - 1], "--help") == 0
        ) {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
      }
      __attribute__((fallthrough));

    default:
      return panic(argv[0], NULL);
    }
  }

  if (optind == argc) {
    return panic(argv[0], "Missing address or hostname after options");
  }
  else if (optind + 1 < argc) {
    return panic(argv[0], "Too many arguments were provided");
  }

  // const char *host = argv[optind];

  return EXIT_SUCCESS;
}
