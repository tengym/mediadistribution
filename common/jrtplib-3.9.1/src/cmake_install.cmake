# Install script for directory: /home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/tengym/ts_over_ip/MediaDistribution/common")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/jrtplib3" TYPE FILE FILES
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcpapppacket.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcpbyepacket.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcpcompoundpacket.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcpcompoundpacketbuilder.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcppacket.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcppacketbuilder.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcprrpacket.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcpscheduler.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcpsdesinfo.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcpsdespacket.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcpsrpacket.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtcpunknownpacket.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpaddress.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpcollisionlist.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpconfig.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpdebug.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpdefines.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtperrors.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtphashtable.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpinternalsourcedata.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpipv4address.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpipv4destination.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpipv6address.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpipv6destination.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpkeyhashtable.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtplibraryversion.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpmemorymanager.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpmemoryobject.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtppacket.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtppacketbuilder.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtppollthread.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtprandom.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtprandomrand48.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtprandomrands.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtprandomurandom.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtprawpacket.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpsession.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpsessionparams.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpsessionsources.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpsourcedata.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpsources.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpstructs.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtptimeutilities.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtptransmitter.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtptypes_win.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtptypes.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpudpv4transmitter.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpudpv6transmitter.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpbyteaddress.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/rtpexternaltransmitter.h"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/extratransmitters/rtpfaketransmitter.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/tengym/ts_over_ip/MediaDistribution/common/lib/libjrtp.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/tengym/ts_over_ip/MediaDistribution/common/lib" TYPE STATIC_LIBRARY FILES "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/libjrtp.a")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/tengym/ts_over_ip/MediaDistribution/common/lib/libjrtp.so.3.9.1;/home/tengym/ts_over_ip/MediaDistribution/common/lib/libjrtp.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/tengym/ts_over_ip/MediaDistribution/common/lib" TYPE SHARED_LIBRARY FILES
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/CMakeFiles/CMakeRelink.dir/libjrtp.so.3.9.1"
    "/home/tengym/ts_over_ip/MediaDistribution/common/jrtplib-3.9.1/src/CMakeFiles/CMakeRelink.dir/libjrtp.so"
    )
endif()

