cmake_minimum_required(VERSION 3.8)
project(tt1_wordnet)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBRARIES OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(SOURCE_FILES wordnet.h Database.cpp Database.h)
add_executable(tt1_wordnet main.cpp ${SOURCE_FILES})

set(TEST_FILES 3rdparty/catch.h tst_Database.cpp tst_main.cpp)
add_executable(tst_tt1_wordnet ${SOURCE_FILES} ${TEST_FILES})