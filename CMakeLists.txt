cmake_minimum_required(VERSION 2.8.12)

project(Cendric)

option(CENDRIC_BUILD_SQLITE_SHELL "Build SQLite shell on Windows platform?" OFF)
option(CENDRIC_BUILD_DIALOGUE_TOOL "Build Dialogue Tool on Windows platform?" ON)
option(CENDRIC_STEAM "Include steamworks API?" OFF)
option(CENDRIC_EXTERNAL_DOCUMENT_FOLDER "Use external documents folder?" OFF)
option(CENDRIC_GERMAN "Use German as default language?" OFF)

if (NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ext/sfml/src)
    message(FATAL_ERROR
        "Seems like some of the required dependencies are missing. "
        "This can happen if you did not clone the project with the --recursive flag. "
        "It is possible to recover by calling \"git submodule update --init --recursive\""
    )
endif()

include(CheckCXXCompilerFlag)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if (APPLE)
	option(SFML_BUILD_FRAMEWORKS "" ON)
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/ext/sfml")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    add_definitions (/D "_CRT_SECURE_NO_WARNINGS")
endif()

# enable modern c++ on clang or gcc
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)

    if (HAS_CPP14_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    elseif (HAS_CPP11_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(FATAL_ERROR "Unsupported compiler. At least C++11 support is required.")
    endif()

	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
	endif()
endif()

file(GLOB_RECURSE Cendric_FILES
	"${PROJECT_SOURCE_DIR}/include/*.h"
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/ext/Particles/*.cpp"
	"${PROJECT_SOURCE_DIR}/ext/lua/src/*.c"
	"${PROJECT_SOURCE_DIR}/ext/sqlite/sqlite3.c"
	"${PROJECT_SOURCE_DIR}/ext/tinyxml2/*.cpp"
    if (CENDRIC_STEAM)
        "${PROJECT_SOURCE_DIR}/ext/steam-sdk/public/steam/*.h"
        "${PROJECT_SOURCE_DIR}/ext/steam-sdk/public/steam/*.cpp"
    endif()
)

if (WIN32)
    # Windows

	add_executable(Cendric
		${Cendric_FILES}
		${PROJECT_SOURCE_DIR}/res/info.rc
	)

    if (CENDRIC_STEAM)
        target_compile_definitions(Cendric PRIVATE STEAM)
        target_link_libraries(Cendric ${PROJECT_SOURCE_DIR}/ext/steam-sdk/redistributable_bin/win64/steam_api64.lib)
    endif()

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # macOS

	add_definitions("-DAPPLE_APP_BUILD")

	add_executable(Cendric MACOSX_BUNDLE
		${Cendric_FILES}
		${PROJECT_SOURCE_DIR}/res/macOS/icon.icns
	)

    # mac
    if (CENDRIC_STEAM)
        add_definitions("-DSTEAM")
        target_link_libraries(Cendric ${PROJECT_SOURCE_DIR}/ext/steam-sdk/redistributable_bin/osx32/libsteam_api.dylib)
        file(COPY ${PROJECT_SOURCE_DIR}/ext/steam-sdk/redistributable_bin/osx32/libsteam_api.dylib
             DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Cendric.app/Contents/MacOS)
        file(COPY ${PROJECT_SOURCE_DIR}/res/macOS/steam_appid.txt
             DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Cendric.app/Contents/MacOS)
    endif()

    add_library(CendricApple STATIC "${PROJECT_SOURCE_DIR}/src/Platform/CendricApple.mm")
    target_include_directories(CendricApple PRIVATE "${PROJECT_SOURCE_DIR}/include")
    set_target_properties(CendricApple PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries(Cendric CendricApple)
else()
    # Linux

	add_executable(Cendric
		${Cendric_FILES}
	)

    if (CENDRIC_STEAM)
        target_compile_definitions(Cendric PRIVATE STEAM)
        target_link_libraries(Cendric ${PROJECT_SOURCE_DIR}/ext/steam-sdk/redistributable_bin/linux64/libsteam_api.so)
    endif()
endif()

if (CENDRIC_EXTERNAL_DOCUMENT_FOLDER)
    add_definitions("-DEXTERNAL_DOCUMENTS_FOLDER")
endif()

if (CENDRIC_GERMAN)
    add_definitions("-DGERMAN_DEFAULT_LANGUAGE")
endif()

set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -lpthread -ldl")

target_link_libraries(Cendric sfml-system sfml-window sfml-graphics sfml-audio)

target_include_directories(Cendric PRIVATE
	"${PROJECT_SOURCE_DIR}/ext/sfml/include"
	"${PROJECT_SOURCE_DIR}/ext"
	"${PROJECT_SOURCE_DIR}/include"
)

if (APPLE)
	set_target_properties(Cendric PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/res/macOS/info.plist)
	set_source_files_properties(res/macOS/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
	file(COPY cendric.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Cendric.app/Contents/Resources)
	file(COPY db/game_data.db DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Cendric.app/Contents/Resources/db)
	file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Cendric.app/Contents/Resources)
	file(COPY saves DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Cendric.app/Contents/Resources)
endif()

if (WIN32 AND CENDRIC_BUILD_SQLITE_SHELL)

	file(GLOB_RECURSE SQLiteShell_FILES
		"${PROJECT_SOURCE_DIR}/ext/sqlite/*.c"
		"${PROJECT_SOURCE_DIR}/ext/sqlite/*.h"
	)

	add_executable(SQLiteShell ${SQLiteShell_FILES})

endif()

if (WIN32 AND CENDRIC_BUILD_DIALOGUE_TOOL)

	file(GLOB_RECURSE DialogueTool_FILES
		"${PROJECT_SOURCE_DIR}/tools/DialogueTool/include/*.h"
		"${PROJECT_SOURCE_DIR}/tools/DialogueTool/src/*.cpp"
		"${PROJECT_SOURCE_DIR}/ext/imgui/imgui.cpp"
		"${PROJECT_SOURCE_DIR}/ext/imgui/imgui_draw.cpp"
		"${PROJECT_SOURCE_DIR}/ext/imgui/imgui_demo.cpp"
		"${PROJECT_SOURCE_DIR}/ext/imgui-sfml/imgui-SFML.cpp"
	)

	add_executable(DialogueTool ${DialogueTool_FILES})

	target_link_libraries(DialogueTool sfml-system sfml-graphics sfml-window)

	target_include_directories(DialogueTool PRIVATE
		"${PROJECT_SOURCE_DIR}/ext/sfml/include"
		"${PROJECT_SOURCE_DIR}/ext/imgui"
		"${PROJECT_SOURCE_DIR}/ext/imgui-sfml"
		"${PROJECT_SOURCE_DIR}/ext"
		"${PROJECT_SOURCE_DIR}/tools/DialogueTool/include"
	)

endif()
