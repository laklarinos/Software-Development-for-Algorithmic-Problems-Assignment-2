#ifndef POINT
#define POINT

#include "../helpers/includes.h"
using namespace std;

class point
{
private:
    int dimensions;
    string key;

public:
    std::vector<float> pVector;
    string getKey();
    std::vector<float> *getVector();
    int getDimensions();
    point(std::vector<float> pVector);
    point(std::vector<float> pVector, string key);
    point();
    ~point();
};

#endif