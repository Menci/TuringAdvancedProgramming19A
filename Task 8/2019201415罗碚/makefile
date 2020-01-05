main: clean
	gcc -o main main.c errhand.c -Ofast -Wall
tree: 
	gcc -o /tmp/tree-c main-c.c tree.c bitio.c errhand.c -DN=${N} -DK=${K} -Ofast -Wall
	gcc -o /tmp/tree-e main-e.c tree.c bitio.c errhand.c -DN=${N} -DK=${K} -Ofast -Wall
arith:
	gcc -o /tmp/arith-c main-c.c arith.c bitio.c errhand.c -Ofast -Wall
	gcc -o /tmp/arith-e main-e.c arith.c bitio.c errhand.c -Ofast -Wall
lzw:
	gcc -o /tmp/lzw-c main-c.c lzw.c bitio.c errhand.c -Ofast -Wall
	gcc -o /tmp/lzw-e main-e.c lzw.c bitio.c errhand.c -Ofast -Wall
clean:
	rm -rf /tmp/tree-c /tmp/tree-e /tmp/arith-c /tmp/arith-e /tmp/lzw-c /tmp/lzw-e main
