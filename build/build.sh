#!/bin/bash
rm -rf release
mkdir release

tcc -o release/brimg ../src/brimg.c ../src/main.c

cp ../src/brimg.lua ./release/brimg.lua
cp brimgt ./release/brimgt
chmod +x ./release/brimgt
cd ..

# tests
rm example/disk0.img

brimgt example/disk0.txt --output example/disk0.img --serialize
brimgt example/disk0.img --output example/disk0.braw --deserialize

cd example
brimg disk0.img