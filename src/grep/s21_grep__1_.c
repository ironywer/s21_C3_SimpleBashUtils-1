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
  // printf("%d - проверка\n", flag);
  if (flag == OK) {
    // printf("9");
    flag = read_and_print(argc, argv, regulars, number_regulars, op_flags,
                          count_flags, count_arg_flags);
  }
  // printf("%d - конечная проверка\n", flag);
  return flag;
}

int read_and_print(int argc, char **argv, char regulars[100][100],
                   int number_regulars, char *op_flags, int count_flags,
                   int count_arg_flags) {
  int flag = OK, k = 0;
  FILE *fp = NULL;
  int num_sym = 0, flag_enter = 1, eline_printed = 0;
  regex_t regex[1000];
  // printf("0");
  int regex_flags = 0;
  if (strchr(op_flags, 'i')) {
    regex_flags |=
        REG_ICASE; // Устанавливаем REG_ICASE, если флаг -i установлен
  }

  for (int i = 0; i < number_regulars && flag == OK; i++) {
    if (regcomp(&regex[i], regulars[i], regex_flags) != 0) {
      flag = ERROR;
    }
  }
  // printf("1");
  if (argc - 1 != number_regulars + count_flags) {
    k = number_regulars + count_flags + 1;
    fp = fopen(argv[k], "r");
    k += 1;
  }
  // printf("2");
  int end_while = 0;
  int match_count = 0;
  int line_number = 0; // Добавьте эту переменную перед циклом
  int schet = 0;
  while (end_while != -1 && flag == OK) {
    char row[1000] = {'\0'};
    int skip = 0;
    schet += 1;
    line_number++; // Увеличиваем номер строки
    if (argc - 1 == number_regulars + count_flags) {
      if (fgets(row, 1000, stdin) == NULL)
        end_while = -1;
    } else {
      if (fp != NULL) {
        if (fgets(row, 1000, fp) == NULL) {
          fclose(fp);
          skip = 1;
          fp = fopen(argv[k], "r");
          k += 1;
          if (count_flags && strchr(op_flags, 'c')) {
            printf("%s: %d\n", argv[k - 2], match_count);
            match_count = 0;
          } else if (count_flags && strchr(op_flags, 'l')) {
            printf("%s\n", argv[k - 2]);
            match_count = 0;
          } else if ((fp != NULL || ( k != argc + 1)) && schet == 1)
            printf("\n");
          line_number = 0;
        }

      } else if (argv[k - 1][0] != '\0') {
        if (strchr(op_flags, 's') == NULL)
          printf("%s: No such file or directory\n", argv[k - 1]);
        skip = 1;
        fp = fopen(argv[k], "r");
        k += 1;
        line_number = 0;
      } else
        end_while = -1;
    }

    // Проверяем совпадение регулярного выражения с текущей строкой
    for (int i = 0;
         i < number_regulars && flag == OK && end_while != -1 && skip == 0;
         i++) {
      int res = regexec(&regex[i], row, 0, NULL, 0);
      char *v = strchr(op_flags, 'v');
      if (res == 0 && v == NULL || (res != 0 && v != NULL)) {
        match_count++;
        schet = 0;
        if (argc - 2 > number_regulars + count_flags &&
            strchr(op_flags, 'c') == NULL && strchr(op_flags, 'l') == NULL &&
            strchr(op_flags, 'h') == NULL)
          printf("%s:", argv[k - 1]);
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
    if (k == argc + 1)
      end_while = -1;
  }

  // Доделать реализацию считывания из файла/stdin и вывода в зависимости от
  // флагов ^
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
        *strchr(op_flags,'\0') = sym;
      }
      if (sym == 'e')
        *flag_e = 0;
    } else
      flag = ERROR;
  }
  return flag;
}