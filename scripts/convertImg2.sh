#!/bin/bash

if [ -d "$1" ] 
then
	IMG_PATH="$1"
else
	IMG_PATH="."	
fi

a=1
for a in {1..263}; do
	new=$(printf "in_%04d.jpg" "$a") #04 pad to length of 4
	echo "mv \ \($a\).jpg $new"  
	mv $IMG_PATH\ \($a\).jpg $IMG_PATH/$new
done
