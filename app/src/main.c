/*
main.c - главный модуль программы.

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

    // временный вывод, чтобы проверить, как работает гетопт
    printf("Debug: Arguments parsed successfully!\n");
    printf("File Name: %s\n", opts.file_name ? opts.file_name : "NULL");
    printf("Offset: %ld\n", opts.offset);
    printf("Length: %ld\n", opts.length);
    printf("Group Size: %d\n", opts.group_size);
    printf("Groups Per Line: %d\n", opts.group_per_line);
    printf("Directory: %s\n", opts.dir_name ? opts.dir_name : "NULL");

    return 0;
}