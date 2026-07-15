# Создаем библиотеку из getopt.c
add_library(lib2_getopt STATIC src/getopt.c)

# Указываем, где искать getopt.h
target_include_directories(lib2_getopt PUBLIC include)