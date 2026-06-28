#! /usr/bin/sh

if [ -d build ]
then
  echo "Removing build directory..."
  rm -rf ./build/
fi

cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug

cd build/

if [ -e compile_commands.json ]
then
  mv compile_commands.json ..
else
  echo "ERROR: compile_commands.json not found in directory:"
  pwd
fi
