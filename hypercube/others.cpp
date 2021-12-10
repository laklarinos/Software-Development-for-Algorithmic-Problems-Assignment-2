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

    if (argcInt != 17)
    {
        cout << "not enough arguments\n";
        return 1;
    }

    //–i <input file>
    if (strcmp(argvArray[1], "-i") == 0)
    {
        // the next argument should be the dataset path
        if (access(argvArray[2], F_OK))
        {
            // file doesn't exist
            cout << "File: " << argvArray[2] << "does not exist\n";
            return 1;
        }
        //–q <query file>
        //inputFile = argvArray[2];
        if (strcmp(argvArray[3], "-q") == 0)
        {
            if (access(argvArray[4], F_OK))
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
        if (strcmp(argvArray[5], "-k") == 0)
        {
            // we want integer
            int counter = 0;
            int flag = 0;
            while (argvArray[6][counter] != '\0')
            {
                if (!isdigit(argvArray[6][counter]))
                {
                    flag = 1;
                    break;
                }
                counter++;
            }
            if (flag)
            {
                // not an int...
                cout << "Argument 6 needs to be an int\n";
                return 1;
            }
            // else int
            //k = atoi(argvArray[6]);
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        if (strcmp(argvArray[7], "-M") == 0)
        {
            // we want integer
            int counter = 0;
            int flag = 0;
            while (argvArray[8][counter] != '\0')
            {
                if (!isdigit(argvArray[8][counter]))
                {
                    flag = 1;
                    break;
                }
                counter++;
            }
            if (flag)
            {
                // not an int...
                cout << "Argument 8 needs to be an int\n";
                return 1;
            }
            // else int
            //L = atoi(argvArray[8]);
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        // -o outputfile
        if (strcmp(argvArray[9], "-probes") == 0)
        {
            // we want integer
            int counter = 0;
            int flag = 0;
            while (argvArray[10][counter] != '\0')
            {
                if (!isdigit(argvArray[10][counter]))
                {
                    flag = 1;
                    break;
                }
                counter++;
            }
            if (flag)
            {
                // not an int...
                cout << "Argument 12 needs to be an int\n";
                return 1;
            }
            // else int
            //numberOfNN = atoi(argvArray[12]);
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        if (strcmp(argvArray[11], "-o") == 0)
        {
            if (access(argvArray[12], F_OK) == 0)
            {
                // file exists
                printf("File exists\n");
            }
            else
            {
                // file doesn't exist
                cout << "File " << argvArray[12] << " does not exist\n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        //outputFile = argvArray[10];

        if (strcmp(argvArray[13], "-N") == 0)
        {
            // we want integer
            int counter = 0;
            int flag = 0;
            while (argvArray[14][counter] != '\0')
            {
                if (!isdigit(argvArray[14][counter]))
                {
                    flag = 1;
                    break;
                }
                counter++;
            }
            if (flag)
            {
                // not an int...
                cout << "Argument 8 needs to be an int\n";
                return 1;
            }
            // else int
            //rad = atoi(argvArray[14]);
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        if (strcmp(argvArray[15], "-R") == 0)
        {
            // we want integer
            int counter = 0;
            int flag = 0;
            while (argvArray[16][counter] != '\0')
            {
                if (!isdigit(argvArray[16][counter]))
                {
                    flag = 1;
                    break;
                }
                counter++;
            }

            if (flag)
            {
                // not an int...
                cout << "Argument 8 needs to be an int\n";
                return 1;
            }
            // else int
            //rad = atoi(argvArray[14]);
        }
    }
    else
    {
        cout << "Incorrect arguments\n";
        return 1;
    }
    return 0; //correct command line arguments
}

/// point ///
// point::point(vector<int> pVector)
// {
//     this->pVector = pVector;
// }

// point::point(std::vector<float> pVector, string key)
// {
//     this->pVector = pVector;
//     this->key = key;
// }

// int point::getDimensions()
// {
//     return this->dimensions;
// }

// vector<int> *point::getVector()
// {
//     return &(this->pVector);
// }

// point::point()
// {
// }
// point::~point() {}

int euclideanRemainder(int a, int b)
{
    assert(b != 0);
    int r = a % b;
    return r >= 0 ? r : r + std::abs(b);
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
    vector<float> *p1coords = point1->getVector();
    vector<float> *p2coords = point2->getVector();
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
    //srand(time(NULL));
    int res = (rand() % 2);
    return res;
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