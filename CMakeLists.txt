cmake_minimum_required(VERSION 3.17)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c parametrs.h calculus.h calc.c)

include_directories(lib)
find_library(LIBTOMMATH libtommath.a lib)


target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBTOMMATH})