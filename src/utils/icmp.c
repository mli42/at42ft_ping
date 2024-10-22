#include "icmp.h"
#include <string.h>

int is_valid_checksum(const t_icmp_packet *const packet) {
  t_icmp_packet packet_copy;

  memcpy(&packet_copy, packet, sizeof(packet_copy));
  packet_copy.icmphdr.checksum = 0;
  return packet->icmphdr.checksum == checksum(&packet_copy, sizeof(packet_copy));
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
