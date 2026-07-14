/*
Заголовочный файл библиотеки hexview. Определение интерфейсов.

Бабурин Дмитрий Сергеевич
МК-101
*/

#ifndef LIB_MAIN_H
#define LIB_MAIN_H

// Структура для хранения параметров нашей программы
typedef struct {
    char *file_name;       // Имя входного файла (-i)
    long offset;           // Смещение от начала файла (-o)
    long length;           // Сколько байт выводить (-l), по умолчанию -1 (всё)
    int group_size;        // Размер кусочка (-g)
    int group_per_line;    // Кол-во кусочков в одной строке (-n)
    char *dir_name;        // Директория для обработки файлов (-d)
} AppOptions;

// Ручной перевод одного байта в шестнадцатеричные символы и вывод через %c
void print_byte_hex(unsigned char b);

#endif // LIB_MAIN_H