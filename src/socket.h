#ifndef SOCKET_H
# define SOCKET_H

# include "ft_ping.h"

int dns_lookup(const char *const hostname, t_ping *ping);
int create_raw_socket(t_ping *ping);

const char *ipv4_to_string(const struct in_addr *addr, char dest[INET_ADDRSTRLEN]);

#endif
