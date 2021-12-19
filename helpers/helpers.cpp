#include "helpers.h"
int parsInit(char *inputFile, vector<point> &vecPoints, int *numLinesReturn, int *numOfElements)
{
    fstream inputFilePtr;

    inputFilePtr.open(inputFile);
    if (!inputFilePtr)
    {
        cout << "Error,  file does not exist\n";
        return 1;
    }

    int numOfLines = 0;
    char *pch;
    ssize_t read;
    string line;
    ssize_t len = 0;
    int lineCounter = 0;
    int elementCounter = 0;

    int elementsPerLine = 0;
    string token;

    while (getline(inputFilePtr, line))
    {
        int c = 0;

        numOfLines++;
        istringstream is(line);
        size_t space_pos = line.find("	");
        string key = line.substr(0, space_pos);
        line.erase(0, space_pos + 1);

        vector<float> vecFloat;
        string values;
        istringstream is2(line);
        while (getline(is2, values, '\t'))
        {
            c++;
            vecFloat.push_back(atof(values.c_str()));
        }

        point pointToInsert(vecFloat, key);
        vecPoints.push_back(pointToInsert);
    }

    elementsPerLine = vecPoints[0].pVector.size();
    *numLinesReturn = numOfLines;
    *numOfElements = elementsPerLine;
    return 0;
}
bool checkCommandLineArguments(int argcInt, char *argvArray[])
{
    if (argcInt != 21)
    {
        cout << "not enough arguments\n";
        return 1;
    }

    //–i <input file>
    if (strcmp(argvArray[1], "-i") == 0)
    {
        // the next argument should be the dataset path
        if (access(argvArray[2], F_OK) != 0)
        {
            // file doesn't exist
            cout << "File: " << argvArray[2] << "does not exist\n";
            return 1;
        }
        //–q <query file>
        //inputFile = argvArray[2];
        if (strcmp(argvArray[3], "-q") == 0)
        {
            if (access(argvArray[4], F_OK) != 0)
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

        if (strcmp(argvArray[7], "-L") == 0)
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

        if (strcmp(argvArray[9], "-M") == 0)
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
                cout << "Argument 10 needs to be an int\n";
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

        if (strcmp(argvArray[11], "-probes") == 0)
        {
            // we want integer
            int counter = 0;
            int flag = 0;
            while (argvArray[12][counter] != '\0')
            {
                if (!isdigit(argvArray[12][counter]))
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
            //L = atoi(argvArray[8]);
        }

        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        // -o outputfile

        if (strcmp(argvArray[13], "-o") == 0)
        {
            if (access(argvArray[14], F_OK) != 0)
            {
                // file doesn't exist
                cout << "File " << argvArray[14] << " does not exist\n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        //outputFile = argvArray[10];
        if (strcmp(argvArray[15], "-algorithm") == 0)
        {
            if (strcmp(argvArray[16], "LSH") != 0 && strcmp(argvArray[16], "Hypercube") != 0 && strcmp(argvArray[16], "Frechet") != 0)
            {
                cout << "Argument 12 needs to be 1 of the 3 algorithms (LSH, Hypercube, Frechet), written in this way.\n";
                return 1;
            }
        }
        else
        {
            cout << "Argument 11 needs to be named \"algorithm\".\n";
            return 1;
        }

        if (strcmp(argvArray[17], "metric") == 0)
        {
            if (strcmp(argvArray[16], "Frechet") == 0)
            {
                if (strcmp(argvArray[18], "discrete") != 0 && strcmp(argvArray[18], "continuous") != 0)
                {
                    cout << "If Frechet is the algorithm, metric should be \"discrete\" or \"continuous\".\n";
                    return 1;
                }
            }
            else
            {
                cout << "Because Frechet algorithm was not selected, this argument will not be taken in to consideration.\n";
            }
        }

        if (strcmp(argvArray[19], "-delta") == 0)
        {
            //check if valid double...
            int len;
            double ignore = 0.0;
            char c;

            int ret = sscanf(argvArray[20], "%f %n", &ignore, &len);
            int res = (ret == 1 && len == (int)strlen(argvArray[20]));

            if (res != 1)
            {
                cout << "Argument 20 should be a valid double.\n";
                return 1;
            }
        }
    }
    else
    {
        cout << "Incorrect arguments\n";
        return 1;
    }
    return 0; //correct command line arguments
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

double euclDistance(point *point1, point *point2)
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

vector<float> snap(int dim, vector<pair<float, float>> &mapCurve, double delta, pair<float, float> txAndTy)
{

    // xi' = floor((x-t)/δ + 1/2)*δ + t
    // yi' = floor((y-t)/δ + 1/2)*δ + t
    // t should be different for each dimension
    // δ should be one digit positive number...

    int sizeOfMap = mapCurve.size();
    double tx = txAndTy.first;
    double ty = txAndTy.second;

    float xiNew;
    float yiNew;
    float xi;
    float yi;

    if (dim == 2)
    {
        vector<pair<float, float>> vecOfPairs;
        for (int i = 0; i < sizeOfMap; i++)
        {
            xi = mapCurve[i].first;
            yi = mapCurve[i].second;

            xiNew = abs(floor((xi - tx) / delta + 1 / 2) * delta + tx);
            yiNew = abs(floor((yi - ty) / delta + 1 / 2) * delta + ty);
            vecOfPairs.push_back(pair<float, float>(xiNew, yiNew));
        }

        // eliminate duplications...
        pair<float, float> tempPair = vecOfPairs[0];
        for (int i = 1; i < sizeOfMap; i++)
        {
            pair<float, float> currPair = vecOfPairs[i];
            if (tempPair.first == currPair.first && tempPair.second == currPair.second)
            {
                vecOfPairs[i].first = -1;
                vecOfPairs[i].second = -1;
            }
            else
            {
                tempPair = currPair;
            }
        }

        vecOfPairs.erase(std::remove(vecOfPairs.begin(), vecOfPairs.end(), pair<float, float>(-1, -1)), vecOfPairs.end()); //erase remove technique

        vector<float> vecToReturn;

        for (int i = 0; i < sizeOfMap; i++)
        {
            vecToReturn.push_back(vecOfPairs[i].first);
            vecToReturn.push_back(vecOfPairs[i].second);
        }
        return vecToReturn;
    }
    else if (dim == 1)
    {
        float epsilon = 1.5;
        vector<float> vecOfPairs;

        for (int i = 0; i < sizeOfMap; i++)
        {
            yi = mapCurve[i].second;
            yiNew = abs(floor((yi - ty) / delta + 1 / 2) * delta + ty);
            vecOfPairs.push_back(yiNew);
        }

        // minima maxima
        for (int i = 0; i < sizeOfMap - 3; i++)
        {
            // if |a − b| ≤ e and |b − c| ≤ e
            if (vecOfPairs[i] <= vecOfPairs[i + 1] && vecOfPairs[i + 1] <= vecOfPairs[i + 2] || vecOfPairs[i] >= vecOfPairs[i + 1] && vecOfPairs[i + 1] >= vecOfPairs[i + 2])
            {
                i += 2;
                vecOfPairs[i + 1] = -1;
            }
        }
        vecOfPairs.erase(std::remove(vecOfPairs.begin(), vecOfPairs.end(), -1), vecOfPairs.end()); //erase remove technique

        vector<float> vecToReturn;
        sizeOfMap = vecOfPairs.size();

        for (int i = 0; i < sizeOfMap; i++)
        {
            vecToReturn.push_back(vecOfPairs[i]);
        }
        return vecToReturn;
    }
}

float DFD(vector<float> curve1, vector<float> curve2)
{
    // Discrete frechet implemented with Dynamic Programming...
    int sizeOfCurve1 = curve1.size();
    int sizeOfCurve2 = curve2.size();

    vector<float> array(sizeOfCurve1);
    vector<vector<float>> completeArray(sizeOfCurve2);

    for (int i = 0; i < sizeOfCurve2; i++)
    {
        completeArray[i] = array;
    }

    completeArray[0][0] = euclDistanceOf2Points(0, 0, curve1[0], curve2[0]); // x1, x2, y1, y2
    for (int i = 0; i < sizeOfCurve1; i++)
    {
        for (int j = 0; j < sizeOfCurve2; j++)
        {
            if (i == 0 & j > 0)
            {
                float dist = euclDistanceOf2Points(0, j, curve1[0], curve2[j]);
                completeArray[i][j] = max<float>(completeArray[0][j - 1], dist); //max{c(0, j − 1), ||p0 − qj||}
                //cout << "1: " << completeArray[i][j] << endl;
            }

            if (i > 0 && j == 0)
            {
                float dist = euclDistanceOf2Points(i, 0, curve1[i], curve2[0]);
                completeArray[i][j] = max<float>(completeArray[i - 1][0], dist); //max{c(i-1, 0), ||pi − q0||}
                //cout << "2: " << completeArray[i][j] << endl;
            }
            if (i > 0 && j > 0)
            {
                float dist1 = euclDistanceOf2Points(i, j, curve1[i], curve2[j]);
                float min1 = min<float>(completeArray[i - 1][j], min<float>(completeArray[i - 1][j - 1], completeArray[i][j - 1])); //min{c(i − 1, j), c(i − 1, j − 1), c(i, j − 1)}
                completeArray[i][j] = max<float>(min1, dist1);                                                                      // max {min, ||pi − qj||} .
                //cout << "3: " << completeArray[i][j] << endl;
            }
        }
    }
    return completeArray[sizeOfCurve1 - 1][sizeOfCurve2 - 1];
}

float CFD(vector<float> curve1, vector<float> curve2)
{
    // first we need to create 2 repository curves out of our 2 curves...
    Curve curveRepo1 = toRepoCurve(curve1);
    Curve curveRepo2 = toRepoCurve(curve2);

    Frechet::Continuous::Distance d = Frechet::Continuous::distance(curveRepo1, curveRepo2);
    return d.value;
}

Curve toRepoCurve(vector<float> myCurve)
{
    Curve c(myCurve.size(), 1, "randy");
    for (float k : myCurve)
    {
        // cout << "k is " << k << endl;
        Point p(1);
        p[0] = k;
        c.push_back(p);
    }

    return c;
}

float euclDistanceOf2Points(float x1, float x2, float y1, float y2)
{
    float res = ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2));
    return sqrt(res);
}

void printToFile(char *outputFile, char *algorithm, vector<kNearest> &list,
                 vector<kNearest> &listTrue, vector<hashTable *> &arrayOfHashTables,
                 int numberOfNN, vector<point> &arrayOfCurves1DQ,
                 vector<vector<vector<float>>> const &arrayOfCurves1DQSnapped)
{
    float MAF = -1;
    float averageAppDist = 0.0;
    float averageTruDist = 0.0;
    int c = 0;
    int numOfLinesQ = arrayOfCurves1DQ.size();

    ofstream outputFileStream;
    int L = arrayOfHashTables.size();
    outputFileStream.open(outputFile, std::ios_base::app);

    for (int i = 0; i < numOfLinesQ; i++)
    {
        int flag = 0;
        initKNearest(numberOfNN, &(list[i]));
        initKNearest(numberOfNN, &(listTrue[i]));

        for (int j = 0; j < L; j++)
        {
            if (strcmp(algorithm, "LSH_Algorithm") == 0)
            {
                arrayOfHashTables[j]->findKNeighbors(&arrayOfCurves1DQ[i], &list[i]);
                arrayOfHashTables[j]->findKNeighborsTrue(&arrayOfCurves1DQ[i], &listTrue[i]);
            }
            else if (strcmp(algorithm, "LSH_Frechet_Discrete") == 0)
            {
                point temPoint(arrayOfCurves1DQSnapped[i][j]);
                arrayOfHashTables[j]->findKNeighbors(&arrayOfCurves1DQ[i], &list[i], arrayOfCurves1DQSnapped[i][j]);
                arrayOfHashTables[j]->findKNeighborsTrue(&arrayOfCurves1DQ[i], &listTrue[i], arrayOfCurves1DQSnapped[i][j]);
            }
            else if (strcmp(algorithm, "LSH_Frechet_Continuous") == 0)
            {
                point temPoint(arrayOfCurves1DQSnapped[i][j]);
                arrayOfHashTables[j]->findKNeighbors(&arrayOfCurves1DQ[i], &list[i], arrayOfCurves1DQSnapped[i][j], 1);
                arrayOfHashTables[j]->findKNeighborsTrue(&arrayOfCurves1DQ[i], &listTrue[i], arrayOfCurves1DQSnapped[i][j], 1);
            }
        }

        outputFileStream << "Query: " << arrayOfCurves1DQ[i].getKey() << endl;
        outputFileStream << "Algorithm: " << algorithm << endl;
        int tempId = -1;

        for (int z = 0; z < list[i].size; z++)
        {
            if (list[i].dist[z] != MAXFLOAT)
            {
                // we have a neighbor
                outputFileStream << "Approximate nearest neighbor"
                                 << ": " << list[i].nearestPoints[z]->getKey() << endl;
                outputFileStream << "Approximate distance: " << list[i].dist[z] << endl;

                if (listTrue[i].dist[z] != MAXFLOAT)
                {
                    // we have a neighbor
                    float temp = list[i].dist[z] / listTrue[i].dist[z];
                    if (temp > MAF)
                    {
                        MAF = temp;
                    }
                    flag = 1;
                    outputFileStream << "Approximate true neighbor"
                                     << ": " << listTrue[i].nearestPoints[z]->getKey() << endl;
                    outputFileStream << "True distance: " << listTrue[i].dist[z] << endl;
                }
                c++;
                averageAppDist += list[i].vecOfTimes[z].count();
                averageTruDist += listTrue[i].vecOfTimes[z].count();
                // outputFileStream << "   tLSH: " << list[i].vecOfTimes[z].count() << " ms" << endl;
                // //if (flag)
                // outputFileStream << "   tTrue: " << listTrue[i].vecOfTimes[z].count() << " ms" << endl
                //                  << endl;
            }
        }
        outputFileStream << "\n////////////////////////////////////////////////////////////\n";
    }
    if (c != 0)
    {
        outputFileStream << "tApproximateAverage: " << averageAppDist / c << " ms" << endl;
        outputFileStream << "tTrueAverage: " << averageTruDist / c << " ms" << endl;
    }
    outputFileStream << "MAF: <double> " << MAF << endl;
}

vector<float> meanCurve(vector<vector<float>> &vecOfCurves)
{
    int sizeOfVector = vecOfCurves.size();
    if(sizeOfVector == 1) return vecOfCurves[0];
    vector<vector<float>> temp;
    int i;
    for (i = 0; i < sizeOfVector - 1; i++)
    {
        temp.push_back(meanFrechet(vecOfCurves[i], vecOfCurves[i + 1]));
        i += 2;
    }

    if (sizeOfVector - i == 2)
    {
        temp.push_back(vecOfCurves[sizeOfVector - 1]);
    }
    
    return meanCurve(temp);
}

vector<float> meanFrechet(vector<float> &curve1, vector<float> &curve2)
{
    
}