#include "ft_ping.h"
#include "socket.h"
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

t_ping ping;

void print_ping_header(const t_ping ping) {
  printf("PING %s (%s): %lu data bytes", ping.hostname, ping.ipaddr, PACKET_PAYLOAD_SIZE);

  if (ping.flags.verbose)
    printf(", id 0x%1$x = %1$d", getpid());

  printf("\n");
}

void init_ping(t_ping *ping) {
  memset(ping, 0, sizeof(t_ping));
  ping->flags.ttl = 64;
}

int main(int argc, char **argv) {
  init_ping(&ping);

  if (!parse_args(argc, argv, &ping.flags))
    return ping.flags.help ? 2 : EXIT_FAILURE;

  ping.program_name = argv[0];
  ping.hostname = argv[argc - 1];

  if (
    !dns_lookup(ping.hostname, &ping) ||
    !create_raw_socket(&ping) ||
    !ipv4_to_string(&ping.sockaddr.sin_addr, ping.ipaddr)
    )
    return EXIT_FAILURE;

  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGALRM, ft_ping);

  print_ping_header(ping);
  ft_ping(0);

  while (1) {
    pause();
  }

  return EXIT_SUCCESS;
}
