CC = g++
CFLAGS = -std=c++17 -Wall -Werror -Wextra -Wpedantic -g -Ofast#$(shell pkg-config --cflags)

all: Arithmetic

Arithmetic: List.o BigInteger.o Arithmetic.o
	$(CC) $(CFLAGS) -o $@ $^ 

BigInteger: List.o BigInteger.o 
	$(CC) $(CFLAGS) -o $@ $^ 

BigIntegerTest: List.o BigIntegerTest.o BigInteger.o 
	$(CC) $(CFLAGS) -o  $@ $^ 


%.o: %.c
	$(CC) $(CFLAGS) -c -Ofast 

clean:
	rm -f *.o
	rm -f Arithmetic
	rm -f BigIntegerTest

