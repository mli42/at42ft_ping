#include "socket.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

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
    fprintf(stderr, "%s: %s: %s\n", ping->program_name, hostname, gai_strerror(status));
    return 0;
  }

  memcpy(&ping->sockaddr, res->ai_addr, sizeof(ping->sockaddr));
  freeaddrinfo(res);
  return 1;
}

int create_raw_socket(t_ping *ping) {
  int on = 1;

  if ((ping->sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
    fprintf(stderr, "%s: socket: %s\n", ping->program_name, strerror(errno));
    return 0;
  }

  if (setsockopt(ping->sock_fd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) == -1) {
    fprintf(stderr, "%s: setsockopt: %s\n", ping->program_name, strerror(errno));
    return 0;
  }
  return 1;
}

const char *ipv4_to_string(struct in_addr *addr, char dest[INET_ADDRSTRLEN]) {
  return inet_ntop(AF_INET, addr, dest, INET_ADDRSTRLEN);
}
