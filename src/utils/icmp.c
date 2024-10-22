#include "icmp.h"
#include <string.h>

uint16_t checksum(void *data, size_t size) {
  uint16_t *ptr = data;
  uint32_t sum = 0;

  while (size > 1) {
    sum += *ptr++;
    size -= 2;
  }
  if (size > 0) {
    sum += *(uint8_t *)ptr;
  }
  while (sum >> 16)
    sum = (sum & 0xffff) + (sum >> 16);
  return ~sum;
}

int is_valid_checksum(t_icmp_packet *const packet, size_t size) {
  const uint16_t checksum_copy = packet->icmphdr.checksum;
  uint16_t new_checksum;

  packet->icmphdr.checksum = 0;
  new_checksum = checksum(packet, size);
  packet->icmphdr.checksum = checksum_copy;
  return new_checksum == checksum_copy;
}

const char *get_error_message(const t_icmp_packet *const packet) {
  switch (packet->icmphdr.type) {
  case ICMP_ECHOREPLY:
    return "Echo Reply";
  case ICMP_DEST_UNREACH:
    switch (packet->icmphdr.code) {
    case ICMP_NET_UNREACH:
      return "Network Unreachable";
    case ICMP_HOST_UNREACH:
      return "Host Unreachable";
    case ICMP_PROT_UNREACH:
      return "Protocol Unreachable";
    case ICMP_PORT_UNREACH:
      return "Port Unreachable";
    case ICMP_FRAG_NEEDED:
      return "Fragmentation Needed/DF set";
    case ICMP_SR_FAILED:
      return "Source Route failed";
    default:
      return "Destination Unreachable";
    }
  case ICMP_SOURCE_QUENCH:
    return "Source Quench";
  case ICMP_REDIRECT:
    return "Redirect (change route)";
  case ICMP_ECHO:
    return "Echo Request";
  case ICMP_TIME_EXCEEDED:
    return "Time to live exceeded";
  case ICMP_PARAMETERPROB:
    return "Parameter Problem";
  case ICMP_TIMESTAMP:
    return "Timestamp Request";
  case ICMP_TIMESTAMPREPLY:
    return "Timestamp Reply";
  case ICMP_INFO_REQUEST:
    return "Information Request";
  case ICMP_INFO_REPLY:
    return "Information Reply";
  case ICMP_ADDRESS:
    return "Address Mask Request";
  case ICMP_ADDRESSREPLY:
    return "Address Mask Reply";
  default:
    return "Unexpected type";
  }
}

char *get_iphdr_dump(const struct iphdr *const iphdr, char dest[IPHDR_DUMP_LEN]) {
  char *dest_copy = dest;
  const unsigned char *data = (const unsigned char *)iphdr;

  for (int i = 0; i < 20; i++) {
    sprintf(dest, "%02x", data[i]);
    dest += 2;

    if ((i + 1) % 2 == 0) {
      *dest++ = ' ';
    }
  }

  *dest = '\0';
  return dest_copy;
}
