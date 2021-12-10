
#ifndef HASH_CUBE
#define HASH_CUBE
#include "linkedlist.h"

class hashTableCube
{

private:
    vector<list<linkedListNode*>> array;
    int size;
    vector<vAndT> vAndTVector;
    vector<vector<int>> zeroAndOneVect; //keep track of the 0 and 1 vector... 
    vector<int> rVector;
    lshConstants *lshCon;
    map<int, int> hToF;

public:
    // inserts a point
    int insert(point *pGiven);

    // given a point, it searches for it in the hash, under construction
    int search(point *pGiven);

    // hash function, for now it represents the G(P) for lsh
    int hashFunction(point *pGiven);

    // destructor, under construction (no pun intented)
    ~hashTableCube();

    // constructor
    hashTableCube(int size, lshConstants *lshCon, int numOfDimensions);

    // returns the array as a whole...
    vector<list<linkedListNode *>> getArray();

    // just prints
    void print();

    // given a point, it finds its neighbors...
    void findKNeighbors(point *queryPoint, kNearest *nearestList, int probes, int M);

    void findKNeighborsTrue(point *queryPoint, kNearest *nearestList, int probes, int M);

    void findNeighborsR(point *queryPoint, kNearest *nearestList, int R, int probes, int M);

    int getSize();
};

#endif