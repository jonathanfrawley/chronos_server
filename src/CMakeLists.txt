cmake_minimum_required(VERSION 2.8)

Project ( chronos )

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#FIND_PACKAGE(Boost COMPONENTS program_options unit_test_framework system filesystem thread REQUIRED)
#FIND_PACKAGE(Boost COMPONENTS program_options system filesystem thread date_time REQUIRED)
FIND_PACKAGE(Boost COMPONENTS program_options system filesystem date_time REQUIRED)
#FIND_PACKAGE(Qt4 REQUIRED)
find_package (Threads)

SET(libCtThirdParty ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

#FILE(GLOB ct_core_cpp "./ctCore/*.cpp")
#ADD_LIBRARY(libCtCore ${ct_core_cpp})

FILE(GLOB cts_cpp "./ctServer/*.cpp")
ADD_EXECUTABLE(cts ${cts_cpp})
#TARGET_LINK_LIBRARIES(cts libCtCore ${libCtThirdParty})
TARGET_LINK_LIBRARIES(cts ${libCtThirdParty})

FILE(GLOB ctc_cpp "./ctClient/*.cpp")
ADD_EXECUTABLE(ctc ${ctc_cpp})
#TARGET_LINK_LIBRARIES(ctc libCtCore ${libCtThirdParty})
TARGET_LINK_LIBRARIES(ctc ${libCtThirdParty})
