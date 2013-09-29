#!/bin/bash

while read line; do
	echo $line
	arr=(${line})
	python ${arr[0]}.py "-n" ${arr[2]} ${arr[3]} ${arr[4]} 
done < $1
	
