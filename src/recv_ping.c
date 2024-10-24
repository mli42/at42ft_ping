#include "ft_ping.h"
#include "socket.h"
#include "utils/icmp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void handle_unexpected_packet(
  const t_ping *const ping,
  const sockaddr_in_t *const addr,
  const t_icmp_packet *const packet,
  int readlen
) {
  char hostname[INET_ADDRSTRLEN] = { 0 };
  char iphdr_dump[IPHDR_DUMP_LEN] = { 0 };
  const char *const error_message = get_error_message(packet);
  const struct iphdr *const sent_iphdr = (struct iphdr *)&packet->payload;
  const t_icmp_packet *const sent_packet = (t_icmp_packet *)((unsigned char *)&packet->payload + sizeof(struct iphdr));

  ipv4_to_string(&addr->sin_addr, hostname);

  printf("%d bytes from %s: %s\n", readlen, hostname, error_message);

  if (ping->flags.verbose) {
    printf("IP Hdr Dump:\n %s\n", get_iphdr_dump(sent_iphdr, iphdr_dump));

    printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src\tDst\tData\n" \
      "%2x %2x  %02x %04x %4x %3x %04x  %02x  %02x %4x %d.%d.%d.%d  %d.%d.%d.%d\n",
      sent_iphdr->version,
      sent_iphdr->ihl,
      sent_iphdr->tos,
      *((uint8_t *)&sent_iphdr->tot_len + 1),
      sent_iphdr->id,
      ((uint8_t)sent_iphdr->frag_off & 0b11100000) >> 5,
      *((uint8_t *)&sent_iphdr->frag_off + 1),
      sent_iphdr->ttl,
      sent_iphdr->protocol,
      sent_iphdr->check,
      *((uint8_t *)&sent_iphdr->saddr + 0),
      *((uint8_t *)&sent_iphdr->saddr + 1),
      *((uint8_t *)&sent_iphdr->saddr + 2),
      *((uint8_t *)&sent_iphdr->saddr + 3),
      *((uint8_t *)&sent_iphdr->daddr + 0),
      *((uint8_t *)&sent_iphdr->daddr + 1),
      *((uint8_t *)&sent_iphdr->daddr + 2),
      *((uint8_t *)&sent_iphdr->daddr + 3)
    );

    printf("ICMP: type %d, code %d, size %d, id 0x%x, seq 0x%04x\n",
      sent_packet->icmphdr.type,
      sent_packet->icmphdr.code,
      PACKET_SIZE,
      sent_packet->icmphdr.un.echo.id,
      sent_packet->icmphdr.un.echo.sequence
    );
  }
}

// https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
void update_welford_stats(t_ping *ping, float time_ms) {
  float delta1, delta2;

  ping->stats.received++;

  if (time_ms < ping->stats.min_rtt || ping->stats.min_rtt == 0.)
    ping->stats.min_rtt = time_ms;
  if (time_ms > ping->stats.max_rtt)
    ping->stats.max_rtt = time_ms;

  delta1 = time_ms - ping->stats.mean;
  ping->stats.mean += delta1 / ping->stats.received;
  delta2 = time_ms - ping->stats.mean;
  ping->stats.m2 += delta1 * delta2;
}

void handle_echo_reply_packet(
  t_ping *const ping,
  const struct iphdr *const iphdr,
  const t_icmp_packet *const packet,
  int readlen
) {
  struct timeval received_time;
  struct timeval time_diff;
  float time_ms;

  if (gettimeofday(&received_time, NULL) == -1) {
    fprintf(stderr, "%s: %s\n", ping->program_name, strerror(errno));
  }
  timersub(&received_time, (struct timeval *)&packet->payload, &time_diff);
  time_ms = time_diff.tv_sec * 1000. + time_diff.tv_usec / 1000.;
  update_welford_stats(ping, time_ms);

  printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
    readlen,
    ping->ipaddr,
    packet->icmphdr.un.echo.sequence,
    iphdr->ttl,
    time_ms
  );
}

void init_payload(
  struct msghdr *msghdr,
  struct cmsghdr *cmsg,
  struct iovec *iov,
  sockaddr_in_t *addr,
  unsigned char reply[READ_REPLY_SIZE]
) {
  memset(msghdr, 0, sizeof(*msghdr));
  memset(cmsg, 0, sizeof(*cmsg));
  memset(iov, 0, sizeof(*iov));
  memset(reply, 0, READ_REPLY_SIZE);

  iov->iov_base = reply;
  iov->iov_len = READ_REPLY_SIZE;

  msghdr->msg_name = addr;
  msghdr->msg_namelen = sizeof(*addr);

  msghdr->msg_iov = iov;
  msghdr->msg_iovlen = 1;

  msghdr->msg_control = cmsg;
  msghdr->msg_controllen = sizeof(*cmsg);
}

void recv_ping(t_ping *const ping) {
  int readlen;
  struct msghdr msghdr;
  struct cmsghdr cmsg;
  struct iovec iov;
  sockaddr_in_t addr;
  unsigned char reply[READ_REPLY_SIZE];
  const struct iphdr *const iphdr = (struct iphdr *)reply;
  t_icmp_packet *const packet = (t_icmp_packet *)&reply[sizeof(struct iphdr)];

  memcpy(&addr, &ping->sockaddr, sizeof(addr));
  init_payload(&msghdr, &cmsg, &iov, &addr, reply);

  if ((readlen = recvmsg(ping->sock_fd, &msghdr, 0)) < 0) {
    fprintf(stderr, "%s: recvmsg: %s\n", ping->program_name, strerror(errno));
    return ;
  }

  readlen -= sizeof(struct iphdr);
  if (!is_valid_checksum(packet, readlen)) {
    handle_unexpected_packet(ping, &addr, packet, readlen);
    return ;
  }

  switch (packet->icmphdr.type) {
  case ICMP_ECHO:
    break;
  case ICMP_ECHOREPLY:
    handle_echo_reply_packet(ping, iphdr, packet, readlen);
    break;
  default:
    handle_unexpected_packet(ping, &addr, packet, readlen);
    break;
  }
}
