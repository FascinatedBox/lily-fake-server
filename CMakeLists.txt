cmake_minimum_required(VERSION 3.0.0)
project("lily-fake-server")

include_directories("${CMAKE_INSTALL_PREFIX}/include/lily/")

add_library(server SHARED lily_fake_server.c)
set_target_properties(server PROPERTIES PREFIX "")
