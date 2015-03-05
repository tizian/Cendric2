cmake_minimum_required(VERSION 2.6)
project(Cendric2)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/SFML-2.2")
include_directories("${PROJECT_SOURCE_DIR}/SFML-2.2/include")

set(ALL_SOURCES
	main.cpp
)

add_executable(Cendric2 ${ALL_SOURCES})
target_link_libraries(Cendric2 sfml-main sfml-system sfml-window sfml-graphics sfml-audio)