all: hw1

hw1: hw1.c 
	gcc -o hw1.out hw1.c

clean:
	rm -rf hw1.out formula
