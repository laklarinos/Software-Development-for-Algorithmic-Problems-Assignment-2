#include "../helpers/parsingInit.h"
#include "hashtable.h"

using namespace std;

int lsh(char *inputFile, char *queryFile, char *outputFile, lshConstants &lshCon, char *frechet, char *metric, double delta)
{
    int numOfLines;
    int numOfElements;
    int numOfDimensions;

    int L = lshCon.L;
    int k = lshCon.k;
    int w = lshCon.w;
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

    int numberOfNN = 5;
    int rad = 10;

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

    vector<hashTable *> arrayOfHashTables(L);
    for (int i = 0; i < L; i++)
    {
        arrayOfHashTables[i] = new hashTable(numOfLines / 4, &lshCon, numOfDimensions);
    }

    if (strcmp(frechet, "yes") == 0)
    {
        // grid
        // need to create curves...
        vector<map<double, double>> arrayOfCurves(numOfLines);
        vector<pair<double, double>> arrayOfTxAndTy(L);
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(0.0, delta);

        // generate t's for grid
        for (int i = 0; i < L; i++)
        {
            double tx = distribution(generator);
            double ty = distribution(generator);
            arrayOfTxAndTy[i] = pair<double, double>(tx, ty);
        }

        // initialize the curves, x 1 ... N (where N the number of Points a curve has)
        for (int i = 0; i < numOfLines; i++)
        {
            point *curPoint = &arrayOfPoints[i];
            for (int j = 0; j < numOfDimensions; j++)
            {
                double dValue = curPoint->pVector[j];
                arrayOfCurves[i].insert(pair<int, double>(j + 1, dValue));
            }
        }

        vector<vector<double>> arrayOfSnappedCurves(numOfLines);
        for (int i = 0; i < numOfLines; i++)
        {
            for (int j = 0; j < L; j++)
            {
                // snaps the curves on the grid, deletes duplicates and returns a vector
                vector<double> temp;
                temp = snap(numOfDimensions, arrayOfCurves[i], delta, arrayOfTxAndTy[j]);

                //  padding
                int sizeOfVector = temp.size();
                for (int i = sizeOfVector - 1; i < 2 * numOfDimensions; i++)
                {
                    temp.push_back(INT_MAX - 5000); // a number that does not exist in the data set
                }
                // in order to insert the curve in the hash table, we pass the modified curve "temp" 
                // from the hash functions which will give us the bucket number. Then we insert the actual 
                // curve.

                // I need to create a curve class 
                
            }
        }
    }
    else if (strcmp(frechet, "no") == 0)
    {
    }

    ////////////////////////////////////////////////////////////////
    //exit(1);

    // vector<kNearest> list;
    // vector<kNearest> listTrue;
    // vector<kNearest> listR;

    // list.resize(numOfLinesQ);
    // listTrue.resize(numOfLinesQ);
    // listR.resize(numOfLinesQ);

    // ofstream outputFileStream;

    // outputFileStream.open(outputFile, std::ios_base::app);

    // for (int i = 0; i < numOfLinesQ; i++)
    // {
    //     int flag = 0;
    //     initKNearest(numberOfNN, &(list[i]));
    //     initKNearest(numberOfNN, &(listTrue[i]));
    //     initKNearest(0, &(listR[i]));
    //     for (int j = 0; j < L; j++)
    //     {
    //         arrayOfHashTables[j]->findKNeighbors(&(arrayOfQueryPoints[i]), &list[i]);
    //         arrayOfHashTables[j]->findKNeighborsTrue(&(arrayOfQueryPoints[i]), &listTrue[i]);
    //         arrayOfHashTables[j]->findNeighborsR(&(arrayOfQueryPoints[i]), &listR[i], rad);
    //     }
    //     outputFileStream << "Query: " << arrayOfQueryPoints[i].getKey() << endl;
    //     int tempId = -1;
    //     for (int z = 0; z < list[i].size; z++)
    //     {
    //         if (list[i].dist[z] != MAXFLOAT)
    //         {
    //             // we have a neighbor
    //             outputFileStream << "Nearest neighbor-" << z + 1 << ": " << list[i].nearestPoints[z]->getKey() << endl;
    //             outputFileStream << "distanceLSH: " << list[i].dist[z] << endl;
    //             if (listTrue[i].dist[z] != MAXFLOAT)
    //             {
    //                 // we have a neighbor
    //                 flag = 1;
    //                 outputFileStream << "distanceTrue: " << listTrue[i].dist[z] << endl;
    //             }
    //             outputFileStream << "   tLSH: " << list[i].vecOfTimes[z].count() << " ms" << endl;
    //             //if (flag)
    //             outputFileStream << "   tTrue: " << listTrue[i].vecOfTimes[z].count() << " ms" << endl
    //                              << endl;
    //         }
    //     }

    //     outputFileStream << "R-nearest neigbors: " << endl;

    //     for (int w = 0; w < listR[i].size; w++)
    //     {
    //         outputFileStream << "item_id: " << listR[i].nearestPoints[w]->getKey() << endl;
    //     }
    //     outputFileStream << "\n////////////////////////////////////////////////////////////\n";
    // }

    // we have initialized an array of objects-points from query file...
    // now let's find their nearest neighbors...

    //outputFileStream.close();

    for (int i = 0; i < L; i++)
    {
        delete arrayOfHashTables[i];
    }
    return 0;
}
