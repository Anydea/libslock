#!/bin/bash

for n in {20..200..20}
	do 
		echo $n
		for r in {10..100..10}
			do	
				./stress_test.o -r $r -n $n >> StaticTreeB.txt
			done
	done


