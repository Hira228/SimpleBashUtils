#include "s21_grep.h"

int main(int argc, char *argv[]) {
  int count_patterns = 0;
  int count_files_f = 0;
  struct Data patterns[2048];
  struct Flags grep_flags = {0};
  struct Flags *ptr = &grep_flags;
  int count_free_f = 0;
  int flag_free_f = 0;
  parsing_flag(argc, argv, ptr, patterns, &count_patterns, &count_files_f);
  int count_files = argc - optind;
  if (!ptr->flag_c && !ptr->flag_e && !ptr->flag_f && !ptr->flag_h &&
      !ptr->flag_i && !ptr->flag_l && !ptr->flag_n && !ptr->flag_o &&
      !ptr->flag_s && !ptr->flag_v) {
    ptr->flag_e = 1;
    patterns[0].pattern = argv[optind];
    count_patterns++;
    optind++;
    count_files--;
  }
  if (!ptr->flag_e && !ptr->flag_f) {
    patterns[0].pattern = argv[optind];
    count_patterns++;
    optind++;
    count_files--;
  }
  while (optind < argc) {
    implementation(argv, count_files, ptr, &count_patterns, &count_files_f,
                   patterns, optind, &count_free_f, &flag_free_f);
    optind++;
  }
  if (ptr->flag_f) {
    for (int i = count_free_f; i < count_patterns; i++) {
      free(patterns[i].pattern);
    }
  }

  return 0;
}

void parsing_flag(int argc, char *argv[], struct Flags *p,
                  struct Data patterns[], int *count, int *count_f) {
  char c;
  while ((c = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) != -1) {
    switch (c) {
      case 'e':
        p->flag_e = 1;
        patterns[*count].pattern = optarg;
        (*count)++;
        break;

      case 'f':
        p->flag_f = 1;
        patterns[*count_f].files_f = optarg;
        (*count_f)++;
        break;

      case 'i':
        p->flag_i = 1;
        break;

      case 'v':
        p->flag_v = 1;
        break;

      case 'c':
        p->flag_c = 1;
        break;

      case 'l':
        p->flag_l = 1;
        break;

      case 'n':
        p->flag_n = 1;
        break;

      case 'h':
        p->flag_h = 1;
        break;

      case 's':
        p->flag_s = 1;
        break;

      case 'o':
        p->flag_o = 1;
        break;

      default:
        break;
    }
  }

  if (p->flag_c) {
    p->flag_n = 0;
  }
  if (p->flag_l) {
    p->flag_n = 0;
  }
  if (p->flag_v) {
    p->flag_o = 0;
  }
}

void remove_end_str(char *str) {
  if (str[strlen(str) - 1] == '\n') {
    str[strlen(str) - 1] = '\0';
  }
}

void print_many_files(int count_files, char *argv[], struct Flags *p) {
  if (count_files > 1 && !p->flag_h) {
    printf("%s:", argv[optind]);
  }
}

void implementation(char *argv[], int count_files, struct Flags *p,
                    int *count_patterns, int *count_files_f,
                    struct Data patterns[], int optind, int *count_free_f,
                    int *flag_free_f) {
  int count_str_n = 1;
  int count_str_c = 0;
  regex_t re;
  char *str = NULL;
  char *str_f = NULL;
  size_t len = 0;
  size_t len_f = 0;
  ssize_t read = 0;
  ssize_t read_f = 0;
  if (p->flag_f) {
    for (int i = 0; i < *count_files_f; i++) {
      FILE *file_f = fopen(patterns[i].files_f, "r");
      if (file_f) {
        if (*flag_free_f == 0) {
          *count_free_f = *count_patterns;
          *flag_free_f = 1;
        }
        while ((read_f = getline(&str_f, &len_f, file_f)) != -1) {
          remove_end_str(str_f);
          patterns[*count_patterns].pattern = malloc(len_f * sizeof(char));
          for (size_t j = 0; j < len_f; j++) {
            patterns[*count_patterns].pattern[j] = str_f[j];
          }
          (*count_patterns)++;
          free(str_f);
          str_f = NULL;
        }
        fclose(file_f);
      } else if (!p->flag_s) {
        fprintf(stderr, "grep: %s: No such file or directory\n",
                patterns[i].files_f);
      }
    }
    p->flag_e = 1;
  }
  int flag_c = 0;
  int flag_v = 0;
  int count_str_file = 0;

  FILE *file = fopen(argv[optind], "r");
  if (file) {
    while ((read = getline(&str, &len, file)) != -1) {
      patterns[count_str_file].str_file = malloc(len * sizeof(char));
      remove_end_str(str);
      for (size_t m = 0; m < len; m++) {
        patterns[count_str_file].str_file[m] = str[m];
      }
      free(str);
      str = NULL;
      flag_c = 0;
      for (int i = 0; i < *count_patterns; i++) {
        if ((match(patterns[count_str_file].str_file, patterns[i].pattern, &re,
                   p) == (p->flag_v ? 1 : 0))) {
          if (*count_patterns > 1 && p->flag_v) {
            for (int j = 0; j < *count_patterns; j++) {
              if (match(patterns[count_str_file].str_file, patterns[j].pattern,
                        &re, p) == 1) {
                flag_v = 1;
              } else {
                flag_v = 0;
                break;
              }
            }
            if (flag_v == 0) {
              break;
            }
          }
          if (!p->flag_l && !p->flag_c && !p->flag_n)
            print_many_files(count_files, argv, p);
          flag_c = 1;
          if (p->flag_n) {
            print_many_files(count_files, argv, p);
            printf("%d:", count_str_n);
            p->flag_e = 1;
          }
          if ((p->flag_i || p->flag_e || p->flag_v || p->flag_o || p->flag_h) &&
              !p->flag_c && !p->flag_l) {
            if (p->flag_o) {
              regmatch_t pmatch = {0};
              char *temp = patterns[count_str_file].str_file;
              regcomp(&re, patterns[i].pattern,
                      p->flag_i ? REG_EXTENDED | REG_ICASE : REG_EXTENDED);
              while (regexec(&re, temp, 1, &pmatch, 0) != REG_NOMATCH) {
                for (int i = pmatch.rm_so; i < pmatch.rm_eo; i++) {
                  printf("%c", temp[i]);
                }
                printf("\n");
                temp += pmatch.rm_eo;
              }
              regfree(&re);
            } else {
              printf("%s\n", patterns[count_str_file].str_file);
            }
            break;
          }
        }
      }
      if (flag_c == 1) count_str_c++;
      count_str_n++;
      count_str_file++;
    }
    if (p->flag_c) {
      if (count_files > 1 && !p->flag_h) {
        printf("%s:", argv[optind]);
      }
      (!p->flag_l) ? printf("%d\n", count_str_c)
                   : printf("%d\n", count_str_c > 0 ? 1 : 0);
    }
    if (p->flag_l && count_str_c > 0) {
      printf("%s\n", argv[optind]);
    }
    fclose(file);
  } else if (!p->flag_s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
  }
  for (int i = 0; i < count_str_file; i++) {
    free(patterns[i].str_file);
  }
  if (str != NULL) free(str);
  if (str_f != NULL) free(str_f);
}

int match(char *string, char *pattern, regex_t *re, struct Flags *p) {
  int status;
  status =
      regcomp(re, pattern, p->flag_i ? REG_EXTENDED | REG_ICASE : REG_EXTENDED);
  status = regexec(re, string, 0, NULL, 0);
  regfree(re);
  return (status);
}
