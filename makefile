
#Use GNU compiler
cc = gcc -g
CC = g++ -g

all: lab4

shell: lab4.cpp
	$(CC) -c lab4.cpp

clean:
	rm -f lab4
