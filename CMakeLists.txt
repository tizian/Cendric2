cmake_minimum_required(VERSION 2.8)

project(Cendric)

add_subdirectory("${PROJECT_SOURCE_DIR}/ext/sfml")

if(WIN32)
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
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
endif()

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  add_definitions (/D "_CRT_SECURE_NO_WARNINGS")
endif()

include_directories(
	"${PROJECT_SOURCE_DIR}/ext/sfml/include"
	"${PROJECT_SOURCE_DIR}/ext"
	"${PROJECT_SOURCE_DIR}/include"
)

file(GLOB_RECURSE Cendric_FILES
	"${PROJECT_SOURCE_DIR}/include/*.h"
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/ext/Particles/*.cpp"
	"${PROJECT_SOURCE_DIR}/ext/lua/src/*.c"
	"${PROJECT_SOURCE_DIR}/ext/sqlite/sqlite3.c"
	"${PROJECT_SOURCE_DIR}/ext/tinyxml2/*.cpp"
)

file(GLOB_RECURSE SQLiteShell_FILES
	"${PROJECT_SOURCE_DIR}/ext/sqlite/*.c"
	"${PROJECT_SOURCE_DIR}/ext/sqlite/*.h"
)

add_executable(Cendric
	${Cendric_FILES}
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_library(CendricApple STATIC "${PROJECT_SOURCE_DIR}/src/Platform/CendricApple.mm")
	set_target_properties(CendricApple PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
	target_link_libraries(Cendric CendricApple)
endif()

set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -lpthread -ldl")

if (WIN32)
	add_executable(SQLiteShell ${SQLiteShell_FILES})
endif()

target_link_libraries(Cendric sfml-system sfml-window sfml-graphics sfml-audio)
