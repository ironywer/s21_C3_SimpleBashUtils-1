#include "stdio.h"
#include "string.h"
#define ERROR -1
#define OK 0
#define N_FLAGS_CAT 6

int output_of_the_entire_file(char *name_of_file);
int options(int argc, char **argv, char *op_flag, int *count_flags,
            int *count_arg_flags);
int output_with_options(int argc, char **argv);
char sym_output_with_options(FILE *fp, char sym, char *op_flags, int *num_row,
                             char pred_sym, int *eline_printed);

int main(int argc, char *argv[]) {
  FILE *fp;
  int flag = OK;
  // if (argc == 2) {
  //   flag = output_of_the_entire_file(argv[1]);
  if (argc > 1) {
    flag = output_with_options(argc, argv);
  } else
    flag = ERROR;
  return flag;
}

int output_of_the_entire_file(char *name_of_file) {
  int flag = OK;
  FILE *fp;
  char sym;
  if ((fp = fopen(name_of_file, "r")) == NULL) {
    printf("Не удалось открыть файл");
    flag = ERROR;
  } else {
    while ((sym = fgetc(fp)) != EOF) {
      putchar(sym);
    }
  }
  fclose(fp);
  return flag;
}

int options(int argc, char **argv, char *op_flag, int *count_flags,
            int *count_arg_flags) {
  int flag = OK;
  char flags_const[N_FLAGS_CAT] = {"benstv"};
  for (int i = 1; i < argc - 1 && flag == OK; i++) {
    if (argv[i][0] != '-') {
      i = argc - 1;
    } else {
      *count_arg_flags += 1;
      for (int j = 1; j < strlen(argv[i]) && flag == OK; j++) {
        char buf_sym = argv[i][j];
        if (*count_flags <= N_FLAGS_CAT - 1 &&
            strchr(flags_const, buf_sym) != NULL &&
            strchr(op_flag, buf_sym) == NULL) {
          op_flag[*count_flags] = buf_sym;
          *count_flags += 1;
        } else if (strchr(flags_const, buf_sym) == NULL) {
          flag = ERROR;
          printf("2E - %d\n", flag);
        }
      }
    }
  }
  return flag;
}

int output_with_options(int argc, char **argv) {
  int flag = OK, count_flags = 0, count_arg_flags = 0;
  char op_flags[N_FLAGS_CAT + 1] = {'\0'};
  flag = options(argc, argv, op_flags, &count_flags, &count_arg_flags);
  for (int i = count_arg_flags + 1; i < argc && flag == OK; i++) {
    FILE *fp;
    if ((fp = fopen(argv[i], "r")) == NULL) {
      printf("cat: %s: No such file or directory\n", argv[i]);
      flag = ERROR;
    } else {
      char sym;
      int num_row = 0, num_sym = 0, flag_enter = 1, eline_printed = 0;
      char pred_sym = '\0';
      while ((sym = fgetc(fp)) != EOF) {
        num_sym += 1;
        pred_sym = sym_output_with_options(fp, sym, op_flags, &num_row,
                                           pred_sym, &eline_printed);
      }
    }
    if (fp != NULL)
      fclose(fp);
  }
  return flag;
}

char sym_output_with_options(FILE *fp, char sym, char *op_flags, int *num_row,
                             char pred_sym, int *eline_printed) {
  if (!(strchr(op_flags, 's') != NULL && pred_sym == '\n' && sym == '\n' &&
        *eline_printed)) {

    if (pred_sym == '\n' && sym == '\n')
      *eline_printed = 1;
    else
      *eline_printed = 0;
    if (((strchr(op_flags, 'n') != NULL && strchr(op_flags, 'b') == NULL) ||
         (strchr(op_flags, 'b') != NULL && sym != '\n')) &&
        pred_sym == '\n') {
      *num_row += 1;
      printf("%6d\t", *num_row);
    }

    if (strchr(op_flags, 'e') != NULL && sym == '\n')
      printf("$");
    if (strchr(op_flags, 't') != NULL && sym == '\t') {
      printf("^");
      sym = '\t' + 64;
    }
    if (strchr(op_flags, 'v') != NULL && sym >= 0 && sym <= 31 && sym != '\n' &&
        sym != '\t') {
      printf("^");
      sym = sym + 64;
    }
    putchar(sym);
  }
  return sym;
}