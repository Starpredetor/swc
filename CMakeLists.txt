

#Cmake defaults
cmake_minimum_required(VERSION 3.20)
project(swc VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)


#LLVM package requirements
find_package(Clang REQUIRED CONFIG)
add_executable(swc src/main.cpp)
target_link_libraries(swc PRIVATE libclang)
