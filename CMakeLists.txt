cmake_minimum_required(VERSION 3.14)

project(knork)

# Set the source directory
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Get all source files in the source directory
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.cxx" "${SOURCE_DIR}/*.cc" "${SOURCE_DIR}/*.c")

add_executable(knork ${SOURCES})

# target_link_libraries(knork PRIVATE stdc++fs)

install(TARGETS knork RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
