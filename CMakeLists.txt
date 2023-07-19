cmake_minimum_required(VERSION 3.0)
project(gxh-from-scratch)

include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)

# 指定编译选项
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11 -O0 -ggdb -Wall -Werror")

# -rdynamic: 将所有符号都加入到符号表中，便于使用dlopen或者backtrace追踪到符号
# -fPIC: 生成位置无关的代码，便于动态链接
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -fPIC")

# -Wno-unused-function: 不要警告未使用函数
# -Wno-builtin-macro-redefined: 不要警告内置宏重定义，用于重定义内置的__FILE__宏
# -Wno-deprecated: 不要警告过时的特性
# -Wno-deprecated-declarations: 不要警告使用带deprecated属性的变量，类型，函数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated -Wno-deprecated-declarations")

include_directories(.)

option(BUILD_TEST "ON for complile test" ON)

find_package(Boost REQUIRED) 
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(LIB_SRC
    gxh/log.cpp
    gxh/util.cpp
    gxh/mutex.cc
    gxh/env.cc
    gxh/config.cc
    gxh/thread.cc
    gxh/fiber.cc
    gxh/scheduler.cc
    gxh/iomanager.cc
    gxh/timer.cc
    gxh/fd_manager.cc
    gxh/hook.cc
    gxh/address.cc 
    gxh/socket.cc 
    gxh/bytearray.cc 
    gxh/tcp_server.cc 
    gxh/http/http-parser/http_parser.c 
    gxh/http/http.cc
    gxh/http/http_parser.cc 
    gxh/stream.cc 
    gxh/streams/socket_stream.cc
    gxh/http/http_session.cc 
    gxh/http/servlet.cc
    gxh/http/http_server.cc 
    gxh/uri.cc 
    gxh/http/http_connection.cc 
    gxh/daemon.cc 
    gxh/db/mysql_pool.cc
    )

add_library(gxh SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(gxh)

set(LIBS
    gxh
    pthread
    dl
    yaml-cpp
    mysqlclient
)

if(BUILD_TEST)
gxh_add_executable(test_log "tests/test_log.cpp" gxh "${LIBS}")
gxh_add_executable(test_util "tests/test_util.cpp" gxh "${LIBS}")
gxh_add_executable(test_env "tests/test_env.cc" gxh "${LIBS}")
gxh_add_executable(test_config "tests/test_config.cc" gxh "${LIBS}")
gxh_add_executable(test_thread "tests/test_thread.cc" gxh "${LIBS}")
gxh_add_executable(test_fiber "tests/test_fiber.cc" gxh "${LIBS}")
gxh_add_executable(test_fiber2 "tests/test_fiber2.cc" gxh "${LIBS}")
gxh_add_executable(test_scheduler "tests/test_scheduler.cc" gxh "${LIBS}")
gxh_add_executable(test_iomanager "tests/test_iomanager.cc" gxh "${LIBS}")
gxh_add_executable(test_timer "tests/test_timer.cc" gxh "${LIBS}")
gxh_add_executable(test_hook "tests/test_hook.cc" gxh "${LIBS}")
gxh_add_executable(test_address "tests/test_address.cc" gxh "${LIBS}")
gxh_add_executable(test_socket_tcp_server "tests/test_socket_tcp_server.cc" gxh "${LIBS}")
gxh_add_executable(test_socket_tcp_client "tests/test_socket_tcp_client.cc" gxh "${LIBS}")
gxh_add_executable(test_bytearray "tests/test_bytearray.cc" gxh "${LIBS}")
gxh_add_executable(test_tcp_server "tests/test_tcp_server.cc" gxh "${LIBS}")
gxh_add_executable(test_http "tests/test_http.cc" gxh "${LIBS}")
gxh_add_executable(test_http_parser "tests/test_http_parser.cc" gxh "${LIBS}")
gxh_add_executable(test_http_server "tests/test_http_server.cc" gxh "${LIBS}")
gxh_add_executable(test_uri "tests/test_uri.cc" gxh "${LIBS}")
gxh_add_executable(test_http_connection "tests/test_http_connection.cc" gxh "${LIBS}")
gxh_add_executable(test_daemon "tests/test_daemon.cc" gxh "${LIBS}")
gxh_add_executable(test_mysql_pool "tests/test_mysql_pool.cc" gxh "${LIBS}")
gxh_add_executable(tiny_webserver "gxh/web/tiny_webserver.cc" gxh "${LIBS}")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)