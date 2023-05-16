#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Data {
  char *pattern;
  char *files_f;
  char *str_file;
};

struct Flags {
  int flag_e;
  int flag_i;
  int flag_v;
  int flag_c;
  int flag_l;
  int flag_n;
  int flag_h;
  int flag_s;
  int flag_f;
  int flag_o;
};

void implementation(char *argv[], int count_files, struct Flags *p,
                    int *count_patterns, int *count_files_f,
                    struct Data patterns[], int optind, int *count_free_f,
                    int *flag_free_f);
void parsing_flag(int argc, char *argv[], struct Flags *p,
                  struct Data patterns[], int *count, int *count_f);
int match(char *string, char *pattern, regex_t *re, struct Flags *p);
void remove_end_str(char *str);
void print_many_files(int count_files, char *argv[], struct Flags *p);
