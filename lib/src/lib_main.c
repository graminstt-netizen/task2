/*
lib_main.c - реализация функций библиотеки.

Бабурин Дмитрий Сергеевич
МК-101
*/

#include <stdio.h>
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

    // Временный вывод для отладки, подтверждающий успешный поиск
    printf("Debug: Successfully opened file '%s' and seeked to offset %ld\n", filepath, opts->offset);

    fclose(f);
}