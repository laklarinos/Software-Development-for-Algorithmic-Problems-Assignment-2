#include "linkedlist.h"
using namespace std;

///// linked list node //////
linkedListNode::linkedListNode(point *pGiven, int key, int IDp)
{
    this->pVector = pGiven;
    this->key = key;
    this->IDp = IDp;
}
point *linkedListNode::getPVector()
{
    return this->pVector;
}
int linkedListNode::getKey()
{
    return this->key;
}

linkedListNode::linkedListNode() {}

int linkedListNode::getIDp()
{
    return this->IDp;
}
