
#ifndef HASH
#define HASH

#include "../importantStructs/linkedlist.h"
#include "../helpers/helpers.h"

class hashTable
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

    int insert(point *pGiven, vector<float> vecToFindKey);

    // given a point, it searches for it in the hash, under construction
    int search(point *pGiven);

    // hash function, for now it represents the G(P) for lsh
    int hashFunction(point *pGiven);

    // destructor, under construction (no pun intented)
    ~hashTable();

    // constructor
    hashTable(int size, lshConstants *lshCon, int numOfDimensions, int frechet = 0);

    // returns the array as a whole...
    vector<list<linkedListNode *>> getArray();

    // just prints
    void print();

    // given a point, it finds its neighbors...
    void findKNeighbors(point *queryPoint, kNearest *nearestList, vector<float> vecFloat = vector<float>());

    void findKNeighborsTrue(point *queryPoint, kNearest *nearestList, vector<float> vecFloat = vector<float>());

    void findNeighborsR(point *queryPoint, kNearest *nearestList, int R);
};

#endif