#!/bin/bash
rm Result.txt
rm L*.dat
sample=$(seq $2 $3 $4)

make LSense
#echo SenseBarrier>>Result.txt
for n in $sample
	do 
		echo $n 
			./Sense  -n $n>>LSeB.dat
	done

make LMSense
#echo SenseBarrier with less cores>>Result.txt
for n in $sample
	do 
		echo $n 
			./MSense  -n $n>>LMSeB.dat
	done

make LTree
#echo TreeBarrier>>Result.txt
for n in $sample
	do 
		echo $n 
			./Tree  -n $n -f $1>>LTB.dat
	done

make LMTree
#echo TreeBarrier with less cores>>Result.txt
for n in $sample
	do 
		echo $n 
			./MTree  -n $n -f $1>>LMTB.dat
	done


make LStatic
#echo StaticTreeBarrier>>Result.txt
for n in $sample
	do 
		echo $n 
			./Static  -n $n -f $1>>LStB.dat
	done

make LMStatic 
#echo StaticTreeBarrier>>Result.txt
for n in $sample
	do 
		echo $n 
			./MStatic -n $n -f $1>>LMStB.dat
	done

gunplot latencyplot
rm LSense LMSense LTree LMTree LStatic LMStatic





