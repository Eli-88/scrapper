project(Scrapper)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

find_package(Boost REQUIRED COMPONENTS system json timer regex program_options)
find_package(OpenSSL REQUIRED)

add_subdirectory(src)
