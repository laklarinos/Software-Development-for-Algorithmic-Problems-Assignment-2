#include "curve.h"

string curve::getKey()
{
    return this->key;
}

vector<pair<int, double>> *curve::getVector()
{
    return &(this->arrayOfPoints);
}

int curve::getDimensions()
{
    return this->dimensions;
}

curve::curve(vector<pair<int, double>> pVector)
{
    this->arrayOfPoints = pVector;
}

curve::curve(vector<pair<int, double>> pVector, string key)
{
    this->arrayOfPoints = pVector;
    this->key = key;
}
curve::curve()
{
}
curve::curve()
{
}