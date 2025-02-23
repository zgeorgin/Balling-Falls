cmake_minimum_required(VERSION 3.27)
project(Balls)

include_directories(./headers/)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenMP REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(OpenCV REQUIRED)

set(SOURCE_DIR "src")
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")

add_executable (Balls ${SOURCES})

add_subdirectory(processingPhoto)

target_include_directories(Balls PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_include_directories(Balls PRIVATE ${DrawingFeatures_INCLUDE_DIRS})
target_link_libraries(Balls PUBLIC SDL2::SDL2 SDL2_image::SDL2_image)
target_link_libraries(Balls PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(Balls PRIVATE yaml-cpp)
target_include_directories(Balls PRIVATE ${OpenCV_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(Balls PRIVATE ${OpenCV_LIBS})

add_compile_options(-g)
