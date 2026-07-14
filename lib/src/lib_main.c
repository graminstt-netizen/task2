/*
lib_main.c - реализация функций библиотеки.

Бабурин Дмитрий Сергеевич
МК-101
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib_main.h"

// Ручной перевод одного байта в шестнадцатеричные символы и вывод через %c
void print_byte_hex(unsigned char b) {
    const char hex_digits[] = "0123456789ABCDEF";
    // Сдвигаем байт на 4 бита вправо, чтобы получить первые 4 бита (старший полубайт)
    printf("%c", hex_digits[(b >> 4) & 0x0F]);
    // Маскируем байт, чтобы получить последние 4 бита (младший полубайт)
    printf("%c", hex_digits[b & 0x0F]);
}

// Функция обработки и вывода одного файла
void process_file(const char *filepath, const AppOptions *opts) {
    FILE *f = fopen(filepath, "rb");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file %s\n", filepath);
        return;
    }

    // Перемещаемся на указанное смещение от начала файла
    if (opts->offset > 0) {
        if (fseek(f, opts->offset, SEEK_SET) != 0) {
            fprintf(stderr, "Error: Cannot seek to offset %ld in file %s\n", opts->offset, filepath);
            fclose(f);
            return;
        }
    }

    long bytes_to_read = opts->length;
    long total_read = 0;
    long current_offset = opts->offset;

    // Вычисляем размер буфера на одну строку
    int max_line_bytes = opts->group_per_line * opts->group_size;
    unsigned char *line_buf = (unsigned char *)malloc(max_line_bytes);
    if (!line_buf) {
        fclose(f);
        return;
    }

    while (1) {
        // Определяем, сколько байт нужно прочитать на текущей строке
        int limit = max_line_bytes;
        if (bytes_to_read != -1) {
            long remaining = bytes_to_read - total_read;
            if (remaining <= 0) break; // Мы прочитали весь лимит
            if (remaining < limit) limit = (int)remaining;
        }

        // Читаем порцию байт в буфер
        int bytes_read = (int)fread(line_buf, 1, limit, f);
        if (bytes_read <= 0) break; // Файл закончился

        // 1. Выводим смещение строки (разрешено через printf)
        printf("%08lx  ", current_offset);

        // 2. Выводим группы в шестнадцатеричном виде (Little Endian)
        // Вычисляем, сколько групп получилось на этой строке (округляем вверх)
        int num_groups = (bytes_read + opts->group_size - 1) / opts->group_size;
        for (int j = 0; j < num_groups; j++) {
            // Выводим байты группы от старшего адреса к младшему (Little Endian)
            for (int i = opts->group_size - 1; i >= 0; i--) {
                int byte_idx = j * opts->group_size + i;
                if (byte_idx < bytes_read) {
                    print_byte_hex(line_buf[byte_idx]);
                } else {
                    print_byte_hex(0x00); // Дополнение нулями
                }
            }
            printf("%c", ' '); // Выводим пробел после каждой группы
        }

        // 3. Вывод ASCII-превью (только если размер группы равен 1)
        if (opts->group_size == 1) {
            int spaces_to_print = (opts->group_per_line - bytes_read) * 3;
            for (int i = 0; i < spaces_to_print; i++) {
                printf("%c", ' ');
            }

            printf("%c", '|');
            printf("%c", ' ');

            for (int i = 0; i < bytes_read; i++) {
                unsigned char c = line_buf[i];
                if (isprint(c)) {
                    printf("%c", c);
                } else {
                    printf("%c", '.');
                }
            }
        }

        printf("%c", '\n'); // Конец строки

        total_read += bytes_read;
        current_offset += bytes_read;
    }

    free(line_buf);
    fclose(f);
}