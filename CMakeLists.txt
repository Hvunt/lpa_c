cmake_minimum_required(VERSION 3.10)

project(LPA VERSION 0.2 LANGUAGES C)

set(CMAKE_C_STANDART 11)

find_library(PSAPI Psapi)


set(SOURCE main.c lpa.c list.c node.c priority_queue.c)
add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} m)
target_link_libraries(${PROJECT_NAME} -lpsapi)
target_include_directories(${PROJECT_NAME} PRIVATE inc)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)