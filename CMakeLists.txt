cmake_minimum_required(VERSION 3.8)
project(Kaleidoscope)
set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(source_files main.cpp)
add_executable(kaleidoscope ${source_files})

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(kaleidoscope ${llvm_libs})


