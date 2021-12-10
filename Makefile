CC = g++ -g -std=c++11 
HEL = helpers/
IMP = importantStructs/
LSH = lsh/

search: main.o checkCommandLine.o parsinginit.o point.o lsh.o others.o hashtable.o linkedlist.o

	$(CC) -o search main.o checkCommandLine.o parsinginit.o point.o lsh.o others.o hashtable.o linkedlist.o

main.o: main.cpp $(HEL)checkCommandLine.cpp $(HEL)parsinginit.cpp $(LSH)lsh.cpp

	$(CC) -c main.cpp $(HEL)checkCommandLine.cpp $(HEL)parsinginit.cpp $(LSH)lsh.cpp

checkCommandLine.o: $(HEL)checkCommandLine.cpp

	$(CC) -c $(HEL)checkCommandLine.cpp

lsh.o: $(LSH)lsh.cpp $(HEL)parsinginit.cpp $(LSH)hashtable.cpp $(IMP)linkedlist.cpp $(LSH)others.cpp $(IMP)point.cpp

	$(CC) -c $(LSH)lsh.cpp $(HEL)parsinginit.cpp $(LSH)hashtable.cpp $(IMP)linkedlist.cpp $(LSH)others.cpp $(IMP)point.cpp -lm

others.o: $(LSH)others.cpp

	$(CC) -c $(LSH)others.cpp

point.o: $(IMP)point.cpp

	$(CC) -c $(IMP)point.cpp

parsinginit.o: $(HEL)parsinginit.cpp $(IMP)point.cpp

	$(CC) -c $(HEL)parsinginit.cpp $(IMP)point.cpp

hashtable.o: $(LSH)hashtable.cpp $(LSH)others.cpp $(IMP)linkedlist.cpp

	$(CC) -c $(LSH)hashtable.cpp $(LSH)others.cpp $(IMP)linkedlist.cpp

linkedlist.o: $(IMP)linkedlist.cpp $(LSH)others.cpp

	$(CC) -c $(IMP)linkedlist.cpp $(LSH)others.cpp

runlsh:

	./search -i input_file -q query_small_id -k 10 -L 10 -M 0 -probes 0 -o output.txt -algorithm LSH -metric no -delta 0.004

#-N 5 -R 500
#$./search –i <input file> –q <query file> –k <int> -L <int> -M <int> -probes
#<int> -ο <output file> -algorithm <LSH or Hypercube or Frechet> -metric <discrete
#or continuous | only for –algorithm Frechet> -delta <double>

valgrind:

	valgrind --leak-check=yes ./lsh -i input_file -q query_small_id -k 10 -L 10 -o output.txt -N 5 -R 100

clean: 

	rm -f *.o

	
	

