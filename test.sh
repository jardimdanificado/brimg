#!/bin/bash
tcc -o main main.c
rm test.img
bruter serialize.br test.txt test.img
bruter deserialize.br test.img test.braw
./main test.img