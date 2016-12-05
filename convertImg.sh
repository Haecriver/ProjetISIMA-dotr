#!/bin/bash

if [ -d "$1" ] 
then
	IMG_PATH="$1"
else
	IMG_PATH="."	
fi
imgs=`ls $IMG_PATH/*.jpg`

a=1
for old in $imgs; do
	new=$(printf "in_%04d.jpg" "$a") #04 pad to length of 4
	echo "mv $old $new"  
	mv "$old" "$IMG_PATH/$new"
	let a=a+1
done
