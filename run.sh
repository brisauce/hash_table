#! /usr/bin/sh

PROGRAM_NAME='hash_table'
TEXT_FILE_NAME="text.txt"


run_program () {
  ./$PROGRAM_NAME $TEXT_FILE_NAME --goof
}

if [ ! -d build ]; then
  echo "Build directory not found. Please run configure.sh to generate build files."
  exit 1
fi

if [ ! -f $TEXT_FILE_NAME ]; then
  echo "$TEXT_FILE_NAME not found"
  exit 1
else
  cp $TEXT_FILE_NAME ./build
fi

cd build

if [ -e $PROGRAM_NAME ]; then
  run_program
else
  echo "$PROGRAM_NAME not found. Please run build.sh."
fi
