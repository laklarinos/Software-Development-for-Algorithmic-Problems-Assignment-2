
#ifndef HASH_CUBE
#define HASH_CUBE
#include "../importantStructs/linkedlist.h"
#include "../importantStructs/structs.h"
#include "../helpers/helpers.h"

class hashTableCube
{

private:
    vector<list<linkedListNode*>> array;
    int size;
    vector<vAndT> vAndTVector;
    vector<vector<int>> zeroAndOneVect; //keep track of the 0 and 1 vector... 
    vector<int> rVector;
    cubeConstants *cubeCon;
    map<int, int> hToF;

public:
    // inserts a point
    int insert(point *pGiven);

    // hash function, for now it represents the G(P) for lsh
    int hashFunction(point *pGiven);

    // destructor, under construction (no pun intented)
    ~hashTableCube();

    // constructor
    hashTableCube(int size, cubeConstants *cubeCon, int numOfDimensions);

    // returns the array as a whole...
    vector<list<linkedListNode *>> getArray();

    // just prints
    void print();

    // given a point, it finds its neighbors by applying KNN...
    void findKNeighbors(point *queryPoint, kNearest *nearestList, int probes, int M);

    // given a point, it finds its K nearest neighbors by applying Brute Force...
    void findKNeighborsTrue(point *queryPoint, kNearest *nearestList, int probes, int M);

    // given a point, it finds its k nearest neighbors by applying Range Search...
    void findNeighborsR(point *queryPoint, kNearest *nearestList, int R, int probes, int M);

    int getSize();
};

#endif