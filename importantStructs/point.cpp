/// point ///
#include "point.h"
point::point(vector<float> pVector)
{
    this->pVector = pVector;
}

string point::getKey()
{
    return this->key;
}

point::point(std::vector<float> pVector, string key)
{
    this->pVector = pVector;
    this->key = key;
}

int point::getDimensions()
{
    return this->dimensions;
}

vector<float> *point::getVector()
{
    return &(this->pVector);
}

point::point() {}
point::~point() {}