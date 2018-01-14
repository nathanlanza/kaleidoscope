cmake_minimum_required(VERSION 3.8)
project(kaleidoscope)

execute_process(COMMAND bash "-c" "llvm-config --cxxflags --system-libs --libs core mcjit native | tr '\n' ' '" OUTPUT_VARIABLE compflags)
execute_process(COMMAND bash "-c" "llvm-config --ldflags --system-libs --libs core mcjit native | tr '\n' ' '" OUTPUT_VARIABLE linkflags)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${compflags}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${linkflags}")

add_subdirectory(googletest)
add_subdirectory(test)

set(source_files main.cpp)
add_executable(kaleidoscope ${source_files})


