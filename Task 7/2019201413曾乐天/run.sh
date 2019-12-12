#!/bin/sh

echo "start."
sudo make
while read -p "enter running type:(0:Normal, 1:Interposition):" ty 
do
	if [ "$ty" = 0 ]; then
		./BST
		break
	elif [ "$ty" = 1 ]; then
		LD_PRELOAD="./mymalloc.so" ./BST 
		break
	else
		continue
	fi
done
sudo make clean

echo "all done."
