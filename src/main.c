#include "parse_args.h"
#include <stdlib.h>

int main(int argc, char **argv) {
  t_flags flags = { 0, 0, 0 };

  if (parse_args(argc, argv, &flags) != 0)
    return flags.help ? EXIT_SUCCESS : EXIT_FAILURE;
  return EXIT_SUCCESS;
}
