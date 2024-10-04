#include "ft_ping.h"
#include "socket.h"
#include <stdlib.h>
#include <string.h>

void init_ping(t_ping *ping) {
  memset(ping, 0, sizeof(t_ping));
  ping->flags.ttl = 64;
}

int main(int argc, char **argv) {
  t_ping ping;

  init_ping(&ping);
  if (parse_args(argc, argv, &ping.flags) != 0)
    return ping.flags.help ? 2 : EXIT_FAILURE;

  hostname_to_socket(argv[argc - 1], &ping);
  ft_ping(&ping);
  return EXIT_SUCCESS;
}
