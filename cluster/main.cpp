#include "../helpers/helpers.h"
#include "checkcommandlinecluster.h"
#include "cluster.h"

using namespace std;

// $./cluster –i <input file> –c <configuration file> -o <output file> -update <Mean
// Frechet or Mean Vector> –assignment <Classic or LSH or Hypercube or LSH_Frechet>
// -complete <optional> -silhouette <optional>

int main(int argc, char *argv[])
{
    char *inputFile;
    char *outputFile;
    char *confFile;
    char *assignment;
    char *update;
    char *silhouette;

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

    // for query file
    int numOfLinesQ;
    int numOfElementsQ;
    int numOfDimensionsQ;

    int checkArgs = checkCommandLineCluster(argc, argv);
    if (checkArgs)
    {
        cout << "Problem with checking command line arguments\n";
        exit(1);
    }

    //init arguments
    inputFile = argv[2];
    confFile = argv[4];
    outputFile = argv[6];
    update = argv[8];
    assignment = argv[10];
    silhouette = argv[12];

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
    vector<centroid> vecOfCentroids;

    int res = parsInit(inputFile, vecOfPoints, &numOfLines, &numOfElements);
    numOfDimensions = numOfElements - 1;

    if (res == 1)
    {
        cout << "Problem when parsing the file\n";
        exit(1);
    }

    if (vecOfPoints.empty())
    {
        cout << "Problem when parsing the file\n";
        exit(1);
    }

    initCentroids(vecOfCentroids, numOfClusters, vecOfPoints);
    vector<curveMine> vecOfCurves;

    // K-MEANS ++, INITIALIZATION
    initCentroids(vecOfCentroids, numOfClusters, vecOfPoints);
    
    // LLOYD, LSH REVERSE, HYPERCUBE REVERSE, ASSIGNMENT

    if (strcmp(assignment, "Classic") == 0)
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

            if (strcmp(update, "Mean_Vector") == 0)
                lloyd(vecOfCentroids, vecOfPoints);

            if (strcmp(update, "Mean_Frechet") == 0)
                lloyd(vecOfCentroids, vecOfPoints, 1);

            updateCentroids(vecOfCentroids, numOfClusters);

            float dist;
            for (int j = 0; j < numOfClusters; j++)
            {
                dist = euclDistance(temp[j].coordinates, vecOfCentroids[j].coordinates);
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
        printToFile(outputFileStream, vecOfCentroids, end, silhouette);
    }
    else if (strcmp(assignment, "LSH") == 0)
    {
        // LSH REVERSE
        lshConstants lshCon;

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
            lshReverse(vecOfCentroids, vecOfPoints, vecOfHashTables);
            insertRestOfPoints(vecOfPoints, vecOfCentroids);
            updateCentroids(vecOfCentroids, numOfClusters);
            float dist;
            for (int j = 0; j < numOfClusters; j++)
            {
                dist = euclDistance(temp[j].coordinates, vecOfCentroids[j].coordinates);
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
        outputFileStream << "Algorithm: Range Search LSH\n";
        printToFile(outputFileStream, vecOfCentroids, end, silhouette);
    }
    else if (strcmp(assignment, "HYPER") == 0)
    {
        // HYPERCUBE REVERSE
        cubeConstants cubCon;
        cubCon.k = kCUBE;
        cubCon.w = w;
        cubCon.L = L;
        cubCon.probes = probes;
        cubCon.M = M;

        hashTableCube *hashT = new hashTableCube(myPow(2, kCUBE), &cubCon, numOfDimensions);
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
            hyperCubeReverse(vecOfCentroids, vecOfPoints, hashT, probes, M);
            insertRestOfPoints(vecOfPoints, vecOfCentroids);
            updateCentroids(vecOfCentroids, numOfClusters);
            float dist = 0;

            for (int j = 0; j < numOfClusters; j++)
            {
                dist += euclDistance(temp[j].coordinates, vecOfCentroids[j].coordinates);
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
        outputFileStream << "Algorithm: Range Search HYPER CUBE\n";
        printToFile(outputFileStream, vecOfCentroids, end, silhouette);
    }

    return 0;
}
