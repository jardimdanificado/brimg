#!/bin/bash
tcc -o main main.c -ltcc
rm test.img
bruter tools/serializer.br test.txt test.img
bruter tools/deserializer.br test.img test.braw
bruter tools/structer.br structs.br structs.h
./main test.c