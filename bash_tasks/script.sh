#!/usr/bin/env bash

if gcc -Wall -std=c99 $1
then
       	./a.out
else
    echo "ok!"	
    echo "compilation ended with code $?"
    fi 
