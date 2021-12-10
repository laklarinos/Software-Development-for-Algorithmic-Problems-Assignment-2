
#ifndef HASH
#define HASH
#include "../linkedlist.h"

class hashTable
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

    // hash function, for now it represents the G(P) for lsh
    int hashFunction(point *pGiven);

    // destructor, under construction (no pun intented)
    ~hashTable();

    // constructor
    hashTable(int size, lshConstants *lshCon, int numOfDimensions);

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