cmake_minimum_required(VERSION 3.14)

project(Compiler-course-hw
  VERSION 1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED graph)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES "${SRC_DIR}/*")

foreach (SOURCE ${SOURCES})
  get_filename_component(target ${SOURCE} NAME_WE)
  add_executable(${target})
  message("${target}")
  target_sources(${target}
  PRIVATE
    ${SOURCE}
  )
  target_include_directories(${target}
  PRIVATE
    ${INCLUDE_DIR}
  )
endforeach()

