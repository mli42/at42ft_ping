#ifndef FT_PING_H
# define FT_PING_H

# include "parse_args.h"
# include <stdio.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <netinet/ip_icmp.h>

# define PACKET_SIZE 64
# define PAYLOAD_SIZE (PACKET_SIZE - sizeof(struct icmphdr))

typedef struct sockaddr sockaddr_t;
typedef struct sockaddr_in sockaddr_in_t;

typedef struct s_ping {
  int             sock_fd;
  sockaddr_in_t   sockaddr;
  t_flags         flags;

  const char      *program_name;
  const char      *hostname;
  char            ipaddr[INET_ADDRSTRLEN];
} t_ping;

typedef struct s_icmp_packet {
  struct icmphdr  icmphdr;
  char            payload[PAYLOAD_SIZE];
} t_icmp_packet;

extern t_ping ping;

void ft_ping(int dummy);
void signal_handler(int dummy);

#endif
