#!/bin/bash

APP_PATH=$1
EXT_PATH=$2
EXT_BUILD_PATH=$3

echo .app path: ${APP_PATH}
echo ext path: ${EXT_PATH}
echo ext build path: ${EXT_BUILD_PATH}

mkdir ${APP_PATH}/Contents/Frameworks

cp -r ${EXT_BUILD_PATH}/sfml/lib/sfml-system.framework ${APP_PATH}/Contents/Frameworks/
cp -r ${EXT_BUILD_PATH}/sfml/lib/sfml-window.framework ${APP_PATH}/Contents/Frameworks/
cp -r ${EXT_BUILD_PATH}/sfml/lib/sfml-graphics.framework ${APP_PATH}/Contents/Frameworks/
cp -r ${EXT_BUILD_PATH}/sfml/lib/sfml-audio.framework ${APP_PATH}/Contents/Frameworks/

cp -r ${EXT_PATH}/sfml/extlibs/libs-osx/Frameworks/FLAC.framework ${APP_PATH}/Contents/Frameworks/
cp -r ${EXT_PATH}/sfml/extlibs/libs-osx/Frameworks/freetype.framework ${APP_PATH}/Contents/Frameworks/
cp -r ${EXT_PATH}/sfml/extlibs/libs-osx/Frameworks/ogg.framework ${APP_PATH}/Contents/Frameworks/
cp -r ${EXT_PATH}/sfml/extlibs/libs-osx/Frameworks/vorbis.framework ${APP_PATH}/Contents/Frameworks/
cp -r ${EXT_PATH}/sfml/extlibs/libs-osx/Frameworks/vorbisenc.framework ${APP_PATH}/Contents/Frameworks/
cp -r ${EXT_PATH}/sfml/extlibs/libs-osx/Frameworks/vorbisfile.framework ${APP_PATH}/Contents/Frameworks/

install_name_tool -change "@rpath/sfml-system.framework/Versions/2.4.2/sfml-system" "@loader_path/../Frameworks/sfml-system.framework/Versions/2.4.2/sfml-system" ${APP_PATH}/Contents/MacOS/Cendric
install_name_tool -change "@rpath/sfml-window.framework/Versions/2.4.2/sfml-window" "@loader_path/../Frameworks/sfml-window.framework/Versions/2.4.2/sfml-window" ${APP_PATH}/Contents/MacOS/Cendric
install_name_tool -change "@rpath/sfml-graphics.framework/Versions/2.4.2/sfml-graphics" "@loader_path/../Frameworks/sfml-graphics.framework/Versions/2.4.2/sfml-graphics" ${APP_PATH}/Contents/MacOS/Cendric
install_name_tool -change "@rpath/sfml-audio.framework/Versions/2.4.2/sfml-audio" "@loader_path/../Frameworks/sfml-audio.framework/Versions/2.4.2/sfml-audio" ${APP_PATH}/Contents/MacOS/Cendric

install_name_tool -change "@rpath/../Frameworks/freetype.framework/Versions/A/freetype" "@loader_path/../Frameworks/freetype.framework/Versions/A/freetype" ${APP_PATH}/Contents/MacOS/Cendric
install_name_tool -change "@rpath/../Frameworks/vorbisenc.framework/Versions/A/vorbisenc" "@loader_path/../Frameworks/vorbisenc.framework/Versions/A/vorbisenc" ${APP_PATH}/Contents/MacOS/Cendric
install_name_tool -change "@rpath/../Frameworks/vorbisfile.framework/Versions/A/vorbisfile" "@loader_path/../Frameworks/vorbisfile.framework/Versions/A/vorbisfile" ${APP_PATH}/Contents/MacOS/Cendric
install_name_tool -change "@rpath/../Frameworks/vorbis.framework/Versions/A/vorbis" "@loader_path/../Frameworks/vorbis.framework/Versions/A/vorbis" ${APP_PATH}/Contents/MacOS/Cendric
install_name_tool -change "@rpath/../Frameworks/ogg.framework/Versions/A/ogg" "@loader_path/../Frameworks/ogg.framework/Versions/A/ogg" ${APP_PATH}/Contents/MacOS/Cendric
install_name_tool -change "@rpath/../Frameworks/FLAC.framework/Versions/A/FLAC" "@loader_path/../Frameworks/FLAC.framework/Versions/A/FLAC" ${APP_PATH}/Contents/MacOS/Cendric
