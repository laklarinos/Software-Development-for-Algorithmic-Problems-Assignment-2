#include "cube.h"

int cube(char *inputFile, char *queryFile, char *outputFile, cubeConstants &cubeCon)
{
    int k = cubeCon.k;
    int M = cubeCon.M;
    int numberOfNN = 1;
    int probes = cubeCon.probes;
    int rad;
    int w = cubeCon.w;

    int numOfLines;
    int numOfElements;
    int numOfDimensions;

    // for query file
    int numOfLinesQ;
    int numOfElementsQ;
    int numOfDimensionsQ;

    fstream inputFilePtr;
    fstream queryFilePtr;
    fstream outputFilePtr;

    vector<point> arrayOfPoints;

    int res = parsInit(inputFile, arrayOfPoints, &numOfLines, &numOfElements);

    numOfDimensions = numOfElements - 1;

    if (res == 1)
    {
        cout << "Error when parsing the file\n";
        return 1;
    }

    if (arrayOfPoints.empty())
    {
        cout << "Array of points is empty\n";
        return 1;
    }
    // now lets start the insertion...

    hashTableCube *hashT;
    hashT = new hashTableCube(myPow(2, k), &cubeCon, numOfDimensions);

    for (int i = 0; i < numOfLines; i++)
        hashT->insert(&arrayOfPoints[i]);

    vector<point> arrayOfQueryPoints;
    res = parsInit(queryFile, arrayOfQueryPoints, &numOfLinesQ, &numOfElementsQ);

    numOfDimensionsQ = numOfElementsQ - 1;
    if (res == 1)
    {
        return 1;
    }

    if (arrayOfQueryPoints.empty())
    {
        return 1;
    }

    vector<kNearest> list;
    vector<kNearest> listTrue;
    vector<kNearest> listR;

    list.resize(numOfLinesQ);
    listTrue.resize(numOfLinesQ);
    //listR.resize(numOfLinesQ);

    ofstream outputFileStream;

    outputFileStream.open(outputFile, std::ios_base::app);
    // we have initialized an array of objects-points from query file...
    // now let's find their nearest neighbors...
    float MAF = -1;
    float averageAppDist = 0.0;
    float averageTruDist = 0.0;
    int c = 0;
    for (int i = 0; i < numOfLinesQ; i++)
    {
        initKNearest(numberOfNN, &(list[i]));
        initKNearest(numberOfNN, &(listTrue[i]));
        //initKNearest(0, &(listR[i]));

        std::chrono::steady_clock::time_point beginHyper = std::chrono::steady_clock::now();
        hashT->findKNeighbors(&(arrayOfQueryPoints[i]), &list[i], probes, M);
        std::chrono::steady_clock::time_point endHyper = std::chrono::steady_clock::now();

        std::chrono::steady_clock::time_point beginTrue = std::chrono::steady_clock::now();
        hashT->findKNeighborsTrue(&(arrayOfQueryPoints[i]), &listTrue[i], probes, M);
        std::chrono::steady_clock::time_point endTrue = std::chrono::steady_clock::now();

        //hashT->findNeighborsR(&(arrayOfQueryPoints[i]), &listR[i], rad, probes, M);

        outputFileStream << "Query: " << arrayOfQueryPoints[i].getKey() << endl;
        outputFileStream << "Algorithm: Hypercube" << endl;
        int tempId = -1;

        for (int z = 0; z < list[i].size; z++)
        {
            int flag = 0;
            if (list[i].dist[z] != MAXFLOAT)
            {
                // we have a neighbor
                outputFileStream << "Approximate nearest neighbor"
                                 << ": " << list[i].nearestPoints[z]->getKey() << endl;
                outputFileStream << "Approximate distance: " << list[i].dist[z] << endl;
                if (list[i].dist[z] != MAXFLOAT)
                {
                    float temp = list[i].dist[z] / listTrue[i].dist[z];
                    if (temp > MAF)
                    {
                        MAF = temp;
                    }
                    flag = 1;
                    // we have a neighbor
                    outputFileStream << "True nearest neighbor"
                                     << ": " << listTrue[i].nearestPoints[z]->getKey() << endl;
                    outputFileStream << "True distance: " << listTrue[i].dist[z] << endl;
                }
                c++;
                averageAppDist += list[i].vecOfTimes[z].count();
                averageTruDist += listTrue[i].vecOfTimes[z].count();

                outputFileStream << "   tHYPER: " << list[i].vecOfTimes[z].count() << " ms" << endl;
                if (flag)
                    outputFileStream << "   tTrue: " << listTrue[i].vecOfTimes[z].count() << " ms" << endl
                                     << endl;
            }
        }

        // outputFileStream << "R-nearest neigbors: " << endl;
        // for (int w = 0; w < listR[i].size; w++)
        // {
        //     outputFileStream << "   item_id: " << listR[i].nearestPoints[w]->getKey() << endl;
        // }
        outputFileStream << "\n////////////////////////////////////////////////////////////\n";
    }
    if (c != 0)
    {
        outputFileStream << "tApproximateAverage: " << averageAppDist / c << " ms" << endl;
        outputFileStream << "tTrueAverage: " << averageTruDist / c << " ms" << endl;
    }
    outputFileStream << "MAF: <double> " << MAF << endl;
    outputFileStream.close();
    return 0;
}
