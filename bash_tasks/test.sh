#!/usr/bin/env bash

#check the existence of a text file, as well as two programs
if [[ -f "file.txt" ]]
then
    echo "File exist!"
    fi
if [[ -f "c_version.c" ]]
then
    echo "C-program exists!"
    fi
if [[ -f "p_version.py" ]]
then
    echo "Python program exists!"
    fi
#compile the program on C
gcc c_version.c
echo "Time of c-program: "
#program C execution time
time ./a.out
echo "Time of python program: "
#program C execution time
time ./p_version.py
