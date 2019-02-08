cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ThreadPool CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic-errors -Werror=return-type")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(THREAD_POOL_HEADERS
    src/ThreadPool.h
    )

set(THREAD_POOL_SOURCES
    src/ThreadPool.cpp
    )

add_library(thread-pool SHARED
    ${THREAD_POOL_SOURCES})

install(TARGETS thread-pool
        DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/")

install(FILES        ${THREAD_POOL_HEADERS}
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ThreadPool")

