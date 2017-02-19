cmake_minimum_required(VERSION 2.4)

# Project setup, C language and name
enable_language(C)
project(date_lib)

# Source files definitions
set(SOURCES date.c)

# Release
add_executable(date ${SOURCES})

