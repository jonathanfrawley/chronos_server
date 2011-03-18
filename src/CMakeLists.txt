cmake_minimum_required(VERSION 2.8)

Project ( chronos )

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#FIND_PACKAGE(Boost COMPONENTS program_options unit_test_framework system filesystem thread REQUIRED)
FIND_PACKAGE(Boost COMPONENTS program_options system filesystem thread date_time REQUIRED)
#FIND_PACKAGE(Qt4 REQUIRED)
find_package (Threads)

SET(libCtThirdParty ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

#FILE(GLOB ct_core_cpp "./ctCore/*.cpp")
#ADD_LIBRARY(libCtCore ${ct_core_cpp})

FILE(GLOB ct_server_cpp "./ctServer/*.cpp")
ADD_EXECUTABLE(ct_server ${ct_server_cpp})
#TARGET_LINK_LIBRARIES(ct_server libCtCore ${libCtThirdParty})
TARGET_LINK_LIBRARIES(ct_server ${libCtThirdParty})

FILE(GLOB ct_client_cpp "./ctClient/*.cpp")
ADD_EXECUTABLE(ct_client ${ct_client_cpp})
#TARGET_LINK_LIBRARIES(ct_client libCtCore ${libCtThirdParty})
TARGET_LINK_LIBRARIES(ct_client ${libCtThirdParty})
