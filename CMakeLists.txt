cmake_minimum_required(VERSION 3.2)
project(httpserver)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(httpserver
        condition.cpp
        condition.h
        handle.cpp
        handle.h
        httpRequest.cpp
        httpRequest.h
        httpResponse.cpp
        httpResponse.h
        InetAddress.cpp
        InetAddress.h
        server.cpp
        Socket.cpp
        Socket.h
        threadpool.cpp
        threadpool.h
        TimeStamp.cpp
        TimeStamp.h
        Util.cpp
        Util.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")