#!/bin/bash

if [ "$1" = "" ]
then
  echo "Usage: $0 <version number (e.g. 1.0.0)>"
  exit
fi

VERSION=$1

echo Build Cendric v${VERSION}
echo "========================="

START_PATH=$PWD

rm -rf ../../../build
mkdir ../../../build

echo ""
echo "Create Github build (EN).."
echo "--------------------------"

cd ../../../build
cmake -DCENDRIC_STEAM=OFF -DCENDRIC_EXTERNAL_DOCUMENT_FOLDER=ON .. && make -j 7
cd ${START_PATH}

zip -r -X "Cendric "${VERSION}" - Github.zip" ../../../build/Cendric.app/ >/dev/null

echo ""
echo "Create Steam build (EN).."
echo "--------------------------"

cd ../../../build
cmake -DCENDRIC_STEAM=ON -DCENDRIC_EXTERNAL_DOCUMENT_FOLDER=ON -DCENDRIC_GERMAN=OFF .. && make -j 7
cd ${START_PATH}

zip -r -X "Cendric "${VERSION}" - Steam (EN).zip" ../../../build/Cendric.app/ >/dev/null

echo ""
echo "Create Steam build (DE).."
echo "--------------------------"

cd ../../../build
cmake -DCENDRIC_STEAM=ON -DCENDRIC_EXTERNAL_DOCUMENT_FOLDER=ON -DCENDRIC_GERMAN=ON .. && make -j 7
cd ${START_PATH}

zip -r -X "Cendric "${VERSION}" - Steam (DE).zip" ../../../build/Cendric.app/ >/dev/null