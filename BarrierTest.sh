#!/bin/bash
cd Barriers
rm *.dat
sample=$(seq $2 $3 $4)
LOOP=$(seq 1 10)

make Sense
make MSense
make Tree
make MTree
make Static
make MStatic 

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

	echo Sense
	for n in $sample
		do 
			echo $n 
				./LSense  -n $n>>LSeB$loop.dat
		done
	echo MSense
	for n in $sample
		do 
			echo $n 
				./LMSense  -n $n>>LMSeB$loop.dat
		done
	echo Tree
	for n in $sample
		do 
			echo $n 
				./LTree  -n $n -f $1>>LTB$loop.dat
		done
	echo Mtree
	for n in $sample
		do 
			echo $n 
				./LMTree  -n $n -f $1>>LMTB$loop.dat
		done

	echo Static
	for n in $sample
		do 
			echo $n 
				./LStatic  -n $n -f $1>>LStB$loop.dat
		done
	echo MStatic
	for n in $sample
		do 
			echo $n 
				./LMStatic -n $n -f $1>>LMStB$loop.dat
		done

	done
make clean

cd ../IB
rm *.dat
sample=$(seq $2 $3 $4)
LOOP=$(seq 1 10)

make Sense
make MSense
make Tree
make MTree
make Static
make MStatic 

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

	echo Sense
	for n in $sample
		do 
			echo $n 
				./LSense  -n $n>>LSeB$loop.dat
		done
	echo MSense
	for n in $sample
		do 
			echo $n 
				./LMSense  -n $n>>LMSeB$loop.dat
		done
	echo Tree
	for n in $sample
		do 
			echo $n 
				./LTree  -n $n -f $1>>LTB$loop.dat
		done
	echo Mtree
	for n in $sample
		do 
			echo $n 
				./LMTree  -n $n -f $1>>LMTB$loop.dat
		done

	echo Static
	for n in $sample
		do 
			echo $n 
				./LStatic  -n $n -f $1>>LStB$loop.dat
		done
	echo MStatic
	for n in $sample
		do 
			echo $n 
				./LMStatic -n $n -f $1>>LMStB$loop.dat
		done

	done
make clean




