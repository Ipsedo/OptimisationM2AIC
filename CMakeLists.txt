cmake_minimum_required(VERSION 3.5)
project(OptimisationM2AIC C CXX)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SRC "src/*.h" "src/*.cpp" "src/*.c")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ggdb -std=c89 -pedantic -Wall -Wextra -Wstrict-prototypes -Wshadow -Wno-sign-compare -Wconversion")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(OptimisationM2AIC ${SRC})

target_link_libraries(OptimisationM2AIC m)
