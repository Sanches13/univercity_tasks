#!/usr/bin/env bash

N=5
#start the cycle of the checking of the directories with a multiple of 5
for((i=5;i<100;i=i+5))
do
#checking for the absence of the directory with "i" number
	if [[ -d /path/dir$i/ ]]
        #start the cycle
        #if the directory is missing
	then
        #finish the cycle
		break
	else
        #delete the directory with name diri, where "i" is number of directory
                rmdir dir$i
        #finish the cycle
	fi
#finish the cycle
done	
	    
		
