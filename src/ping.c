#include "ft_ping.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void signal_handler(int signum) {
  switch (signum) {
  case SIGQUIT:
    printf("Quit\n");
    close(ping.sock_fd);
    exit(131);
  case SIGINT:
    printf("--- %s ping statistics ---\n", ping.hostname);
    close(ping.sock_fd);
    exit(0);
  default:
    return ;
  }
}

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

void fill_payload(t_icmp_packet_payload *payload) {
  unsigned long int i;
  const char data[] = "fortytwo! ";
  const int data_len = strlen(data);

  if (gettimeofday(&payload->timeval, NULL) == -1) {
    fprintf(stderr, "%s: %s\n", ping.program_name, strerror(errno));
  }
  for (i = 0; i < PACKET_DATA_SIZE; i++) {
    payload->data[i] = data[i % data_len];
  }
  payload->data[i - 1] = '\0';
}

void fill_icmp_packet(t_icmp_packet *packet) {
  fill_payload(&packet->payload);
  packet->icmphdr.type = ICMP_ECHO;
  packet->icmphdr.un.echo.id = getpid();
  packet->icmphdr.un.echo.sequence = ++ping.stats.sent;
  packet->icmphdr.checksum = checksum(packet, sizeof(*packet));
}

void ft_ping(__attribute__((unused)) int dummy) {
  t_icmp_packet packet;

  memset(&packet, 0, sizeof(packet));
  fill_icmp_packet(&packet);
  if (sendto(ping.sock_fd, &packet, sizeof(packet), 0, (sockaddr_t *)&ping.sockaddr, sizeof(ping.sockaddr)) == -1) {
    fprintf(stderr, "%s: sendto: %s\n", ping.program_name, strerror(errno));
  }
  printf("COUCOU %lu\n", ping.stats.sent);
  alarm(1);
}
