#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define ERROR -1
#define N_FLAGS_GREP 9
int options_and_regulars(int argc, char **argv, char regulars[100][100],
                         int *number_regulars, char *op_flags, int *count_flags,
                         int *count_arg_flags);

int arg_flags(char *argum, char *op_flags, int *count_flags, int *flag_e);

int read_and_print(int argc, char **argv, char regulars[100][100],
                   int number_regulars, char *op_flags, int count_flags,
                   int count_arg_flags);
int compile_regex(regex_t *regex, char regulars[100][100], int number_regulars,
                  char *op_flags);

FILE *open_file(int argc, char *argv[], int k, int number_regulars,
                int count_flags);

int one_file(int argc, char *argv, char regulars[100][100], int number_regulars,
             char *op_flags, int count_flags, int count_arg_flags,
             regex_t *regex, FILE *fp);

void print_file(regex_t *regex, int number_regulars, int count_flags,
                char *op_flags, int *match_count, int end_while, char row[1000],
                int *schet, int argc, int line_number, char *argv);

int main(int argc, char *argv[]) {
  int flag = OK;
  char op_flags[N_FLAGS_GREP] = {'\0'};
  int count_flags = 0, count_arg_flags = 0;
  char regulars[100][100] = {'\0'};
  int number_regulars = 0;
  if (argc > 1) {
    flag = options_and_regulars(argc, argv, regulars, &number_regulars,
                                op_flags, &count_flags, &count_arg_flags);
  } else
    flag = ERROR;
  if (flag == OK) {
    flag = read_and_print(argc, argv, regulars, number_regulars, op_flags,
                          count_flags, count_arg_flags);
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
    if (regcomp(&regex[i], regulars[i], regex_flags) != 0) {
      flag = ERROR;
      i = number_regulars;
    }
  }

  return OK;
}

void print_file(regex_t *regex, int number_regulars, int count_flags,
                char *op_flags, int *match_count, int end_while, char row[1000],
                int *schet, int argc, int line_number, char *argv) {
  int flag = OK;
  for (int i = 0; i < number_regulars && flag == OK && end_while != -1; i++) {
    int res = regexec(&regex[i], row, 0, NULL, 0);
    char *v = strchr(op_flags, 'v');
    if (res == 0 && v == NULL || (res != 0 && v != NULL)) {
      *match_count += 1;
      *schet = 0;
      if (argc - 2 > number_regulars + count_flags &&
          strchr(op_flags, 'c') == NULL && strchr(op_flags, 'l') == NULL &&
          strchr(op_flags, 'h') == NULL)
        printf("%s:", argv);
      if (count_flags == 0 ||
          strchr(op_flags, 'c') == NULL && strchr(op_flags, 'l') == NULL) {
        if (strchr(op_flags, 'n')) {
          printf("%d:", line_number); // Выводим номер строки
        }
        printf("%s", row);
      }
      i = number_regulars;
    }
  }
}

int one_file(int argc, char *argv, char regulars[100][100], int number_regulars,
             char *op_flags, int count_flags, int count_arg_flags,
             regex_t *regex, FILE *fp) {
  int flag = OK, end_while = 0, match_count = 0, line_number = 0, schet = 0;
  while (end_while != -1 && flag == OK) {
    char row[1000] = {'\0'};
    schet += 1;
    line_number++;
    if (fgets(row, 1000, fp) == NULL) {
      fclose(fp);
      end_while = -1;
      if (count_flags && strchr(op_flags, 'c')) {
        printf("%s: %d\n", argv, match_count);
      } else if (count_flags && strchr(op_flags, 'l')) {
        printf("%s\n", argv);
      } else if ((fp != NULL) && schet == 1)
        printf("\n");
      line_number = 0;
    }
    print_file(regex, number_regulars, count_flags, op_flags, &match_count,
               end_while, row, &schet, argc, line_number, argv);
  }
  return flag;
}

int read_and_print(int argc, char **argv, char regulars[100][100],
                   int number_regulars, char *op_flags, int count_flags,
                   int count_arg_flags) {
  int flag = OK;
  FILE *fp;
  int num_sym = 0, flag_enter = 1, eline_printed = 0;
  regex_t regex[1000];
  int regex_flags = 0;
  int f = 1;
  flag = compile_regex(regex, regulars, number_regulars, op_flags);
  if (number_regulars + count_flags + 1 == argc)
    f = 0;
  for (int k = number_regulars + count_flags + f; flag == OK && k < argc; k++) {
    fp = open_file(argc, argv, k, number_regulars, count_flags);
    if (fp != NULL) {
      flag = one_file(argc, argv[k], regulars, number_regulars, op_flags,
                      count_flags, count_arg_flags, regex, fp);
    } else if (argv[k][0] != '\0' && strchr(op_flags, 's') == NULL)
      printf("%s: No such file or directory\n", argv[k]);
  }

  for (int i = 0; i < number_regulars && flag == OK; i++) {
    regfree(&regex[i]);
  }
  return flag;
}

int options_and_regulars(int argc, char **argv, char regulars[100][100],
                         int *number_regulars, char *op_flags, int *count_flags,
                         int *count_arg_flags) {
  int flag = OK;
  int flag_e = 0;
  char flags_const[N_FLAGS_GREP] = {"eisfvhcln"};
  for (int i = 1; i < argc && flag == OK; i++) {
    char *argum = argv[i];
    if (argum[0] == '-') {
      flag = arg_flags(argum, op_flags, count_flags, &flag_e);
      *count_flags += 1;
    } else if (flag_e == 0) {
      strcpy(regulars[*number_regulars], argv[i]);
      *number_regulars += 1;
      flag_e = -1;
    } else if (flag_e == -1) {
      i = argc;
    } else
      flag = ERROR;
  }
  return flag;
}

int arg_flags(char *argum, char *op_flags, int *count_flags, int *flag_e) {
  int flag = OK;
  char flags_const[N_FLAGS_GREP] = {"eivfshcln"};
  for (int i = 1; i < strlen(argum) && flag == OK; i++) {
    char sym = argum[i];
    if (strchr(flags_const, sym) != NULL) {
      if (strchr(op_flags, sym) == NULL) {
        *strchr(op_flags, '\0') = sym;
      }
      if (sym == 'e')
        *flag_e = 0;
    } else {
      printf("%s: Illegal option -- %c\n", argum, sym);
      flag = ERROR;
    }
  }
  return flag;
}