#include "s21_cat.h"

int main(int argc, char *argv[]) {
  struct Flags cat_flags = {0, 0, 0, 0, 0, 0};
  struct Flags *ptr = &cat_flags;

  parsing_flag(argc, argv, ptr);

  while (optind < argc) {
    implementation(argv, ptr);
    optind++;
  }
}

void parsing_flag(int argc, char *argv[], struct Flags *p) {
  struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                  {"number", 0, NULL, 'n'},
                                  {"squeeze-blank", 0, NULL, 's'},
                                  {0, 0, 0, 0}};

  char c;

  while ((c = getopt_long(argc, argv, "+bevnstET", long_options, 0)) != -1) {
    switch (c) {
      case 'b':
        p->flag_b = 1;
        break;

      case 'e':
        p->flag_e = 1;
        p->flag_v = 1;
        break;

      case 'n':
        p->flag_n = 1;
        break;

      case 's':
        p->flag_s = 1;
        break;

      case 't':
        p->flag_t = 1;
        p->flag_v = 1;
        break;

      case 'E':
        p->flag_e = 1;
        break;

      case 'T':
        p->flag_t = 1;
        break;

      case 'v':
        p->flag_v = 1;
        break;

      default:
        break;
    }
  }

  if (p->flag_b && p->flag_n) p->flag_n = 0;
}

void implementation(char *argv[], struct Flags *p) {
  int ch;
  int count_s = 0;
  int count_str_not_blank = 1;
  int count_str_all = 1;
  char last_symbol = '\n';
  FILE *file = fopen(argv[optind], "r");
  if (file) {
    while ((ch = fgetc(file)) != EOF) {
      if (p->flag_s) {
        if (last_symbol == '\n' && ch == '\n') {
          count_s++;
          if (count_s > 1) {
            continue;
          }
        } else {
          count_s = 0;
        }
      }

      if (p->flag_b) {
        if (last_symbol == '\n' && ch != '\n') {
          printf("%6d\t", count_str_not_blank++);
        }
      }

      if (p->flag_n) {
        if (last_symbol == '\n') {
          printf("%6d\t", count_str_all++);
        }
      }

      if (p->flag_v) {
        if (ch < 32 && ch != '\n' && ch != '\t') {
          printf("^");
          ch += 64;
        } else if (ch == 127) {
          printf("^");
          ch -= 64;
        } else if (ch >= 128 && ch <= 159) {
          printf("M-^");
          ch -= 64;
        } else if (ch > 159 && ch < 255) {
          printf("M-");
          ch -= 128;
        }
        if (ch == 255) {
          printf("M-^");
          ch -= 192;
        }
      }

      if (p->flag_t) {
        if (ch == 0x09) {
          printf("^");
          ch += 64;
        }
      }

      if (p->flag_e) {
        if (ch == '\n') {
          printf("$");
        }
      }

      printf("%c", ch);
      last_symbol = ch;
    }
  }
}
