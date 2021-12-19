
#ifndef HASH_CLUSTER
#define HASH_CLUSTER

#include "../importantStructs/linkedlist.h"
#include "../importantStructs/structs.h"
#include "../helpers/helpers.h"

class hashTableCluster
{

private:
    vector<list<linkedListNode *>> array;
    int size;
    vector<vAndT> vAndTVector;
    vector<int> rVector;
    lshConstants *lshCon;

public:
    // inserts a point
    int insert(point *pGiven);

    // given a point, it searches for it in the hash, under construction
    int search(point *pGiven);

    // hash function, for now it represents the G(P) for lsh
    int hashFunction(point *pGiven);

    // destructor, under construction (no pun intented)
    ~hashTableCluster();

    // constructor
    hashTableCluster(int size, lshConstants *lshCon, int numOfDimensions);

    // returns the array as a whole...
    vector<list<linkedListNode *>> getArray();

    // just prints
    void print();

    // given a point, it finds its neighbors...
    void findKNeighbors(point *queryPoint, kNearest *nearestList);

    void findKNeighborsTrue(point *queryPoint, kNearest *nearestList);

    void findNeighborsR(point *queryPoint, kNearest *nearestList, int R);
};

#endif