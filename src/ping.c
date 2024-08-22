#include "ft_ping.h"
#include <unistd.h>
#include <signal.h>

short int PING_LOOP = 1;

const int PING_WAIT = 1000000; // 1 second

void signal_handler(int dummy) {
  (void)dummy;
  PING_LOOP = 0;
}

void ft_ping(t_ping *ping) {
  signal(SIGINT, signal_handler);

  while (PING_LOOP) {
    usleep(PING_WAIT);
  }
}
