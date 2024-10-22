#ifndef UTILS_ICMP_H
# define UTILS_ICMP_H

# include "../ft_ping.h"

# define IPHDR_DUMP_LEN 50

uint16_t checksum(void *data, size_t size);
int is_valid_checksum(t_icmp_packet *packet, size_t size);
const char *get_error_message(const t_icmp_packet *const packet);
char *get_iphdr_dump(const struct iphdr *const iphdr, char dest[IPHDR_DUMP_LEN]);

#endif
