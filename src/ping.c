#include "ft_ping.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

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

void ft_ping(int dummy) {
  (void)dummy;
  alarm(1);
  printf("COUCOU\n");
}
