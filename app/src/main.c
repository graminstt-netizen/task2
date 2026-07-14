/*
main.c - главный модуль программы (только для Windows).

Бабурин Дмитрий Сергеевич
МК-101
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getopt.h"
#include "lib_main.h"

int main(int argc, char *argv[]) {
    // Инициализируем настройки значениями по умолчанию
    AppOptions opts;
    opts.file_name = NULL;
    opts.offset = 0;
    opts.length = -1; // по умолчанию выводим всё
    opts.group_size = 1;
    opts.group_per_line = 16;
    opts.dir_name = NULL;

    int opt;
    // настраиваем getopt, символ ':' после буквы означает, что флаг требует значения
    while ((opt = getopt(argc, argv, "hi:o:l:g:n:d:")) != -1) {
        switch (opt) {
            case 'h':
                printf("Usage: %s [-i file_name] [-o offset] [-l size] [-g size] [-n count] [-d dir]\n", argv[0]);
                return 0;
            case 'i':
                opts.file_name = optarg;
                break;
            case 'o':
                // strtol безопасно переводит строку в число лонг
                opts.offset = strtol(optarg, NULL, 10);
                break;
            case 'l':
                opts.length = strtol(optarg, NULL, 10);
                break;
            case 'g':
                opts.group_size = (int)strtol(optarg, NULL, 10);
                break;
            case 'n':
                opts.group_per_line = (int)strtol(optarg, NULL, 10);
                break;
            case 'd':
                opts.dir_name = optarg;
                break;
            default:
                fprintf(stderr, "Error: Unknown option. Use -h for help.\n");
                return 1;
        }
    }



    // 1. Проверяем, что указано что-то одно: либо файл, либо директория
    if (opts.file_name == NULL && opts.dir_name == NULL) {
        fprintf(stderr, "Error: You must specify either file (-i) or directory (-d).\n");
        return 1;
    }
    if (opts.file_name != NULL && opts.dir_name != NULL) {
        fprintf(stderr, "Error: You cannot specify both file (-i) and directory (-d) at the same time.\n");
        return 1;
    }

    // 2. Проверяем корректность смещения
    if (opts.offset < 0) {
        fprintf(stderr, "Error: Offset (-o) must be non-negative.\n");
        return 1;
    }

    // 3. Проверяем лимит на чтение
    if (opts.length <= 0 && opts.length != -1) {
        fprintf(stderr, "Error: Length (-l) must be positive.\n");
        return 1;
    }

    // 4. Проверяем размер кусочка и количество колонок
    if (opts.group_size <= 0) {
        fprintf(stderr, "Error: Group size (-g) must be positive.\n");
        return 1;
    }
    if (opts.group_per_line <= 0) {
        fprintf(stderr, "Error: Groups per line (-n) must be positive.\n");
        return 1;
    }


    // Если задана директория, обрабатываем её файлы, иначе — один файл
    if (opts.dir_name != NULL) {
        process_directory(opts.dir_name, &opts);
    } else {
        process_file(opts.file_name, &opts);
    }

    return 0;
}