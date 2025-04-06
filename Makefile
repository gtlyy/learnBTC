LIBS256=-lsecp256k1 -lssl -lcrypto -lcunit

all:
	make test
	make create_address
	make rand_pri_key
	make rand_pri_address
	make crack
	make sign_string
	
test:
	make test_mybtc

test_mybtc: test_mybtc.o mybtc.o
	gcc -o a.out test_mybtc.o mybtc.o ${LIBS256}
	cp -f a.out test_mybtc     

create_address: create_address.o mybtc.o
	gcc -o a.out create_address.o mybtc.o ${LIBS256}
	cp -f a.out create_address

rand_pri_key: rand_pri_key.o mybtc.o
	gcc -o a.out rand_pri_key.o mybtc.o ${LIBS256}
	cp -f a.out rand_pri_key

rand_pri_address: rand_pri_address.o mybtc.o
	gcc -o a.out rand_pri_address.o mybtc.o ${LIBS256}
	cp -f a.out rand_pri_address

crack: crack.o mybtc.o
	gcc -o a.out crack.o mybtc.o ${LIBS256}
	cp -f a.out crack		

sign_string: sign_string.o mybtc.o
	gcc -o a.out sign_string.o mybtc.o ${LIBS256}
	cp -f a.out sign_string

test_mybtc.o: test_mybtc.c
	gcc -c test_mybtc.c	

create_address.o: create_address.c
	gcc -c create_address.c	

mybtc.o: mybtc.c
	gcc -c mybtc.c	

rand_pri_key.o: rand_pri_key.c
	gcc -c rand_pri_key.c	

rand_pri_address.o: rand_pri_address.c
	gcc -c rand_pri_address.c			

crack.o: crack.c
	gcc -c crack.c		

sign_string.o: sign_string.c
	gcc -c sign_string.c

clean:
	rm -f a.out *.o
	rm -f create_address sign_string test_mybtc rand_pri_key crack rand_pri_address
