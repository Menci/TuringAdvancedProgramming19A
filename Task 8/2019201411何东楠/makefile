run : 
	gcc compress.c -o compress && ./compress
	gcc extract.c -o extract && ./extract
	gcc checker.c -o checker && ./checker
clean :
	rm checker extract compress rate_calcer *.out data_zip.bin
check : 
	diff my_extract.out std_extract.out
calc :
	gcc rate_calcer.c -o rate_calcer && ./rate_calcer
