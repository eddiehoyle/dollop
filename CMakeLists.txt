cmake_minimum_required(VERSION 3.10)
project(dollop LANGUAGES CXX)

set(DLP_VERSION_MAJOR 0)
set(DLP_VERSION_MINOR 0)
set(DLP_VERSION_PATCH 0)
set(DLP_VERSION ${DLP_VERSION_MAJOR}.${DLP_VERSION_MINOR}.${DLP_VERSION_PATCH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(DLP_LOGGING_ENABLED true)
set(DLP_ASSERT_ENABLED true)

set(DLP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(DLP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(DLP_EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext)

include_directories(
        ${DLP_INCLUDE_DIR})

add_definitions(
        -DDLP_LOGGING_ENABLED=${DLP_LOGGING_ENABLED}
        -DDLP_ASSERT_ENABLED=${DLP_ASSERT_ENABLED}
        -DDLP_SOURCE_DIR="${DLP_SOURCE_DIR}")

add_subdirectory(ext)
add_subdirectory(src)
add_subdirectory(test)