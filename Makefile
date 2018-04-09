poker:  cephes.c poker.c 
	gcc  -o poker -lm cephes.c poker.c

hexdump: hexdump.c
	gcc -o hexdump hexdump.c

clean:
	rm poker
