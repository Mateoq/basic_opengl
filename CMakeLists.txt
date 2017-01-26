# Specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Project's name

project(Game CXX)
# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

# Set the compiler config
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(flags "-std=c++1z -stdlib=libc++ -m64 -Wall -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(flags "/std:c++17 /W4 /WX /EHsc /ZI /MACHINE:X64")
endif()
  
if (NOT CONFIGURED_ONCE)
  set(CMAKE_CXX_FLAGS "${flags}")
endif()

# Find specific library
find_library(FREE_IMAGE freeImagePlus)

# Assimp
find_library(ASSIMP_LIBRARY assimp)
#add_subdirectory("${CMAKE_SOURCE_DIR}/dependencies/lib/assimp")

# Include subfolders with CMAKE files.
# GLFW
add_subdirectory("${CMAKE_SOURCE_DIR}/dependencies/lib/glfw")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


# The folders with the header files.
include_directories("${CMAKE_SOURCE_DIR}/dependencies/include")
include_directories(${SDL2_INCLUDE_DIR})


# Include recursively folders.
# file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
# add_executable(foo ${SRC_FILES})


add_executable(Game
  ${PROJECT_SOURCE_DIR}/dependencies/lib/glad.cpp
  ${PROJECT_SOURCE_DIR}/src/Camera.cpp
  ${PROJECT_SOURCE_DIR}/src/Shader.cpp
  ${PROJECT_SOURCE_DIR}/src/Mesh.cpp
  ${PROJECT_SOURCE_DIR}/src/Model.cpp
  ${PROJECT_SOURCE_DIR}/src/TextureLoader.cpp
  ${PROJECT_SOURCE_DIR}/src/Cube.cpp
  ${PROJECT_SOURCE_DIR}/src/Plane.cpp
  ${PROJECT_SOURCE_DIR}/src/main.cpp
)

# Link a library
target_link_libraries(Game glfw freeImagePlus assimp)

# Install
install(TARGETS Game RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/bin")

