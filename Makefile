CC = g++ -g -std=c++14 
HEL = helpers/
IMP = importantStructs/
LSH = lsh/
CUB = hypercube/
FRE = Fred-master/src/

search: main.o helpers.o lsh.o lshhashtable.o cubehashtable.o point.o linkedlist.o cube.o frechet.o config.o pointRepo.o interval.o simplification.o curve.o
	
	$(CC) -o search main.o helpers.o lsh.o lshhashtable.o cubehashtable.o point.o linkedlist.o cube.o frechet.o config.o pointRepo.o interval.o simplification.o curve.o

######## MAIN #######
main.o: main.cpp $(HEL)helpers.cpp $(LSH)lsh.cpp $(LSH)lshhashtable.cpp $(CUB)cube.cpp $(CUB)cubehashtable.cpp $(IMP)point.cpp $(IMP)linkedlist.cpp 

	$(CC) -c main.cpp $(HEL)helpers.cpp $(LSH)lsh.cpp $(LSH)lshhashtable.cpp $(CUB)cube.cpp $(CUB)cubehashtable.cpp $(IMP)point.cpp $(IMP)linkedlist.cpp
##############################

######## FOR FILES lsh ########
lsh: lsh.o lshhashtable.o helpers.o

	$(CC) -o lsh lsh.o lshhashtable.o helpers.o

lsh.o: $(LSH)lsh.cpp $(LSH)lshhashtable.cpp $(IMP)linkedlist.cpp $(HEL)helpers.cpp

	$(CC) -c $(LSH)lsh.cpp $(LSH)lshhashtable.cpp $(IMP)linkedlist.cpp $(HEL)helpers.cpp

lshhashtable.o: $(LSH)lshhashtable.cpp $(LSH) $(IMP)linkedlist.cpp $(HEL)helpers.cpp $(IMP)point.cpp

	$(CC) -c $(LSH)lshhashtable.cpp $(LSH) $(IMP)linkedlist.cpp $(HEL)helpers.cpp $(IMP)point.cpp
################################


######## FOR FILES hypercube ########
cube: cube.o cubehashtable.o  

	$(CC) -o cube cube.o cubehashtable.o  

cube.o: $(CUB)cube.cpp $(CUB)cubehashtable.cpp $(CUB)  

	$(CC) -c $(CUB)cube.cpp $(CUB)cubehashtable.cpp $(CUB)  

cubehashtable.o: $(CUB)cubehashtable.cpp $(IMP)linkedlist.cpp

	$(CC) -c $(CUB)cubehashtable.cpp $(IMP)linkedlist.cpp
#################################


######## FOR FILES importantStructs AND helpers #########
point.o: $(IMP)point.cpp

	$(CC) -c $(IMP)point.cpp

linkedlist.o: $(IMP)linkedlist.cpp

	$(CC) -c $(IMP)linkedlist.cpp

helpers: helpers.o frechet.o

	$(CC) -o helpers helpers.o frechet.o

helpers.o: $(HEL)helpers.cpp $(IMP)point.cpp $(LSH)lshhashTable.cpp

	$(CC) -c $(HEL)helpers.cpp $(IMP)point.cpp $(LSH)lshhashTable.cpp
################################

###### FRECHET ######
frechet: frechet.o config.o curve.o interval.o pointRepo.o simplification.o

	$(CC) -o frechet frechet.o config.o curve.o interval.o pointRepo.o simplification.o

frechet.o:	$(FRE)frechet.cpp $(FRE)config.cpp $(FRE)curve.cpp $(FRE)interval.cpp $(FRE)pointRepo.cpp $(FRE)simplification.cpp

	$(CC) -c $(FRE)frechet.cpp $(FRE)config.cpp $(FRE)curve.cpp $(FRE)interval.cpp $(FRE)pointRepo.cpp $(FRE)simplification.cpp
################################

runlsh:

	./search -i nasd_input2017 -q nasd_query2017 -k 10 -L 10 -M 0 -probes 0 -o output.txt -algorithm LSH -metric no -delta 3.25

runlshfrechetd:

	./search -i nasd_input2017 -q nasd_query2017 -k 10 -L 10 -M 0 -probes 0 -o output.txt -algorithm Frechet -metric discrete -delta 3.25

runlshfrechetc:

	./search -i nasd_input2017 -q nasd_query2017 -k 10 -L 10 -M 0 -probes 0 -o output.txt -algorithm Frechet -metric continuous -delta 3.25

runhyper:

	./search -i nasd_input2017 -q nasd_query2017 -k 10 -L 10 -M 12 -probes 1 -o output.txt -algorithm Hypercube -metric no -delta 3.25

valgrind:

	valgrind --leak-check=yes ./lsh -i nasd_input2017 -q nasdaq2017_query -k 10 -L 10 -o output.txt -N 5 -R 100

clean: 

	rm -f *.o search main

	
	

