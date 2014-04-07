#!/bin/bash

for n in {20..200..2}
	do 
		echo $n
		for r in {10..100..2}
			do	
				./stress_test.o -r $r -n $n >> StaticTreeB.txt
			done
	done


