temp = main.o build.o struct.o operate.o cmd.o
exe = cal_jose

cal_jose:$(temp)
	gcc -o $(exe) $(temp)

main.o:main.c
	gcc -c main.c  

struct.o:struct.c
	gcc -c struct.c  

operate.o:operate.c
	gcc -c operate.c 

build.o:build.c
	gcc -c build.c 

cmd.o:cmd.c
	gcc -c cmd.c

clean:
	rm $(temp) $(exe)