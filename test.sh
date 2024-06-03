#!/bin/bash
gcc -o main main.c
rm test.img
bruter serialize.br test.txt test.img
./main test.img