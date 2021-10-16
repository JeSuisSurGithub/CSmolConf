cmake_minimum_required(VERSION 3.10)
project(csc_test VERSION 1.0.0)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Source files
add_executable(csc_test "${CMAKE_CURRENT_LIST_DIR}/main.c")

# Includes
target_include_directories(csc_test PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${CSC_DIR}")

# Flags
target_compile_options(csc_test PUBLIC "-Wall" "-ggdb")
