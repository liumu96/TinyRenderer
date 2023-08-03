cmake_minimum_required(VERSION 3.1)
project(tinyrenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Reference: https://github.com/actions/runner-images/issues/5555
if(APPLE)
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(OpenMP_C "${CMAKE_C_COMPILER}")
    set(OpenMP_C_FLAGS "-Xclang -fopenmp -I/opt/homebrew/Cellar/libomp/16.0.6/include")
    set(OpenMP_C_LIB_NAMES "libomp")
    set(OpenMP_libomp_LIBRARY "omp")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
    set(OpenMP_CXX_FLAGS "-Xclang -fopenmp -I/opt/homebrew/Cellar/libomp/16.0.6/include")
    set(OpenMP_CXX_LIB_NAMES "libomp")
    set(OpenMP_libomp_LIBRARY "omp")
endif()
link_directories("/opt/homebrew/Cellar/libomp/16.0.6/lib/")
endif()

find_package(OpenMP)
if(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB SOURCES *.h *.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "/opt/homebrew/Cellar/libomp/16.0.6/lib/libomp.dylib")
    # target_link_directories(${PROJECT_NAME} PRIVATE "/opt/homebrew/Cellar/libomp/16.0.6/lib/")
endif(APPLE)
