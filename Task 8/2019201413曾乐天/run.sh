#!/bin/sh

echo "start."
gcc score.c -o score
vk=(0 4 8 64 8 8 8 8 8)

for ((i = 1; i <= 8; i ++))
do
	mv "$i".in input.bin
	gcc main.c -o main -DK=${vk[i]}
	./main
	printf "case $i:\n"
	if diff com_log.txt dec_log.txt;then
		./score
		mv input.bin "$i".in 
		mv output.bin input.bin
		./main
		if diff input.bin output.bin;then
			rm input.bin
			printf "AC\n"
		else
			rm input.bin
			printf "WA\n"
			exit 0
		fi
	else
		mv input.bin "$i".in 
		printf "WA\n"
		exit 0
	fi
done

echo "done."
