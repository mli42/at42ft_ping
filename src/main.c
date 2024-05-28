#include "parse_args.h"
#include <stdlib.h>

int main(int argc, char **argv) {
  if (parse_args(argc, argv) != 0)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
