#include "ft_ping.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int hostname_to_socket(const char *const hostname, t_ping *ping) {
  int status;
  struct addrinfo hints;
  struct addrinfo *res = NULL;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;

  if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0 || !res) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 0;
  }

  memcpy(&ping->sock_addr, res->ai_addr, res->ai_addrlen);
  freeaddrinfo(res);
  return 1;
}

int create_raw_socket(t_ping *ping) {
  if ((ping->sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
    fprintf(stderr, "Couldn't create raw socket, do you have root privileges?\n");
    return 0;
  }
  return 1;
}
