#!/bin/bash
if [ $# -eq 0 ]; then
	for f in *.c
	do
		f=`echo "$f" | cut -d'.' -f1`
		echo "gcc -g -shared -fPIC $f.c -o ../lib/lib$f.so"
		gcc -g -shared -fPIC $f.c -o ../lib/lib$f.so
	done
	rm ../lib/libpash.so
elif [ $# -gt 0 ]; then 
	f=`echo "$1" | cut -d'.' -f1`
	echo "gcc -g -shared -fPIC $f.c -o ../lib/lib$f.so"
	gcc -g -shared -fPIC $f.c -o ../lib/lib$f.so
fi
