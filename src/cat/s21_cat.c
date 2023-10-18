#include "stdio.h"
#include "string.h"
#define ERROR -1
#define OK 0
#define N_FLAGS_CAT 5

int output_of_the_entire_file(char *name_of_file);
int options(int argc, char **argv, char *op_flag, int *count_flags, int *count_arg_flags);
int output_with_options(int argc, char **argv);
void sym_output_with_options(FILE *fp, char sym, char *op_flags, int count_flags, int *num_row, int num_sym);

int main(int argc, char *argv[])
{
    FILE *fp;
    int flag = OK;
    if (argc == 2)
    {
        flag = output_of_the_entire_file(argv[1]);
    }
    else if (argc > 2)
    {
        flag = output_with_options(argc, argv);
    }
    else
        flag = ERROR;
    return flag;
}

int output_of_the_entire_file(char *name_of_file)
{
    int flag = OK;
    FILE *fp;
    char sym;
    if ((fp = fopen(name_of_file, "r")) == NULL)
    {
        printf("Не удалось открыть файл");
        flag = ERROR;
    }
    else
    {
        while ((sym = fgetc(fp)) != EOF)
        {
            putchar(sym);
        }
    }
    fclose(fp);
    return flag;
}

int options(int argc, char **argv, char *op_flag, int *count_flags, int *count_arg_flags)
{
    int flag = OK;
    char flags_const[N_FLAGS_CAT] = {"benst"};
    for (int i = 1; i < argc - 1 && flag == OK; i++)
    {
        if (argv[i][0] != '-')
        {
            i = argc - 1;
        }
        else
        {
            *count_arg_flags += 1;
            for (int j = 1; j < strlen(argv[i]) && flag == OK; j++)
            {
                char buf_sym = argv[i][j];
                if (*count_flags <= N_FLAGS_CAT - 1 && strchr(flags_const, buf_sym) != NULL && strchr(op_flag, buf_sym) == NULL)
                {
                    op_flag[*count_flags] = buf_sym;
                    *count_flags += 1;
                }
                else
                {

                    flag = ERROR;
                    printf("2E - %d\n", flag);
                }
            }
        }
    }
    return flag;
}

int output_with_options(int argc, char **argv)
{
    int flag = OK, count_flags = 0, count_arg_flags = 0;
    char op_flags[N_FLAGS_CAT + 1] = {'\0'};
    flag = options(argc, argv, op_flags, &count_flags, &count_arg_flags);
    // if (flag == OK)
    //     printf("%s\n", op_flags);
    // printf("%d %d %d\n", count_arg_flags, argc, flag);
    for (int i = count_arg_flags + 1; i < argc && flag == OK; i++)
    {
        FILE *fp;
        if ((fp = fopen(argv[i], "r")) == NULL)
        {
            printf("Не удалось открыть файл");
            flag = ERROR;
        }
        else
        {
            char sym;
            int num_row = 1, num_sym = 0, flag_enter = 1;
            // printf("222  %s\n", argv[i]);
            while ((sym = fgetc(fp)) != EOF)
            {
                num_sym += 1;
                sym_output_with_options(fp, sym, op_flags, count_flags, &num_row, num_sym);
            }
        }
        fclose(fp);
    }
    return flag;
}

void sym_output_with_options(FILE *fp, char sym, char *op_flags, int count_flags, int *num_row, int num_sym)
{
    if (sym == '\n' && strchr(op_flags, 'e') != NULL)
        putchar('$');
    if (sym == '\n' && strchr(op_flags, 's') != NULL){
        while (fgetc(fp)!= '\n' && fgetc(fp)!=EOF);
    }
    if (strchr(op_flags, 'b') != NULL && ((*num_row == 1 && num_sym == 1) || sym == '\n'))
    {
        char sym_sled = fgetc(fp);
        if (sym_sled != '\n' && num_sym != 1 && sym_sled != EOF)
        {
            printf("%c% 6d\t", sym, *num_row);
            *num_row += 1;
        }
        else if (sym != '\n' && num_sym == 1 && sym_sled != EOF)
        {
            printf("% 6d\t%c", *num_row, sym);
            *num_row += 1;
        }
        else
            putchar(sym);
        if (sym_sled != EOF)
            fseek(fp, -1, SEEK_CUR);
    }
    else if (strchr(op_flags, 'n') != NULL && ((*num_row == 1 && num_sym==1) || sym == '\n')){
        char sym_sled = fgetc(fp);
        if (num_sym != 1 && *num_row != 1 && sym_sled != EOF)
        {
            printf("%c% 6d\t", sym, *num_row);
            *num_row += 1;
        }
        else if (num_sym == 1 && *num_row ==1 && sym_sled != EOF)
        {

            if (sym != '\n')printf("% 6d\t%c", *num_row, sym);
            else printf("% 6d\t", *num_row);
            *num_row += 1;
            if (sym == '\n') fseek(fp, -1, SEEK_CUR);
        }
        else
            putchar(sym);
        if (sym_sled != EOF)
            fseek(fp, -1, SEEK_CUR);
    }
    else putchar(sym);
    // printf("1");
}