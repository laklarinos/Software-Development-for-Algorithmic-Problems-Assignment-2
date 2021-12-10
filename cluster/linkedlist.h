#ifndef LINKEDLIST
#define LINKEDLIST
#include "others.h"
using namespace std;

class linkedListNode
{
private:
   int key;
   int IDp;

public:
   linkedListNode(point *pGiven, int key, int IDp);
   point *pVector;
   linkedListNode();
   point *getPVector();
   int getKey();
   int getIDp();
};

#endif
