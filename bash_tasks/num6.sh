#!/usr/bin/env bash
#print message
echo "script for compile C language"
if [[ -f "hello.c" ]]
#if a C program exists
then
#if true, print the messages and start compiling the program 
       echo "File exists!"	
       echo "Starting compilation..."
       gcc hello.c
else
#if false, print message 
       echo "ERROR"
#finish the cycle	
       fi
if [[ -f "a.out" ]]
#if compiled successfully
then
#print messages and run the program 
      echo "Compilation sucess! Program name: a.out"
      echo "Starting a.out"
      ./a.out
else
#else print ERROR 
      echo "ERROR"
#finish the cycle	
      fi




