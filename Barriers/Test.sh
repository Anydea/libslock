#!/bin/bash
rm Result.txt

sample=$(seq $2 $3 $4)

make Sense
echo SenseBarrier>>Result.txt
for n in $sample
	do 
		echo $n 
			./Sense  -n $n>>Result.txt
	done

make MSense
echo SenseBarrier with less cores>>Result.txt
for n in $sample
	do 
		echo $n 
			./MSense  -n $n>>Result.txt
	done

make Tree
echo TreeBarrier>>Result.txt
for n in $sample
	do 
		echo $n 
			./Tree  -n $n -f $1>>Result.txt
	done

make MTree
echo TreeBarrier with less cores>>Result.txt
for n in $sample
	do 
		echo $n 
			./MTree  -n $n -f $1>>Result.txt
	done


make Static
echo StaticTreeBarrier>>Result.txt
for n in $sample
	do 
		echo $n 
			./Static  -n $n -f $1>>Result.txt
	done

make MStatic 
echo StaticTreeBarrier with less cores>>Result.txt
for n in $sample
	do 
		echo $n 
			./MStatic -n $n -f $1>>Result.txt
	done

make clean






