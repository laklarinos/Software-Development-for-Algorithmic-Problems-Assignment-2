CC = g++ -g -O2 -std=c++14 
HEL = ../helpers/
IMP = ../importantStructs/
LSH = ../lsh/
CUB = ../hypercube/
FRE = ../Fred-master/src/
PREVDIR = ../

clusterprog: main.o cluster.o checkcommandlinecluster.o helpers.o lshhashtable.o cubehashtable.o point.o linkedlist.o frechet.o config.o pointRepo.o interval.o simplification.o curve.o
	
	$(CC) -o clusterprog main.o cluster.o checkcommandlinecluster.o helpers.o lshhashtable.o cubehashtable.o point.o linkedlist.o frechet.o config.o pointRepo.o interval.o simplification.o curve.o

cluster: cluster.o helpers.o lshhashTable.o cubehashTable.o

	$(CC) -o cluster.o helpers.o lshhashTable.o cubehashTable.o

cluster.o: cluster.cpp $(IMP)linkedlist.cpp $(HEL)helpers.cpp $(CUB)cubehashtable.cpp $(LSH)lshhashTable.cpp

	$(CC) -c cluster.cpp $(IMP)linkedlist.cpp $(HEL)helpers.cpp $(CUB)cubehashtable.cpp $(LSH)lshhashTable.cpp

######## MAIN #######
main.o: main.cpp checkcommandlinecluster.cpp cluster.cpp $(HEL)helpers.cpp $(LSH)lshhashtable.cpp $(CUB)cubehashtable.cpp $(IMP)point.cpp $(IMP)linkedlist.cpp 

	$(CC) -c main.cpp checkcommandlinecluster.cpp cluster.cpp $(HEL)helpers.cpp $(LSH)lshhashtable.cpp $(CUB)cubehashtable.cpp $(IMP)point.cpp $(IMP)linkedlist.cpp
#####################

######## FOR FILES lsh ########
lshhashtable.o: $(LSH)lshhashtable.cpp $(IMP)linkedlist.cpp $(HEL)helpers.cpp $(IMP)point.cpp

	$(CC) -c $(LSH)lshhashtable.cpp $(IMP)linkedlist.cpp $(HEL)helpers.cpp $(IMP)point.cpp
################################


######## FOR FILES hypercube ######## 
cubehashtable.o: $(CUB)cubehashtable.cpp $(IMP)linkedlist.cpp

	$(CC) -c $(CUB)cubehashtable.cpp $(IMP)linkedlist.cpp
#################################

######## FOR FILES hypercube ######## 
clusterhashtable.o: clusterhashtable.cpp $(IMP)linkedlist.cpp

	$(CC) -c clusterhashtable.cpp $(IMP)linkedlist.cpp
#################################


###### FRECHET ######
frechet: frechet.o config.o curve.o interval.o pointRepo.o simplification.o

	$(CC) -o frechet frechet.o config.o curve.o interval.o pointRepo.o simplification.o

frechet.o:	$(FRE)frechet.cpp $(FRE)config.cpp $(FRE)curve.cpp $(FRE)interval.cpp $(FRE)pointRepo.cpp $(FRE)simplification.cpp

	$(CC) -c $(FRE)frechet.cpp $(FRE)config.cpp $(FRE)curve.cpp $(FRE)interval.cpp $(FRE)pointRepo.cpp $(FRE)simplification.cpp
################################

###### FOR important structs FILES ######
point.o: $(IMP)point.cpp

	$(CC) -c $(IMP)point.cpp

linkedlist.o: $(IMP)linkedlist.cpp

	$(CC) -c $(IMP)linkedlist.cpp

helpers: helpers.o frechet.o

	$(CC) -o helpers helpers.o frechet.o

helpers.o: $(HEL)helpers.cpp $(IMP)point.cpp $(LSH)lshhashTable.cpp

	$(CC) -c $(HEL)helpers.cpp $(IMP)point.cpp $(LSH)lshhashTable.cpp
##########################################

# $./cluster –i <input file> –c <configuration file> -o <output file> -update <Mean
# Frechet or Mean Vector> –assignment <Classic or LSH or Hypercube or LSH_Frechet>
# -complete <optional> -silhouette <optional>

run:

	./clusterprog –i $(PREVDIR)nasd_input2017 –c cluster.conf -o $(PREVDIR)output.txt -update Mean_Frechet -assignment Classic -silhouette no

runlsh:

	./clusterprog –i $(PREVDIR)nasd_input2017 –c cluster.conf -o $(PREVDIR)output.txt -update Mean_Vector -assignment LSH -silhouette no

runhyper:

	./clusterprog –i $(PREVDIR)nasd_input2017 –c cluster.conf -o $(PREVDIR)output.txt -update Mean_Vector -assignment Hypercube -silhouette no

runclassic:

	./clusterprog –i $(PREVDIR)nasd_input2017 –c cluster.conf -o $(PREVDIR)output.txt -update Mean_Vector -assignment Classic -silhouette no

runlshfrechet:

	./clusterprog –i $(PREVDIR)nasd_input2017 –c cluster.conf -o $(PREVDIR)output.txt -update Mean_Frechet -assignment LSH -silhouette no

runclassicfrechet:

	./clusterprog –i $(PREVDIR)nasd_input2017 –c cluster.conf -o $(PREVDIR)output.txt -update Mean_Frechet -assignment Classic -silhouette no

valgrind:

	valgrind --leak-check=yes ./clusterprog –i $(PREVDIR)input_small_id –c cluster.conf -o $(PREVDIR)output.txt -Complete yes -m HYPER

clean: 

	rm -f *.o clusterprog

	
	

