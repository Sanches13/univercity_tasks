#!/usr/bin/env bash

if [[ -f "sum_out.txt" ]]
#start the cycle
#checking for the absence of the file "sum_out.txt"
#if true
then
#write Error
	echo "Error"
else
#trransfer the contents of all txt files to file sum_out.txt
		cat *.txt > sum_out.txt
#finish the cycle
		fi


