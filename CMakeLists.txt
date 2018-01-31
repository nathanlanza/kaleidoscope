cmake_minimum_required(VERSION 3.8)

project(kaleidoscope CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

#execute_process(
#  COMMAND bash "-c" "llvm-config --cxxflags --system-libs --libs core mcjit native | tr '\n' ' '" 
#  OUTPUT_VARIABLE compflags
#  )
#execute_process(
#  COMMAND bash "-c" "llvm-config --ldflags --system-libs --libs core mcjit native | tr '\n' ' '" 
#  OUTPUT_VARIABLE linkflags
#  )

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${compflags}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${linkflags}")

set(source_files 
  Utilities.cpp
  main.cpp 
  Lexer.cpp
  Parser.cpp
  AST.cpp 
  CodeGen.cpp
  )

add_executable(kaleidoscope ${source_files})

llvm_map_components_to_libnames(llvm_libs 
  Analysis
  Core
  ExecutionEngine
  InstCombine
  native 
  Object
  RuntimeDyld
  ScalarOpts
  Support
  )

target_link_libraries(kaleidoscope ${llvm_libs})

