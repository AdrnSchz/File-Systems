all: demo

ext2.o: ext2.h ext2.c
	gcc -Wall -Wextra -g -c ext2.c -o ext2.o

print.o: print.h print.c ext2.o
	gcc -Wall -Wextra -g -c print.c -o print.o

fsutils: fsutils.c print.o
	gcc -Wall -Wextra -g -c fsutils.c -o fsutils

demo: fsutils

clean:
	rm -fr *.o