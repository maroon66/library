#!/bin/bash

EXE=./main

if [ $# -gt 0 ]; then
	EXE="$1"
fi

for f in *.in; do
    echo $f
	extension="${f##*.}"
	filename="${f%.*}"
	outpath="$filename.${extension//in/out}"
	anspath="$filename.${extension//in/ans}"
	$EXE < $f > $outpath
	diff $outpath $anspath
done
