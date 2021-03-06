cmake_minimum_required(VERSION 3.15)
project(Console VERSION 2.1.0 DESCRIPTION "Beautiful Log library")

option(BUILD_EXAMPLES "Make tests also" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -W -O3 -march=native")

find_package(Boost 1.54.0 REQUIRED COMPONENTS filesystem)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_library(${PROJECT_NAME} SHARED
    src/Console.cc
    src/LogTimes.cc
    src/Browse.cc
    src/Profiler.cc
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

# Install console scripts
# install(FILES inc/Console.h inc/TerminalCommads.h DESTINATION include/${PROJECT_NAME}) # cmake install prefix
# install(TARGETS ${PROJECT_NAME} DESTINATION lib) # cmake install prefix

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/inc)

if(BUILD_EXAMPLES)
    add_executable(simple_console examples/simple_console_example.cc)
    target_link_libraries(simple_console ${PROJECT_NAME})

    add_executable(advanced_console examples/advanced_console_example.cc)
    target_link_libraries(advanced_console ${PROJECT_NAME})

    add_executable(logtimes_example examples/logtimes_example.cc)
    target_link_libraries(logtimes_example ${PROJECT_NAME})

    add_executable(browse_example examples/browse_example.cc)
    target_link_libraries(browse_example ${PROJECT_NAME})

    add_executable(profiler_example examples/profiler_example.cc)
    target_link_libraries(profiler_example ${PROJECT_NAME})
endif()

if(BUILD_TEST)
endif()
