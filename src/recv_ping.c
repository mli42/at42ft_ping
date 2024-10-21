#include "recv_ping.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int is_valid_checksum(const t_icmp_packet *const packet) {
  t_icmp_packet packet_copy;

  memcpy(&packet_copy, packet, sizeof(packet_copy));
  packet_copy.icmphdr.checksum = 0;
  return packet->icmphdr.checksum == checksum(&packet_copy, sizeof(packet_copy));
}

void init_payload(
  struct msghdr *msghdr,
  struct cmsghdr *cmsg,
  struct iovec *iov,
  unsigned char reply[REPLY_PACKET_SIZE]
) {
  memset(msghdr, 0, sizeof(*msghdr));
  memset(cmsg, 0, sizeof(*cmsg));
  memset(iov, 0, sizeof(*iov));
  memset(reply, 0, REPLY_PACKET_SIZE);

  iov->iov_base = reply;
  iov->iov_len = REPLY_PACKET_SIZE;

  msghdr->msg_name = &ping.sockaddr;
  msghdr->msg_namelen = sizeof(ping.sockaddr);

  msghdr->msg_iov = iov;
  msghdr->msg_iovlen = 1;

  msghdr->msg_control = cmsg;
  msghdr->msg_controllen = sizeof(*cmsg);
}

void recv_ping(const t_ping *const ping) {
  struct msghdr msghdr;
  struct cmsghdr cmsg;
  struct iovec iov;
  unsigned char reply[REPLY_PACKET_SIZE];
  const t_icmp_packet *const packet = (t_icmp_packet *)&reply[sizeof(struct iphdr)];

  init_payload(&msghdr, &cmsg, &iov, reply);

  if (recvmsg(ping->sock_fd, &msghdr, 0) < 0) {
    fprintf(stderr, "%s: recvmsg: %s\n", ping->program_name, strerror(errno));
    return ;
  }

  if (!is_valid_checksum(packet) || packet->icmphdr.type != ICMP_ECHOREPLY) {
    fprintf(stderr, "%s: Unexpected packet\n", ping->program_name);
    return ;
  }
}
