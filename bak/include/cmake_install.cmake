# Install script for directory: /home/long/code/jsoncpp/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/json" TYPE FILE FILES
    "/home/long/code/jsoncpp/include/json/autolink.h"
    "/home/long/code/jsoncpp/include/json/features.h"
    "/home/long/code/jsoncpp/include/json/allocator.h"
    "/home/long/code/jsoncpp/include/json/config.h"
    "/home/long/code/jsoncpp/include/json/reader.h"
    "/home/long/code/jsoncpp/include/json/forwards.h"
    "/home/long/code/jsoncpp/include/json/assertions.h"
    "/home/long/code/jsoncpp/include/json/json.h"
    "/home/long/code/jsoncpp/include/json/value.h"
    "/home/long/code/jsoncpp/include/json/version.h"
    "/home/long/code/jsoncpp/include/json/writer.h"
    )
endif()

