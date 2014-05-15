#!/bin/bash

if [ -e file ]
then
	rm -r file
fi

mkdir file
cd file

for i in {0..99}
do
	mkdir "file$i"
	cd "file$i"
	j=0

	while [ $j -le $i ]
	do
		touch "file$j.txt"
		j=$(($j+1))
	done

	cd ..

done
cd ..

