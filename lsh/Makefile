CC = g++ -g -std=c++11 

lsh: main.o parsinginit.o hashtable.o linkedlist.o others.o point.o

	$(CC) -o lsh main.o parsinginit.o hashtable.o linkedlist.o others.o point.o -lm

main.o: main.cpp others.cpp parsinginit.cpp hashtable.cpp point.cpp

	$(CC) -c main.cpp others.cpp parsinginit.cpp hashtable.cpp point.cpp

others.o: others.cpp

	$(CC) -c others.cpp

point.o: point.cpp

	$(CC) -c point.cpp

parsinginit.o: parsinginit.cpp others.cpp

	$(CC) -c parsinginit.cpp others.cpp

hashtable.o: hashtable.cpp others.cpp linkedlist.cpp

	$(CC) -c hashtable.cpp others.cpp linkedlist.cpp

linkedlist.o: linkedlist.cpp others.cpp

	$(CC) -c linkedlist.cpp others.cpp

run:

	./lsh -i input_file -q query_small_id -k 10 -L 10 -o output.txt -N 5 -R 500

valgrind:

	valgrind --leak-check=yes ./lsh -i input_file -q query_small_id -k 10 -L 10 -o output.txt -N 5 -R 100

clean: 

	rm -f *.o

	
	

