#!/bin/bash
tcc -o main main.c -ltcc
rm test.img
bruter tools/serialize.br test.txt test.img
bruter tools/deserialize.br test.img test.braw
./main test.c