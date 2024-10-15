cmake_minimum_required(VERSION 3.27)
project(Balls)

include_directories(./headers/)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(OpenMP REQUIRED)
find_package(yaml-cpp REQUIRED)

set(SOURCE_DIR "src")
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")

add_executable (Balls ${SOURCES})

target_include_directories(Balls PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Balls PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(Balls PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(Balls PRIVATE yaml-cpp)

