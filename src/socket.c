#include "ft_ping.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int dns_lookup(const char *const hostname, t_ping *ping) {
  int status;
  struct addrinfo hints;
  struct addrinfo *res = NULL;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_flags = AI_ADDRCONFIG;
  hints.ai_socktype = SOCK_RAW;
  hints.ai_protocol = IPPROTO_ICMP;

  if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0 || !res) {
    fprintf(stderr, "ft_ping: %s: %s\n", hostname, gai_strerror(status));
    return 0;
  }

  memcpy(&ping->sockaddr, res->ai_addr, sizeof(ping->sockaddr));
  freeaddrinfo(res);
  return 1;
}

int create_raw_socket(t_ping *ping) {
  if ((ping->sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
    fprintf(stderr, "ft_ping: Couldn't create raw socket, do you have root privileges?\n");
    return 0;
  }
  return 1;
}
