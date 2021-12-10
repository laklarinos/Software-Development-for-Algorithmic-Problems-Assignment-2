#include "../parsingInit.h"
#include "hashtable.h"

using namespace std;

int main(int argc, char *argv[])
{
    char *inputFile;
    char *outputFile;
    char *queryFile;

    int k;
    int L;
    int numberOfNN;
    int rad;
    int w = 3;

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

    //define arguments
    inputFile = argv[2];
    queryFile = argv[4];
    outputFile = argv[10];
    k = atoi(argv[6]);
    L = atoi(argv[8]);
    numberOfNN = atoi(argv[12]);
    rad = atoi(argv[14]);

    lshCon.k = k;
    lshCon.w = w;
    lshCon.L = L;

    fstream inputFilePtr;
    fstream queryFilePtr;
    fstream outputFilePtr;

    vector<point> arrayOfPoints;

    int res = parsInit(inputFile, arrayOfPoints, &numOfLines, &numOfElements);
    numOfDimensions = numOfElements - 1;

    //return EXIT_SUCCESS;

    if (res == 1)
    {
        cout << "Error when ParInit\n";
        exit(1);
    }

    if (arrayOfPoints.empty())
    {
        cout << "Array Of Points is Empty\n";
        exit(1);
    }

    // return 1;

    // now lets start the insertion...

    vector<hashTable *> hashTablesArray(L);
    for (int i = 0; i < L; i++)
    {
        hashTablesArray[i] = new hashTable(numOfLines / 4, &lshCon, numOfDimensions);
    }

    for (int i = 0; i < numOfLines; i++)
    {
        for (int j = 0; j < L; j++)
        {
            hashTablesArray[j]->insert(&arrayOfPoints[i]);
        }
    }

    vector<point> arrayOfQueryPoints;

    res = parsInit(inputFile, arrayOfQueryPoints, &numOfLinesQ, &numOfElementsQ);
    numOfDimensionsQ = numOfElementsQ - 1;

    if (res == 1)
    {
        cout << "Error when ParInit\n";
        exit(1);
    }

    if (arrayOfQueryPoints.empty())
    {
        cout << "ArrayOfQueryPoints is Empty\n";
        exit(1);
    }

    //exit(1);

    vector<kNearest> list;
    vector<kNearest> listTrue;
    vector<kNearest> listR;

    list.resize(numOfLinesQ);
    listTrue.resize(numOfLinesQ);
    listR.resize(numOfLinesQ);

    ofstream outputFileStream;

    outputFileStream.open(outputFile, std::ios_base::app);

    for (int i = 0; i < numOfLinesQ; i++)
    {
        int flag = 0;
        initKNearest(numberOfNN, &(list[i]));
        initKNearest(numberOfNN, &(listTrue[i]));
        initKNearest(0, &(listR[i]));
        for (int j = 0; j < L; j++)
        {
            hashTablesArray[j]->findKNeighbors(&(arrayOfQueryPoints[i]), &list[i]);
            hashTablesArray[j]->findKNeighborsTrue(&(arrayOfQueryPoints[i]), &listTrue[i]);
            hashTablesArray[j]->findNeighborsR(&(arrayOfQueryPoints[i]), &listR[i], rad);
        }
        outputFileStream << "Query: " << arrayOfQueryPoints[i].getKey() << endl;
        int tempId = -1;
        for (int z = 0; z < list[i].size; z++)
        {
            if (list[i].dist[z] != MAXFLOAT)
            {
                // we have a neighbor
                outputFileStream << "Nearest neighbor-" << z + 1 << ": " << list[i].nearestPoints[z]->getKey() << endl;
                outputFileStream << "distanceLSH: " << list[i].dist[z] << endl;
                if (listTrue[i].dist[z] != MAXFLOAT)
                {
                    // we have a neighbor
                    flag = 1;
                    outputFileStream << "distanceTrue: " << listTrue[i].dist[z] << endl;
                }
                outputFileStream << "   tLSH: " << list[i].vecOfTimes[z].count() << " ms" << endl;
                //if (flag)
                outputFileStream << "   tTrue: " << listTrue[i].vecOfTimes[z].count() << " ms" << endl
                                 << endl;
            }
        }

        outputFileStream << "R-nearest neigbors: " << endl;

        for (int w = 0; w < listR[i].size; w++)
        {
            outputFileStream << "item_id: " << listR[i].nearestPoints[w]->getKey() << endl;
        }
        outputFileStream << "\n////////////////////////////////////////////////////////////\n";
    }

    // we have initialized an array of objects-points from query file...
    // now let's find their nearest neighbors...

    outputFileStream.close();

    for (int i = 0; i < L; i++)
    {
        delete hashTablesArray[i];
    }
    return 0;
}
