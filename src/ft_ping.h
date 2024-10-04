#ifndef FT_PING_H
# define FT_PING_H

# include "parse_args.h"
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>

typedef struct sockaddr sockaddr_t;
typedef struct sockaddr_in sockaddr_in_t;

typedef struct s_ping {
  int             sock_fd;
  sockaddr_in_t   sockaddr;
  t_flags         flags;
} t_ping;

void ft_ping(t_ping *ping);

#endif
