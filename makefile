blockchain.o : blockchain.h blockchain.c
	gcc -c blockchain.c

proofofwork.o : blockchain.h proofofwork.c
	gcc -c proofofwork.c

interface.o : blockchain.h interface.c
	gcc -c interface.c

admin.o : blockchain.h admin.c
	gcc -c admin.c

main.o : main.c blockchain.h
	gcc -c main.c

projet_c2 : main.o blockchain.o proofofwork.o interface.o admin.o
	gcc blockchain.o proofofwork.o  main.o interface.o admin.o -o projet_c2 -lssl -lcrypto

clean :
	rm -f *.o core
