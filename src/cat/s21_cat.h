#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

struct Flags {
  int flag_b;
  int flag_e;
  int flag_v;
  int flag_n;
  int flag_s;
  int flag_t;
};

void implementation(char *argv[], struct Flags *p);
void parsing_flag(int argc, char *argv[], struct Flags *p);