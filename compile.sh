#!/bin/bash
echo "compiling $1"
if [[ $1 == *.cpp ]]
then
    dir=$(dirname "$1")
    cd $dir
    filename=$(basename "$1")
    g++ -std=c++11 -ggdb `pkg-config --cflags opencv` -o `basename $filename .cpp` $filename `pkg-config --libs opencv`
else
    echo "Not a CPP file"
fi
echo "Output file: ${1%.*}"
