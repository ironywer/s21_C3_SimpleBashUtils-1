#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define ERROR -1
#define N_FLAGS_GREP 10
int options_and_regulars(int argc, char **argv, char regulars[100][100],
                         int *number_regulars, char *op_flags,
                         int *count_flags);

int arg_flags(char *argum, char *op_flags, int *flag_e, int *flag_f);

int read_and_print(int argc, char **argv, char regulars[100][100],
                   int number_regulars, char *op_flags, int count_flags);
int compile_regex(regex_t *regex, char regulars[100][100], int number_regulars,
                  char *op_flags);

FILE *open_file(int argc, char *argv[], int k, int number_regulars,
                int count_flags);

int one_file(int argc, char *argv, int number_regulars, char *op_flags,
             int count_flags, regex_t *regex, FILE *fp);

void print_file(regex_t *regex, int number_regulars, int count_flags,
                char *op_flags, int *match_count, int end_while, char row[1000],
                int *schet, int argc, int line_number, char *argv);

void outputMatches(int *match_count, int number_regulars, int *schet,
                   int count_flags, char *op_flags, int argc, int *line_number,
                   char *argv, regex_t reg, char *row);

int main(int argc, char *argv[]) {
  int flag = OK;
  char op_flags[N_FLAGS_GREP] = {'\0'};
  int count_flags = 0;
  char regulars[100][100] = {'\0'};
  int number_regulars = 0;
  if (argc > 1) {
    flag = options_and_regulars(argc, argv, regulars, &number_regulars,
                                op_flags, &count_flags);
  } else
    flag = ERROR;
  if (flag == OK) {
    flag = read_and_print(argc, argv, regulars, number_regulars, op_flags,
                          count_flags);
  }
  return flag;
}

FILE *open_file(int argc, char *argv[], int k, int number_regulars,
                int count_flags) {
  FILE *fp = stdin;
  if (argc - 1 != number_regulars + count_flags) {
    fp = fopen(argv[k], "r");
  }
  return fp;
}

int compile_regex(regex_t *regex, char regulars[100][100], int number_regulars,
                  char *op_flags) {
  int regex_flags = 0;
  int flag = OK;
  if (strchr(op_flags, 'i')) {
    regex_flags |= REG_ICASE;
  }

  for (int i = 0; i < number_regulars; i++) {
    // printf("1 %s", regulars[i]);
    if (regcomp(&regex[i], regulars[i], regex_flags) != 0) {
      flag = ERROR;
      i = number_regulars;
    }
  }

  return flag;
}
//
void outputMatches(int *match_count, int number_regulars, int *schet,
                   int count_flags, char *op_flags, int argc, int *line_number,
                   char *argv, regex_t reg, char *row) {
  *match_count += 1;
  *schet = 0;
  if (argc - 2 > number_regulars + count_flags &&
      strchr(op_flags, 'c') == NULL && strchr(op_flags, 'l') == NULL &&
      strchr(op_flags, 'h') == NULL)
    printf("%s:", argv);
  if (count_flags == 0 ||
      (strchr(op_flags, 'c') == NULL && strchr(op_flags, 'l') == NULL)) {
    if (strchr(op_flags, 'n')) {
      printf("%d:", *line_number);  // Выводим номер строки
    }
    if (strchr(op_flags, 'o') == NULL || strchr(op_flags, 'v') != NULL)
      printf("%s", row);
  }
  if (strchr(op_flags, 'o') != NULL && strchr(op_flags, 'l') == NULL &&
      strchr(op_flags, 'c') == NULL && strchr(op_flags, 'v') == NULL) {
    char *curs = row;
    int first_row = 0;
    regmatch_t match;
    while (regexec(&reg, curs, 1, &match, 0) == 0) {
      if (first_row != 0) putchar('\n');
      printf("%.*s", (int)(match.rm_eo - match.rm_so), &curs[match.rm_so]);
      curs += match.rm_eo;
      first_row += 1;
    }
    if (first_row == 1) putchar('\n');
  }
}

void print_file(regex_t *regex, int number_regulars, int count_flags,
                char *op_flags, int *match_count, int end_while, char row[1000],
                int *schet, int argc, int line_number, char *argv) {
  for (int i = 0; i < number_regulars && end_while != -1; i++) {
    int res = regexec(&regex[i], row, 0, NULL, 0);
    char *v = strchr(op_flags, 'v');
    if ((res == 0 && v == NULL) || (res != 0 && v != NULL)) {
      outputMatches(match_count, number_regulars, schet, count_flags, op_flags,
                    argc, &line_number, argv, regex[i], row);
      i = number_regulars;
    }
  }
}

