#ifndef SOCKET_H
# define SOCKET_H

# include "ft_ping.h"

int hostname_to_socket(const char *const hostname, t_ping *ping);
int create_raw_socket(t_ping *ping);

#endif
