all:
	gcc -Wall -c common.c
	gcc -Wall -c handles.c 
	gcc -Wall cliente.c common.o -o cliente
	gcc -Wall servidor.c common.o handles.o -o servidor -lm
	gcc -Wall servidor-mt.c common.o -lpthread -o servidor-mt
clean:
	rm common.o cliente servidor servidor-mt
