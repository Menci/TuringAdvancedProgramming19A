all : main_press main_unpress
form_press.o : form_press.c form_press.h
	cc -c form_press.c
read_bin.o : read_bin.c read_bin.h
	cc -c read_bin.c
unpress.o : unpress.c unpress.h
	cc -c unpress.c
write_bin.o : write_bin.c write_bin.h
	cc -c write_bin.c
main_press.o : main_press.c
	cc -c main_press.c
main_press : main_press.o read_bin.o form_press.o write_bin.o
	gcc -o main_press main_press.o read_bin.o form_press.o write_bin.o
main_unpress : main_unpress.o read_bin.o unpress.o
	gcc -o main_unpress main_unpress.o read_bin.o unpress.o
.PHONY : clean
clean : 
	rm form_press.o read_bin.o unpress.o write_bin.o main_press.o main_press main_unpress main_unpress.o -f
