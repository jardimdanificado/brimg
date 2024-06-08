#!/bin/bash
rm -rf release
mkdir release

tcc -o release/brimg ../src/brimg.c -ltcc
bruter build.br
cd ..

# tests
rm example/test.img
bruter tools/serializer.br example/test.txt example/test.img
bruter tools/deserializer.br example/test.img example/test.braw
cd example
bruter ../build/release/brimg.br test.br test.c
../build/release/brimg test.c