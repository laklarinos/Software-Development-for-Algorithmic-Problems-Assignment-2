CC = g++ -g -O2 -std=c++11 

cluster: main.o parsinginit.o hashtable.o hashtablehyper.o linkedlist.o others.o cluster.o

	$(CC) -o cluster main.o parsinginit.o hashtable.o hashtablehyper.o linkedlist.o others.o cluster.o

cluster.o: cluster.cpp hashtable.cpp hashtablehyper.cpp

	$(CC) -c cluster.cpp hashtable.cpp hashtablehyper.cpp

main.o: main.cpp others.cpp parsingInit.cpp hashtable.cpp hashtablehyper.cpp

	$(CC) -c main.cpp others.cpp parsingInit.cpp hashtable.cpp hashtablehyper.cpp

others.o: others.cpp

	$(CC) -c others.cpp

parsinginit.o: parsingInit.cpp others.cpp

	$(CC) -c parsingInit.cpp others.cpp

hashtable.o: hashtable.cpp others.cpp linkedlist.cpp 

	$(CC) -c hashtable.cpp others.cpp linkedlist.cpp

hashtablehyper.o: hashtablehyper.cpp others.cpp linkedlist.cpp 

	$(CC) -c hashtablecube.cpp others.cpp linkedlist.cpp

linkedlist.o: linkedlist.cpp others.cpp

	$(CC) -c linkedlist.cpp others.cpp

run:

	./cluster –i input_small_id –c cluster.conf -o output.txt -Complete yes -m HYPER

runlsh:

	./cluster –i input_small_id –c cluster.conf -o output.txt -Complete yes -m LSH

runhyper:

	./cluster –i input_small_id –c cluster.conf -o output.txt -Complete yes -m HYPER

runclassic:

	./cluster –i input_small_id –c cluster.conf -o output.txt -Complete yes -m Classic

valgrind:

	valgrind --leak-check=yes ./cluster –i input_small_id –c cluster.conf -o output.txt -Complete yes -m HYPER

clean: 

	rm -f *.o

	
	

