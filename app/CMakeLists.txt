get_all_sources("${CMAKE_CURRENT_SOURCE_DIR}/src" APP_SOURCES)
add_executable(my_program ${APP_SOURCES})

# Связываем наше приложение как с основной библиотекой (app_lib), так и с библиотекой getopt (lib2_getopt)
target_link_libraries(my_program PRIVATE app_lib lib2_getopt)

# Тесты программы
#if(BUILD_TESTING)
#    add_subdirectory(tests)
#endif()