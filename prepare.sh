#!/bin/bash

if [ $# -lt 1 ]; then
	echo "number of problems not specified"
	exit 0
fi

cnt=$1

mkdir temp
mv main.cpp temp
mv check.sh temp

for x in {a..z}; do
	cp -r temp $x
	cnt=$((cnt-1))
	if [ $cnt -eq 0 ]; then
		break
	fi
done
