cmake_minimum_required(VERSION 3.0)

project(overtelord9000)

add_executable(overtelord9000 main.cpp)

install(TARGETS overtelord9000 RUNTIME DESTINATION bin)
