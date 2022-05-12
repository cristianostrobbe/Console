cmake_minimum_required(VERSION 3.0)
project(TestConsole)

find_package(Console REQUIRED)

if (Console_FOUND)
    message("Console Found")
else()
    message("Console not Found")
endif()

message("Console library at:")
message("${CONSOLE_LIBRARY_PATH}")

add_executable(test_install test_install.cc)
target_link_libraries(test_install Console)