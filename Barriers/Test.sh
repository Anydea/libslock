#!/bin/bash

for n in {2..8..1}
	do 
		echo $n
			./stress_test.o  -n $n -f $1>>StaticTreeBarrier_$1.txt
	done


