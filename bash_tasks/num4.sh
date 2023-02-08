#!/usr/bin/env bash

for((i=0;i<=100000;i++))
#start a cycle with a enumeration of numbers
do
#if then subtracting 943, the original number when divided by 1000 gives the remainder 0
	if ((($i-943)%1000==0))
        #if remainder 0
	then
        #print this number
			echo "$i"
        #finish the cycle
	fi
#finish the cycle		
done
