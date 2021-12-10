#include "others.h"
using namespace std;
bool checkCommandLineArguments(int argcInt, char *argvArray[])
{

    int k;
    int L;
    int numberOfNN;
    int rad;
    int numOfLines = 0;
    int numOfElements = 0;
    char *pch;
    ssize_t read;
    char *line;
    ssize_t len = 0;
    int lineCounter = 0;
    int elementCounter = 0;
    char *inputFile;
    char *queryFile;
    char *outputFile; // na to doume

    if (argcInt != 11)
    {
        cout << "not enough arguments\n";
        return 1;
    }

    //–i <input file>
    if (strcmp(argvArray[1], "-i") == 0)
    {
        // the next argument should be the dataset path
        if (access(argvArray[2], F_OK) == 0)
        {
            // file exists
            cout << "file exists\n";
        }
        else
        {
            // file doesn't exist
            cout << "File: " << argvArray[2] << "does not exist\n";
            return 1;
        }
        //–q <query file>
        //inputFile = argvArray[2];
        if (strcmp(argvArray[3], "-c") == 0)
        {
            if (access(argvArray[4], F_OK) == 0)
            {
                // file exists
                cout << "File Exists\n";
            }
            else
            {
                // file doesn't exist
                cout << "File: " << argvArray[4] << " does not exist\n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        //queryFile = argvArray[4];
        //–k <int>
        if (strcmp(argvArray[5], "-o") == 0)
        {
            if (access(argvArray[6], F_OK) == 0)
            {
                // file exists
                cout << "File Exists\n";
            }
            else
            {
                // file doesn't exist
                cout << "File: " << argvArray[6] << " does not exist\n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        if (strcmp(argvArray[7], "-complete") == 0)
        {
            if (strcmp(argvArray[8], "yes") != 0 && strcmp(argvArray[8], "no") != 0)
            {
                cout << "-Complete argument must be followed by a \"yes\" or \"no\" \n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        // -o outputfile

        if (strcmp(argvArray[9], "-m") == 0)
        {
            if (strcmp(argvArray[10], "Classic") != 0 && 
                strcmp(argvArray[10], "LSH") != 0 && 
                strcmp(argvArray[10], "Hypercube")!=0)
            {
                cout << "-m Argument must be followed by a \"Classic\", \"LSH\" or \"Hypercube\".\n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }
    }
    return 0; //correct command line arguments
}

/// point ///
point::point(vector<int> pVector)
{
    this->pVector = pVector;
    this->dimensions = pVector.size() - 1;
}

int point::getDimensions()
{
    return this->dimensions;
}

vector<int> *point::getVector()
{
    return &(this->pVector);
}

point::point() {}
point::~point() {}

int euclideanRemainder(int a, int b)
{
    assert(b != 0);
    int r = a % b;
    
    return r >= 0 ? r : r + std::abs(b);
}

int euclideanRemainder(int a, unsigned int b)
{
    assert(b != 0);
    int r = a % b;
    
    return r >= 0 ? r : r + b;
}

int myPow(int x, int p)
{
    if (p == 0)
        return 1;
    if (p == 1)
        return x;

    int tmp = myPow(x, p / 2);
    if (p % 2 == 0)
        return tmp * tmp;
    else
        return x * tmp * tmp;
}

double calculateDistance(point *point1, point *point2)
{
    vector<int> *p1coords = point1->getVector();
    vector<int> *p2coords = point2->getVector();
    double Sum = 0.0;
    for (int i = 1; i < p1coords->size(); i++)
    {
        Sum = Sum + pow(((*p1coords)[i] - (*p2coords)[i]), 2.0);
    }
    return sqrt(Sum);
}

void initKNearest(int k, kNearest *list)
{
    list->nearestPoints.resize(k);
    list->dist.resize(k);
    list->size = k;
    list->vecOfTimes.resize(k);

    for (int i = 0; i < k; i++)
    {
        list->dist[i] = MAXFLOAT;
        list->nearestPoints[i] = NULL;
    }
}
void sortNearest(kNearest *list)
{
    point *tempPoint;
    double tempDist;
    std::chrono::duration<double> tempTime;
    for (int i = list->size - 1; i > 0; i--)
    {
        if (list->dist[i] < list->dist[i - 1])
        {
            //Swap
            tempDist = list->dist[i];
            list->dist[i] = list->dist[i - 1];
            list->dist[i - 1] = tempDist;

            tempPoint = list->nearestPoints[i];
            list->nearestPoints[i] = list->nearestPoints[i - 1];
            list->nearestPoints[i - 1] = tempPoint;

            tempTime = list->vecOfTimes[i];
            list->vecOfTimes[i] = list->vecOfTimes[i - 1];
            list->vecOfTimes[i - 1] = tempTime;
        }
    }
}

void decToBin(vector<int> *vecGiven, int numBits, int dec)
{
    for (int i = 0; i < numBits; i++)
    {
        vecGiven->push_back(0);
    }

    int i = 0;
    int r;
    while (dec != 0)
    {
        r = dec % 2;

        vecGiven->at(i) = r;
        dec /= 2;
        i++;
    }

    reverse(vecGiven->begin(), vecGiven->end());
}

bool zeroOrOne(int numGiven)
{
    return euclideanRemainder(numGiven, 2);
}

int binToDec(vector<int> &binVector)
{
    int total = 0;
    int size = binVector.size();
    for (int i = size - 1; i >= 0; i--)
    {
        if (binVector[i] == 1)
            total += binVector[i] * myPow(2, i);
    }
    return total;
}

void findVectorsOfHamDist(vector<int> vecGiven, int hamDist, vector<vector<int>> *vecToFill, int index)
{
    if (hamDist == 0)
    {
        vecToFill->push_back(vecGiven);
        return;
    }

    if (index < 0)
        return;

    vecGiven[index] = vecGiven[index] == 0 ? 1 : 0;
    findVectorsOfHamDist(vecGiven, hamDist - 1, vecToFill, index - 1);

    vecGiven[index] = vecGiven[index] == 0 ? 1 : 0;
    findVectorsOfHamDist(vecGiven, hamDist, vecToFill, index - 1);
}