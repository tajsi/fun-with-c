#!/usr/bin/sh

gcc -m64 -O2 -o bin/test_length test_length.c
gcc -m64 -O2 -o bin/test_regex test_regex.c
gcc -m64 -O2 -o bin/test test.c
