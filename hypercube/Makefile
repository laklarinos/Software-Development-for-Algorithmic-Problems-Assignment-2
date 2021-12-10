CC = g++ -g -std=c++11 

cube: main.o parsinginit.o hashtable.o linkedlist.o others.o point.o

	$(CC) -o cube main.o parsinginit.o hashtable.o linkedlist.o others.o point.o -lm

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

	./cube -i input_file -q query_small_id -k 14 -M 10 -probes 2 -o output.txt -N 5 -R 10000

valgrind:

	valgrind --leak-check=full 	./cube -i input_file -q query_small_id -k 14 -M 10 -probes 2 -o output.txt -N 5 -R 10000

clean: 

	rm -f *.o

	
	 