int one_file(int argc, char *argv, int number_regulars, char *op_flags,
             int count_flags, regex_t *regex, FILE *fp) {
  int flag = OK, end_while = 0, match_count = 0, line_number = 0, schet = 0;
  while (end_while != -1 && flag == OK) {
    char row[1000] = {'\0'};
    schet += 1;
    line_number++;
    if (fgets(row, 1000, fp) == NULL) {
      fclose(fp);
      end_while = -1;
      if (count_flags && strchr(op_flags, 'c')) {
        if (argc - 2 > number_regulars + count_flags &&
            strchr(op_flags, 'h') == NULL)
          printf("%s:", argv);
        if (strchr(op_flags, 'l') != NULL) match_count = 1;
        printf("%d\n", match_count);
        if (strchr(op_flags, 'l') != NULL) printf("%s\n", argv);
      } else if (count_flags && strchr(op_flags, 'l')) {
        printf("%s\n", argv);
      } else if (schet == 1 && strchr(op_flags, 'o') == NULL)
        printf("\n");
      line_number = 0;
    }
    print_file(regex, number_regulars, count_flags, op_flags, &match_count,
               end_while, row, &schet, argc, line_number, argv);
  }
  return flag;
}

int read_and_print(int argc, char **argv, char regulars[100][100],
                   int number_regulars, char *op_flags, int count_flags) {
  int flag = OK;
  FILE *fp = NULL;
  regex_t regex[1000];
  int f = 1;
  flag = compile_regex(regex, regulars, number_regulars, op_flags);
  if (number_regulars + count_flags + 1 == argc) f = 0;
  for (int k = number_regulars + count_flags + f; flag == OK && k < argc; k++) {
    fp = open_file(argc, argv, k, number_regulars, count_flags);
    if (fp != NULL) {
      flag = one_file(argc, argv[k], number_regulars, op_flags, count_flags,
                      regex, fp);
    } else if (argv[k][0] != '\0' && strchr(op_flags, 's') == NULL)
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[k]);
  }

  for (int i = 0; i < number_regulars && flag == OK; i++) {
    regfree(&regex[i]);
  }
  return flag;
}

void read_file_regulars(char *filename, char regulars[100][100],
                        int *number_regulars) {
  FILE *f = NULL;
  int flag = OK;
  if ((f = fopen(filename, "r")) == NULL) {
    flag = ERROR;
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
  }
  char *line = NULL;
  size_t size;
  while (flag == OK && getline(&line, &size, f) != -1) {
    strcpy(regulars[*number_regulars], line);
    *number_regulars += 1;
    // printf("%s - %s", filename, regulars[*number_regulars-1]);
  }
  free(line);
  if (f != NULL) fclose(f);
}

int options_and_regulars(int argc, char **argv, char regulars[100][100],
                         int *number_regulars, char *op_flags,
                         int *count_flags) {
  int flag = OK;
  int flag_e = 0, flag_f = 1;
  for (int i = 1; i < argc && flag == OK; i++) {
    char *argum = argv[i];
    if (argum[0] == '-') {
      flag = arg_flags(argum, op_flags, &flag_e, &flag_f);
      *count_flags += 1;
      if (flag_e > 0 && flag_f == 1 && (int)strlen(argv[i]) - 1 > flag_e &&
          (int)strlen(argv[i]) > 0) {
        strcpy(regulars[*number_regulars], argv[i] + flag_e + 1);
        *number_regulars += 1;
        *count_flags -= 1;
        flag_e = -1;
      }
    } else if (flag_f == 0) {
      read_file_regulars(argv[i], regulars, number_regulars);
      flag_f = -1;
      // *number_regulars += 1;
    } else if (flag_e > -1 && flag_f == 1) {
      strcpy(regulars[*number_regulars], argv[i]);
      *number_regulars += 1;
      flag_e = -1;
      // } else if (flag_e == -1 || flag_f == -1) {
    } else {
      i = argc;
    }
    // else
    //   flag = ERROR;
  }
  return flag;
}

int arg_flags(char *argum, char *op_flags, int *flag_e, int *flag_f) {
  int flag = OK;
  char flags_const[N_FLAGS_GREP] = {"eivfsohcln"};
  for (int i = 1; i < (int)strlen(argum) && flag == OK; i++) {
    char sym = argum[i];
    if (strchr(flags_const, sym) != NULL) {
      if (strchr(op_flags, sym) == NULL) {
        *strchr(op_flags, '\0') = sym;
      }
      if (sym == 'f') *flag_f = 0;
      if (sym == 'e') {
        *flag_e = i;
        i = (int)strlen(argum);
      }
    } else {
      fprintf(stderr, "s21_grep: %s: Illegal option -- %c\n", argum, sym);
      flag = ERROR;
    }
  }
  return flag;
}
