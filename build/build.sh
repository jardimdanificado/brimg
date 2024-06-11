#!/bin/bash
rm -rf release
mkdir release

tcc -o release/brimgc ../src/brimg.c -ltcc
cp ../src/brimg.lua ./release/brimg.lua
cp ../src/brimg.h ./release/brimg.h
cp brimgt ./release/brimgt
chmod +x ./release/brimgt
cd ..

# tests
rm example/disk0.img
rm example/brimg.h
cp src/brimg.h example/brimg.h
brimgt example/disk0.txt --output example/disk0.img --serialize
brimgt example/disk0.img --output example/disk0.braw --deserialize

cd example
brimgt main.c test.c
brimgc test.c