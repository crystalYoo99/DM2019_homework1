quiz : cross.o
	gcc -o quiz cross.o

cross.o : cross.c
	gcc -c -o cross.o cross.c
clean :
	rm *.o quiz formula
