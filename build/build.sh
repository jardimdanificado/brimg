#!/bin/bash
rm -rf release
mkdir release

tcc -o release/brimgc ../src/brimg.c -ltcc
bruter build.br
cp brimgt ./release/brimgt
chmod +x ./release/brimgt
cd ..

# tests
rm example/test.img
brimgt example/test.txt --output example/test.img --serialize
brimgt example/test.img --output example/test.braw --deserialize

cd example
brimgt test.br test.c
brimgc test.c