cmake_minimum_required(VERSION 3.20)
project(CCApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

find_package(fmt REQUIRED)
find_package(OpenMP REQUIRED)

add_subdirectory(src)

# cmake -S . -B build/ -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_MAKE_PROGRAM=/bin/make -DCMAKE_CXX_COMPILER=/bin/clang++ -DCMAKE_C_COMPILER=/bin/clang -DCMAKE_BUILD_TYPE=DEBUG
