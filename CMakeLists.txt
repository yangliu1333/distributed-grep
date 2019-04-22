cmake_minimum_required(VERSION 3.13)
project(mp1_boost)

set(CMAKE_CXX_STANDARD 11)

SET(BOOST_ROOT ${CMAKE_INCLUDE_PATH} "/Users/yangliu/tools/boost_1_68_0/boost")

find_package(Boost COMPONENTS system filesystem regex program_options REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})


if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)


ADD_SUBDIRECTORY(googletest)

#json
set(JSON_BuildTests OFF CACHE INTERNAL "")
ADD_SUBDIRECTORY(json)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
message("hello" ${gtest_SOURCE_DIR})


add_library(grep STATIC grep.h grep.cpp)

add_executable(grep_client grep_client.cpp)
add_executable(grep_server grep_server.cpp)

add_executable(grep_test grep_test.cpp)

if(Boost_FOUND)
    target_link_libraries(grep_client ${Boost_LIBRARIES} nlohmann_json::nlohmann_json grep)
    target_link_libraries(grep_server ${Boost_LIBRARIES} nlohmann_json::nlohmann_json grep)

    message("111" ${gtest_SOURCE_DIR})
    target_link_libraries(grep ${Boost_LIBRARIES})
endif()

#target_link_libraries(grep_client PRIVATE )

target_link_libraries(grep_test gtest gtest_main grep)