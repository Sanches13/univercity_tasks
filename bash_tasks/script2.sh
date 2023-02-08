#!/usr/bin/env bash

-Wall -std=c99 $1
compilation=$?

if (( $compilation == 0))
then
    echo "ok!"
    ./a.out
else
    echo "compilation ended with code $compilation"
fi
