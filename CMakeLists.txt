cmake_minimum_required(VERSION 3.8)
project(tt1_wordnet)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBRARIES OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(SOURCE_FILES wordnet.h FileAccess.cpp FileAccess.h Database.cpp Database.h Input.cpp Input.h Output.cpp Output.h)
add_executable(tt1_wordnet main.cpp 3rdparty/CLI11.hpp ${SOURCE_FILES})

set(TEST_FILES 3rdparty/catch.h tst_main.cpp tst_FileAccess.cpp )
add_executable(tst_tt1_wordnet ${SOURCE_FILES} ${TEST_FILES})