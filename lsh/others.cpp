#include "others.h"
using namespace std;
// bool checkCommandLineArguments(int argcInt, char *argvArray[])
// {

//     int k;
//     int L;
//     int numberOfNN;
//     int rad;
//     int numOfLines = 0;
//     int numOfElements = 0;
//     char *pch;
//     ssize_t read;
//     char *line;
//     ssize_t len = 0;
//     int lineCounter = 0;
//     int elementCounter = 0;
//     char *inputFile;
//     char *queryFile;
//     char *outputFile; // na to doume

//     if (argcInt != 15)
//     {
//         cout << "not enough arguments\n";
//         return 1;
//     }

//     //–i <input file>
//     if (strcmp(argvArray[1], "-i") == 0)
//     {
//         // the next argument should be the dataset path
//         if (access(argvArray[2], F_OK) == 0)
//         {
//             // file exists
//             cout << "file exists\n";
//         }
//         else
//         {
//             // file doesn't exist
//             cout << "File: " << argvArray[2] << "does not exist\n";
//             return 1;
//         }
//         //–q <query file>
//         //inputFile = argvArray[2];
//         if (strcmp(argvArray[3], "-q") == 0)
//         {
//             if (access(argvArray[4], F_OK) == 0)
//             {
//                 // file exists
//                 cout << "File Exists\n";
//             }
//             else
//             {
//                 // file doesn't exist
//                 cout << "File: " << argvArray[4] << " does not exist\n";
//                 return 1;
//             }
//         }
//         else
//         {
//             cout << "Incorrect arguments\n";
//             return 1;
//         }

//         //queryFile = argvArray[4];
//         //–k <int>
//         if (strcmp(argvArray[5], "-k") == 0)
//         {
//             // we want integer
//             int counter = 0;
//             int flag = 0;
//             while (argvArray[6][counter] != '\0')
//             {
//                 if (!isdigit(argvArray[6][counter]))
//                 {
//                     flag = 1;
//                     break;
//                 }
//                 counter++;
//             }
//             if (flag)
//             {
//                 // not an int...
//                 cout << "Argument 6 needs to be an int\n";
//                 return 1;
//             }
//             // else int
//             //k = atoi(argvArray[6]);
//         }
//         else
//         {
//             cout << "Incorrect arguments\n";
//             return 1;
//         }

//         if (strcmp(argvArray[7], "-L") == 0)
//         {
//             // we want integer
//             int counter = 0;
//             int flag = 0;
//             while (argvArray[8][counter] != '\0')
//             {
//                 if (!isdigit(argvArray[8][counter]))
//                 {
//                     flag = 1;
//                     break;
//                 }
//                 counter++;
//             }
//             if (flag)
//             {
//                 // not an int...
//                 cout << "Argument 8 needs to be an int\n";
//                 return 1;
//             }
//             // else int
//             //L = atoi(argvArray[8]);
//         }
//         else
//         {
//             cout << "Incorrect arguments\n";
//             return 1;
//         }

//         // -o outputfile

//         if (strcmp(argvArray[9], "-o") == 0)
//         {
//             if (access(argvArray[10], F_OK) == 0)
//             {
//                 // file exists
//                 printf("File exists\n");
//             }
//             else
//             {
//                 // file doesn't exist
//                 cout << "File " << argvArray[10] << " does not exist\n";
//                 return 1;
//             }
//         }
//         else
//         {
//             cout << "Incorrect arguments\n";
//             return 1;
//         }

//         //outputFile = argvArray[10];

//         if (strcmp(argvArray[11], "-N") == 0)
//         {
//             // we want integer
//             int counter = 0;
//             int flag = 0;
//             while (argvArray[12][counter] != '\0')
//             {
//                 if (!isdigit(argvArray[12][counter]))
//                 {
//                     flag = 1;
//                     break;
//                 }
//                 counter++;
//             }
//             if (flag)
//             {
//                 // not an int...
//                 cout << "Argument 12 needs to be an int\n";
//                 return 1;
//             }
//             // else int
//             //numberOfNN = atoi(argvArray[12]);
//         }
//         else
//         {
//             cout << "Incorrect arguments\n";
//             return 1;
//         }

//         if (strcmp(argvArray[13], "-R") == 0)
//         {
//             // we want integer
//             int counter = 0;
//             int flag = 0;
//             while (argvArray[14][counter] != '\0')
//             {
//                 if (!isdigit(argvArray[14][counter]))
//                 {
//                     flag = 1;
//                     break;
//                 }
//                 counter++;
//             }
//             if (flag)
//             {
//                 // not an int...
//                 cout << "Argument 8 needs to be an int\n";
//                 return 1;
//             }
//             // else int
//             //rad = atoi(argvArray[14]);
//         }
//         else
//         {
//             cout << "Incorrect arguments\n";
//             return 1;
//         }
//     }
//     else
//     {
//         cout << "Incorrect arguments\n";
//         return 1;
//     }
//     return 0; //correct command line arguments
// }

vector<double> snap(int dim, map<double, double> &mapCurve, double delta, pair<double, double> txAndTy)
{
    // xi' = floor((x-t)/δ + 1/2)*δ + t
    // yi' = floor((y-t)/δ + 1/2)*δ + t
    // t should be different for each dimension
    // δ should be one digit positive number...

    int sizeOfMap = mapCurve.size();
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, delta);
    double tx = distribution(generator);
    double ty = distribution(generator);

    double xiNew;
    double yiNew;
    double xi;
    double yi;

    vector<pair<double, double>> vecOfPairs;

    for (int i = 0; i < sizeOfMap; i++)
    {
        std::map<double, double>::iterator it = mapCurve.begin();
        advance(it, i);

        xi = it->first;
        yi = it->second;

        xiNew = floor((xi - tx) / delta + 1 / 2) * delta + tx;
        yiNew = floor((yi - ty) / delta + 1 / 2) * delta + ty;
        vecOfPairs.push_back(pair<double, double>(xiNew, yiNew));
    }

    // eliminate duplications...
    pair<double, double> tempPair = vecOfPairs[0];
    for (int i = 1; i < sizeOfMap; i++)
    {
        pair<double, double> currPair = vecOfPairs[i];
        if (tempPair.first == currPair.first && tempPair.second == currPair.second)
        {
            auto it = vecOfPairs.begin() + i;
            vecOfPairs[i].first = 0;
            vecOfPairs[i].second = 0;
        }
        else
        {
            tempPair = currPair;
        }
    }

    vecOfPairs.erase(std::remove(vecOfPairs.begin(), vecOfPairs.end(), pair<double,double>(0,0)), vecOfPairs.end()); //erase remove technique

    vector<double> vecToReturn;
    sizeOfMap = vecOfPairs.size();

    for (int i = 0; i < sizeOfMap; i++)
    {
        auto it = vecOfPairs.begin() + i;
        vecToReturn.push_back(it->first);
        vecToReturn.push_back(it->second);
    }

    return vecToReturn;
}

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