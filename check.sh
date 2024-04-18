#!/bin/bash

EXE=./main

if [ $# -gt 0 ]; then
	EXE="$1"
fi

for f in *.in; do
    echo $f
	$EXE < $f
done
