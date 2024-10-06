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

void fill_data(char payload[PACKET_DATA_SIZE]) {
  unsigned long int i;
  const char data[] = "fortytwo! ";
  const int data_len = strlen(data);

  for (i = 0; i < PACKET_DATA_SIZE; i++) {
    payload[i] = data[i % data_len];
  }
  payload[i - 1] = '\0';
}

void ft_ping(__attribute__((unused)) int dummy) {
  t_icmp_packet packet;

  memset(&packet, 0, sizeof(packet));
  if (gettimeofday(&packet.payload.timeval, NULL) == -1) {
    fprintf(stderr, "%s: %s\n", ping.program_name, strerror(errno));
  }
  fill_data(packet.payload.data);
  printf("COUCOU\n");
  alarm(1);
}
