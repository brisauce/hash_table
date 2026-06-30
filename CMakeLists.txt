cmake_minimum_required(VERSION 3.22.1)

project(
hash_table 
VERSION 0.1.1
LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(${PROJECT_NAME} main.c file_to_buf.c CLI.c dynamic_array.c)
configure_file(${PROJECT_NAME}Config.h.in ${PROJECT_NAME}Config.h)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

list(APPEND MY_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR})
list(APPEND MY_INCLUDE_DIRECTORIES ${PROJECT_BUILD_DIR})

target_compile_options(${PROJECT_NAME} PRIVATE -Werror)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
  target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
  target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
  target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=undefined)
  target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=undefined)
endif()


target_include_directories(${PROJECT_NAME}
        PRIVATE ${MY_INCLUDE_DIRECTORIES}
)
