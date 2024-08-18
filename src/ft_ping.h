#ifndef FT_PING_H
# define FT_PING_H

# include "parse_args.h"
# include <sys/socket.h>
# include <sys/types.h>

typedef struct s_ping {
  int             sock_fd;
  struct sockaddr sock_addr;
  t_flags         flags;
} t_ping;

#endif
