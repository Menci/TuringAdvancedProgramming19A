ojbk = lst.o jose.o

jose_run : $(ojbk)
	gcc -o jose_run $(ojbk) -lm

lst.o : head_lst.h
jose.o : head_lst.h

.PHONY : clean
clean :
	rm -f jose_run $(ojbk)
