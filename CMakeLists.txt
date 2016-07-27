cmake_minimum_required(VERSION 2.8)

project(Cendric)

set(CMAKE_BUILD_TYPE Release)

option(CENDRIC_APPLE_BUILD_APP_BUNDLE "Build .app bundle on Mac platform?" OFF)

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

file(GLOB_RECURSE DialogueTool_FILES
	"${PROJECT_SOURCE_DIR}/tools/DialogueTool/include/*.h"
	"${PROJECT_SOURCE_DIR}/tools/DialogueTool/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/ext/imgui/imgui.cpp"
	"${PROJECT_SOURCE_DIR}/ext/imgui/imgui_draw.cpp"
	"${PROJECT_SOURCE_DIR}/ext/imgui/imgui_demo.cpp"
	"${PROJECT_SOURCE_DIR}/ext/imgui-sfml/imgui-SFML.cpp"
)

if (WIN32)
	add_executable(Cendric
		${Cendric_FILES}
		${PROJECT_SOURCE_DIR}/res/info.rc
	)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND CENDRIC_APPLE_BUILD_APP_BUNDLE)
	add_executable(Cendric MACOSX_BUNDLE
		${Cendric_FILES}
		${PROJECT_SOURCE_DIR}/res/icon.icns
	)
else()
	add_executable(Cendric
		${Cendric_FILES}
	)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_library(CendricApple STATIC "${PROJECT_SOURCE_DIR}/src/Platform/CendricApple.mm")
	set_target_properties(CendricApple PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
	target_link_libraries(Cendric CendricApple)
endif()

set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -lpthread -ldl")

target_link_libraries(Cendric sfml-system sfml-window sfml-graphics sfml-audio)

if (APPLE)
	set_target_properties(Cendric PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/res/info.plist)
	set_source_files_properties(res/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

if (WIN32)
	add_executable(SQLiteShell ${SQLiteShell_FILES})
	add_executable(DialogueTool ${DialogueTool_FILES})
	
	target_link_libraries(DialogueTool sfml-system sfml-graphics sfml-window)
endif()