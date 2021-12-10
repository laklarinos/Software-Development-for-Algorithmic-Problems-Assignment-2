#include "parsinginit.h"
#include "cluster.h"
using namespace std;

int main(int argc, char *argv[])
{
    char *inputFile;
    char *outputFile;
    char *confFile;
    char *complete;
    char *method;

    int kLSH;
    int kCUBE;
    int L;
    int M;
    int numberOfNN;
    int rad;
    int w = 3;
    int numOfClusters;
    int probes;
    int numOfLines;
    int numOfElements;
    int numOfDimensions;

    lshConstants lshCon;

    // for query file
    int numOfLinesQ;
    int numOfElementsQ;
    int numOfDimensionsQ;

    int checkArgs = checkCommandLineArguments(argc, argv);
    if (checkArgs)
    {
        exit(1);
    }

    //init arguments
    inputFile = argv[2];
    confFile = argv[4];
    outputFile = argv[6];
    complete = argv[8];
    method = argv[10];

    //read conf file...
    vector<int> vecOfConfArgs;
    string line;
    fstream confFilePtr;
    confFilePtr.open(confFile);
    int counter = 0;

    while (getline(confFilePtr, line))
    {
        stringstream ss(line);
        string s;
        while (getline(ss, s, ' '))
        {
            counter++;
            if (counter % 2 == 0)
            {
                vecOfConfArgs.push_back(stoi(s));
            }
        }
    }

    if (vecOfConfArgs.size() == 6)
    {
        numOfClusters = vecOfConfArgs[0];
        L = vecOfConfArgs[1];
        kLSH = vecOfConfArgs[2];
        M = vecOfConfArgs[3];
        kCUBE = vecOfConfArgs[4];
        probes = vecOfConfArgs[5];
    }
    else
    {
        cout << "ERROR, .conf file should provide 6 arguments.\n";
        return 1;
    }

    fstream inputFilePtr;
    fstream queryFilePtr;
    fstream outputFilePtr;

    vector<point> vecOfPoints;

    int res = parsInit(inputFile, vecOfPoints, &numOfLines, &numOfElements);
    numOfDimensions = numOfElements - 1;
    if (res == 1)
    {
        exit(1);
    }

    if (vecOfPoints.empty())
    {
        exit(1);
    }

    // first centroid randomly generated...
    vector<centroid> vecOfCentroids;

    // K-MEANS ++, INITIALIZATION
    initCentroids(vecOfCentroids, numOfClusters, vecOfPoints);
    // LLOYD, LSH REVERSE, HYPERCUBE REVERSE, ASSIGNMENT

    if (strcmp(method, "Classic") == 0)
    {
        // LLOYD

        vector<centroid> temp = vecOfCentroids;
        int maxIt = 100;
        int countIt = 1;
        int flag = 0;

        using clock = std::chrono::system_clock;
        auto begin = clock::now();

        while (countIt < maxIt && flag < numOfClusters)
        {
            flag = 0;
            //cout << currMeanDistance << endl;
            lloyd(vecOfCentroids, vecOfPoints);
            updateCentroids(vecOfCentroids, numOfClusters);
            float dist;
            for (int j = 0; j < numOfClusters; j++)
            {
                dist = calculateDistance(temp[j].coordinates, vecOfCentroids[j].coordinates);
                if (dist < 85)
                    flag++;
                else
                    flag = 0;
            }
            temp = vecOfCentroids;
            countIt++;
        }

        using sec = std::chrono::duration<double>;
        sec end = clock::now() - begin;

        ofstream outputFileStream;
        outputFileStream.open(outputFile);
        outputFileStream << "Algorithm: Range Search Classic\n";
        printToFile(outputFileStream, vecOfCentroids, end, complete);
    }
    else if (strcmp(method, "LSH") == 0)
    {
        // LSH REVERSE
        lshCon.k = kLSH;
        lshCon.w = w;
        lshCon.L = L;
        vector<hashTable *> vecOfHashTables(L);
        for (int i = 0; i < L; i++)
        {
            vecOfHashTables[i] = new hashTable(numOfLines / 8, &lshCon, numOfDimensions);
        }

        // insert to HashTables...s
        for (int i = 0; i < numOfLines; i++)
        {
            for (int j = 0; j < L; j++)
            {
                vecOfHashTables[j]->insert(&vecOfPoints[i]);
            }
        }

        vector<centroid> temp = vecOfCentroids;
        int maxIt = 100;
        int countIt = 1;
        int flag = 0;

        using clock = std::chrono::system_clock;
        auto begin = clock::now();

        while (countIt < maxIt && flag < numOfClusters)
        {
            flag = 0;
            //cout << currMeanDistance << endl;
            lshReverse(vecOfCentroids, vecOfPoints, vecOfHashTables);
            insertRestOfPoints(vecOfPoints, vecOfCentroids);
            updateCentroids(vecOfCentroids, numOfClusters);
            float dist;
            for (int j = 0; j < numOfClusters; j++)
            {
                dist = calculateDistance(temp[j].coordinates, vecOfCentroids[j].coordinates);
                if (dist < 85)
                    flag++;
                else
                    flag = 0;
            }
            temp = vecOfCentroids;
            countIt++;
        }
        // now let's check which items did not get assigned to any cluster...

        using sec = std::chrono::duration<double>;
        sec end = clock::now() - begin;

        ofstream outputFileStream;
        outputFileStream.open(outputFile);
        outputFileStream << "Algorithm: Range Search LSH\n";
        printToFile(outputFileStream, vecOfCentroids, end, complete);
    }
    else if (strcmp(method, "HYPER") == 0)
    {
        // HYPERCUBE REVERSE
        lshCon.k = kCUBE;
        lshCon.w = w;
        lshCon.L = M;
        hashTableCube *hashT = new hashTableCube(myPow(2, kCUBE), &lshCon, numOfDimensions);
        for (int i = 0; i < numOfLines; i++)
            hashT->insert(&vecOfPoints[i]);
        vector<centroid> temp = vecOfCentroids;
        int maxIt = 100;
        int countIt = 1;
        int flag = 0;

        using clock = std::chrono::system_clock;
        auto begin = clock::now();

        while (countIt < maxIt && flag < numOfClusters)
        {
            flag = 0;
            //cout << currMeanDistance << endl;
            //lshReverse(vecOfCentroids, vecOfPoints, vecOfHashTables);
            hyperCubeReverse(vecOfCentroids, vecOfPoints, hashT, probes, M);
            insertRestOfPoints(vecOfPoints, vecOfCentroids);
            updateCentroids(vecOfCentroids, numOfClusters);
            float dist = 0;

            for (int j = 0; j < numOfClusters; j++)
            {
                dist += calculateDistance(temp[j].coordinates, vecOfCentroids[j].coordinates);
                if (dist < 85)
                    flag++;
                else
                    flag = 0;
            }
            temp = vecOfCentroids;
            countIt++;
            cout << countIt << endl;
        }

        // now let's check which items did not get assigned to any cluster...

        using sec = std::chrono::duration<double>;
        sec end = clock::now() - begin;

        ofstream outputFileStream;
        outputFileStream.open(outputFile);
        outputFileStream << "Algorithm: Range Search HYPER CUBE\n";
        printToFile(outputFileStream, vecOfCentroids, end, complete);
    }

    return 0;
}
