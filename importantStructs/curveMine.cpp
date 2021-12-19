#include "curveMine.h"

string curveMine::getKey()
{
    return this->key;
}

vector<pair<float, float>> *curveMine::getVector()
{
    return &(this->arrayOfPoints);
}

int curveMine::getDimensions()
{
    return this->dimensions;
}

curveMine::curveMine(vector<pair<float, float>> pVector)
{
    this->arrayOfPoints = pVector;
}

curveMine::curveMine(vector<pair<float, float>> pVector, string key)
{
    this->arrayOfPoints = pVector;
    this->key = key;
}
curveMine::curveMine() {}

curveMine::~curveMine() {}
