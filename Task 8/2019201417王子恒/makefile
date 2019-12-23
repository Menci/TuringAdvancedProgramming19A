K = 4
run1 : 1.cc zipper.c first_unzip.c second_unzip.c checker.c
	@g++ 1.cc -o maker -std=c++1z
	@./maker
	@gcc zipper.c -DK=$(K) -o zipper
	@./zipper
	@gcc first_unzip.c -DK=$(K) -o first_unzip
	@./first_unzip
	@gcc second_unzip.c -DK=$(K) -o second_unzip
	@./second_unzip
	@gcc checker.c -DK=$(K) -o checker
	@./checker
run2 : 2.cc zipper.c first_unzip.c second_unzip.c checker.c
	@g++ 2.cc -o maker -std=c++1z
	@./maker
	@gcc zipper.c -DK=$(K) -o zipper
	@./zipper
	@gcc first_unzip.c -DK=$(K) -o first_unzip
	@./first_unzip
	@gcc second_unzip.c -DK=$(K) -o second_unzip
	@./second_unzip
	@gcc checker.c -DK=$(K) -o checker
	@./checker
run3 : 3.cc zipper.c first_unzip.c second_unzip.c checker.c
	@g++ 3.cc -o maker -std=c++1z
	@./maker
	@gcc zipper.c -DK=$(K) -o zipper
	@./zipper
	@gcc first_unzip.c -DK=$(K) -o first_unzip
	@./first_unzip
	@gcc second_unzip.c -DK=$(K) -o second_unzip
	@./second_unzip
	@gcc checker.c -DK=$(K) -o checker
	@./checker
run4 : 4.cc zipper.c first_unzip.c second_unzip.c checker.c
	@g++ 4.cc -o maker -std=c++1z
	@./maker
	@gcc zipper.c -DK=$(K) -o zipper
	@./zipper
	@gcc first_unzip.c -DK=$(K) -o first_unzip
	@./first_unzip
	@gcc second_unzip.c -DK=$(K) -o second_unzip
	@./second_unzip
	@gcc checker.c -DK=$(K) -o checker
	@./checker
run5 : 5.cc zipper.c first_unzip.c second_unzip.c checker.c
	@g++ 5.cc -o maker -std=c++1z
	@./maker
	@gcc zipper.c -DK=$(K) -o zipper
	@./zipper
	@gcc first_unzip.c -DK=$(K) -o first_unzip
	@./first_unzip
	@gcc second_unzip.c -DK=$(K) -o second_unzip
	@./second_unzip
	@gcc checker.c -DK=$(K) -o checker
	@./checker
run6 : 6.cc zipper.c first_unzip.c second_unzip.c checker.c
	@g++ 6.cc -o maker -std=c++1z
	@./maker
	@gcc zipper.c -DK=$(K) -o zipper
	@./zipper
	@gcc first_unzip.c -DK=$(K) -o first_unzip
	@./first_unzip
	@gcc second_unzip.c -DK=$(K) -o second_unzip
	@./second_unzip
	@gcc checker.c -DK=$(K) -o checker
	@./checker
run7 : 7.cc zipper.c first_unzip.c second_unzip.c checker.c
	@g++ 7.cc -o maker -std=c++1z
	@./maker
	@gcc zipper.c -DK=$(K) -o zipper
	@./zipper
	@gcc first_unzip.c -DK=$(K) -o first_unzip
	@./first_unzip
	@gcc second_unzip.c -DK=$(K) -o second_unzip
	@./second_unzip
	@gcc checker.c -DK=$(K) -o checker
	@./checker
run8 : 8.cc zipper.c first_unzip.c second_unzip.c checker.c
	@g++ 8.cc -o maker -std=c++1z
	@./maker
	@gcc zipper.c -DK=$(K) -o zipper
	@./zipper
	@gcc first_unzip.c -DK=$(K) -o first_unzip
	@./first_unzip
	@gcc second_unzip.c -DK=$(K) -o second_unzip
	@./second_unzip
	@gcc checker.c -DK=$(K) -o checker
	@./checker
special : zipped.bin zipper.c first_unzip.c second_unzip.c checker.c
	@gcc zipper.c -DK=$(K) -o zipper
	@./zipper
	@gcc first_unzip.c -DK=$(K) -o first_unzip
	@./first_unzip
	@gcc second_unzip.c -DK=$(K) -o second_unzip
	@./second_unzip
	@gcc checker.c -DK=$(K) -o checker
	@./checker
clean :
	rm -r first_unzip second_unzip zipper checker maker\
		first_unzip.out second_unzip.out zipped.bin my_zip.bin
