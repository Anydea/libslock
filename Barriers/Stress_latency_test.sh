#!/bin/bash
rm L*.dat
sample=$(seq $2 $3 $4)
LOOP=$(seq 1 10)
make LSense
make LMSense
make LTree
make LMTree
make LStatic
make LMStatic 

for loop in $LOOP
	do
	echo Sense
	for n in $sample
		do 
			echo $n 
				./Sense  -n $n>>LSeB$loop.dat
		done
	echo MSense
	for n in $sample
		do 
			echo $n 
				./MSense  -n $n>>LMSeB$loop.dat
		done
	echo Tree
	for n in $sample
		do 
			echo $n 
				./Tree  -n $n -f $1>>LTB$loop.dat
		done
	echo Mtree
	for n in $sample
		do 
			echo $n 
				./MTree  -n $n -f $1>>LMTB$loop.dat
		done

	echo Static
	for n in $sample
		do 
			echo $n 
				./Static  -n $n -f $1>>LStB$loop.dat
		done
	echo MStatic
	for n in $sample
		do 
			echo $n 
				./MStatic -n $n -f $1>>LMStB$loop.dat
		done

	done
#gunplot latencyplot
make clean




