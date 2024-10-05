#include "ft_ping.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void signal_handler(int dummy) {
  (void)dummy;
  exit(0);
}

void ft_ping(int dummy) {
  (void)dummy;
  alarm(1);
  printf("COUCOU\n");
}
