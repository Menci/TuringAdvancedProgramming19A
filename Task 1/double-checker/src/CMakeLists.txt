project(double-checker CXX)

set(CMAKE_CXX_FLAGS "-Wall -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC "*.cc" "**/*.cc")
add_executable(double-checker ${SRC})

target_link_libraries(double-checker dl argument-parser)
