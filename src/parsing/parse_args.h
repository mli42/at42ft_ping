#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

typedef struct flags {
  unsigned short int  verbose:1;
  unsigned short int  help:1;
  unsigned long int   ttl;
} t_flags;

int parse_args(int argc, char *argv[], t_flags *flags);

#endif
