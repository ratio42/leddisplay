cmake_minimum_required(VERSION 3.23)
project(leddisplay)

set(CMAKE_CXX_STANDARD 14)

#get googletest - according to description from Udemy course "C++ Unit Testing: Google Test and Google Mock"
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG release-1.12.1
)
FetchContent_GetProperties(googletest)
#variables that will be set: googletest_POPULATED, googletest_SOURCE_DIR, googletest_BINARY_DIR
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()


#define library that is being build: leddisplay (as a shared library)
add_library(leddisplay SHARED library.cpp library.h internal.h)

#define executable that is being build: UnitTest
add_executable(UnitTest UnitTest/unittest.cpp)
#link leddisplay against the executable
target_link_libraries(UnitTest leddisplay gtest_main) # and gmock_main