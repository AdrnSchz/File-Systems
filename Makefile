all: demo

ext2.o: ext2.h ext2.c
	gcc -Wall -Wextra -g -c ext2.c -o ext2.o

fat16.o: fat16.h fat16.c
	gcc -Wall -Wextra -g -c fat16.c -o fat16.o

print.o: print.h print.c ext2.o fat16.o
	gcc -Wall -Wextra -g -c print.c -o print.o

fsutils: fsutils.c print.o ext2.o fat16.o
	gcc -Wall -Wextra -g fsutils.c print.o ext2.o fat16.o -o fsutils

demo: fsutils

clean:
	rm -fr *.o fsutils
