#!/bin/bash
rm *.dat
sample=$(seq $2 $3 $4)
LOOP=$(seq 1 10)

make Sense
make MSense
make Tree
make MTree
make Static
make MStatic 

for loop in $LOOP
	do
	echo Sense
	for n in $sample
		do 
			echo $n 
				./Sense  -n $n>>SeB$loop.dat
		done
	echo Msense
	for n in $sample
		do 
			echo $n 
				./MSense  -n $n>>MSeB$loop.dat
		done
	echo Tree
	for n in $sample
		do 
			echo $n 
				./Tree  -n $n -f $1>>TB$loop.dat
		done
	echo MTree
	for n in $sample
		do 
			echo $n 
				./MTree  -n $n -f $1>>MTB$loop.dat
		done
	echo Static
	for n in $sample
		do 
			echo $n 
				./Static  -n $n -f $1>>StB$loop.dat
		done
	echo MStatic
	for n in $sample
		do 
			echo $n 
				./MStatic -n $n -f $1>>MStB$loop.dat
		done


	done

make clean

#gnuplot stressplot




