main: main.o merge.o test.o
	gcc -o main main.o merge.o test.o

main.o: main.c
	gcc -Wall -c main.c

merge.o: merge.c
	gcc -Wall -c merge.c

test.o: test.c
	gcc -Wall -c test.c

clean:
	rm main.o merge.o test.o main
