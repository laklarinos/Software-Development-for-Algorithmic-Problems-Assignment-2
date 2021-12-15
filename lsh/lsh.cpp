#include "lsh.h"

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

    if (strcmp(frechet, "yes") == 0 && strcmp(metric, "discrete") == 0)
    {
        vector<point> arrayOfCurves1D;
        int res = parsInit(inputFile, arrayOfCurves1D, &numOfLines, &numOfElements);

        numOfDimensions = numOfElements - 1;

        int numberOfNN = 5;
        int rad = 10;

        if (res == 1)
        {
            cout << "Error when ParsInit\n";
            exit(1);
        }

        if (arrayOfCurves1D.empty())
        {
            cout << "Array Of Points is Empty\n";
            exit(1);
        }

        vector<hashTable *> arrayOfHashTables(L);
        for (int i = 0; i < L; i++)
        {
            arrayOfHashTables[i] = new hashTable(numOfLines / 4, &lshCon, numOfDimensions, 1);
        }

        // need to create curves...
        vector<map<float, float>> arrayOfCurves(numOfLines);
        vector<pair<float, float>> arrayOfTxAndTy(L);
        std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(0.0, delta);

        // generate t's for grid
        for (int i = 0; i < L; i++)
        {
            float tx = distribution(generator);
            float ty = distribution(generator);
            arrayOfTxAndTy[i] = pair<float, float>(tx, ty);
        }

        // initialize the curves, x 1 ... N (where N the number of Points a curve has)
        for (int i = 0; i < numOfLines; i++)
        {
            point *curPoint = &arrayOfCurves1D[i];
            for (int j = 0; j < numOfDimensions; j++)
            {
                float dValue = curPoint->pVector[j];
                arrayOfCurves[i].insert(pair<int, float>(j + 1, dValue));
            }
        }

        // insert timeseries to hash tables...
        vector<vector<float>> arrayOfSnappedCurves(numOfLines);
        for (int i = 0; i < numOfLines; i++)
        {
            for (int j = 0; j < L; j++)
            {
                // snaps the curves on the grid, deletes duplicates and returns a vector
                vector<float> temp;
                temp = snap(numOfDimensions, arrayOfCurves[i], delta, arrayOfTxAndTy[j]);

                //  padding
                int sizeOfVector = temp.size();
                for (int i = sizeOfVector - 1; i < 2 * numOfDimensions; i++)
                {
                    temp.push_back(500); // a number that does not exist in the data set
                }

                int key = arrayOfHashTables[j]->insert(&(arrayOfCurves1D[i]), temp);
                // in order to insert the curve in the hash table, we pass the modified curve "temp"
                // from the hash functions which will give us the bucket number. Then we insert the actual
                // curve.
                // I need to create a curve class
            }
        }

        // read query file...
        vector<point> arrayOfCurves1DQ;
        res = parsInit(queryFile, arrayOfCurves1DQ, &numOfLinesQ, &numOfElementsQ);
        numOfDimensionsQ = numOfElementsQ - 1;

        if (res == 1)
        {
            cout << "Error when ParsInit\n";
            exit(1);
        }

        if (arrayOfCurves1DQ.empty())
        {
            cout << "Array Of Points is Empty\n";
            exit(1);
        }

        vector<kNearest> list;
        vector<kNearest> listTrue;
        vector<kNearest> listR;

        list.resize(numOfLinesQ);
        listTrue.resize(numOfLinesQ);
        listR.resize(numOfLinesQ);

        ofstream outputFileStream;

        outputFileStream.open(outputFile, std::ios_base::app);

        numberOfNN = 1;
        vector<map<float, float>> arrayOfCurvesQ(numOfLinesQ);

        for (int i = 0; i < numOfLinesQ; i++)
        {
            point *curPoint = &arrayOfCurves1DQ[i];
            for (int j = 0; j < numOfDimensions; j++)
            {
                float dValue = curPoint->pVector[j];
                arrayOfCurvesQ[i].insert(pair<int, float>(j + 1, dValue));
            }
        }

        vector<vector<vector<float>>> arrayOfCurves1DQSnapped(numOfLinesQ);
        for (int i = 0; i < numOfLinesQ; i++)
        {
            vector<vector<float>> arrayOfSnappedCurvePerHashTable(L);
            for (int j = 0; j < L; j++)
            {
                vector<float> temp;
                temp = snap(numOfDimensions, arrayOfCurvesQ[i], delta, arrayOfTxAndTy[j]);
                //  padding
                int sizeOfVector = temp.size();
                for (int i = sizeOfVector - 1; i < 2 * numOfDimensions; i++)
                {
                    temp.push_back(INT_MAX - 5000); // a number that does not exist in the data set
                }
                arrayOfSnappedCurvePerHashTable[j] = temp;
            }
            arrayOfCurves1DQSnapped[i] = arrayOfSnappedCurvePerHashTable;
        }

        float MAF = -1;
        float averageAppDist = 0.0;
        float averageTruDist = 0.0;
        int c = 0;

        for (int i = 0; i < numOfLinesQ; i++)
        {
            int flag = 0;
            initKNearest(numberOfNN, &(list[i]));
            initKNearest(numberOfNN, &(listTrue[i]));
            initKNearest(0, &(listR[i]));

            for (int j = 0; j < L; j++)
            {
                point temPoint(arrayOfCurves1DQSnapped[i][j]);
                arrayOfHashTables[j]->findKNeighbors(&arrayOfCurves1DQ[i], &list[i], arrayOfCurves1DQSnapped[i][j]);
                arrayOfHashTables[j]->findKNeighborsTrue(&arrayOfCurves1DQ[i], &listTrue[i], arrayOfCurves1DQSnapped[i][j]);
            }

            outputFileStream << "Query: " << arrayOfCurves1DQ[i].getKey() << endl;
            outputFileStream << "Algorithm:  LSH_Frechet_Discrete" << endl;
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
    else if (strcmp(frechet, "no") == 0)
    {
        vector<point> arrayOfPoints;
        int res = parsInit(inputFile, arrayOfPoints, &numOfLines, &numOfElements);

        numOfDimensions = numOfElements - 1;

        int numberOfNN = 5;
        int rad = 10;

        if (res == 1)
        {
            cout << "Error when ParsInit\n";
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

        for (int i = 0; i < numOfLines; i++)
        {
            for (int j = 0; j < L; j++)
            {
                arrayOfHashTables[j]->insert(&arrayOfPoints[i]);
            }
        }

        vector<point> arrayOfQueryPoints;
        res = parsInit(inputFile, arrayOfQueryPoints, &numOfLinesQ, &numOfElementsQ);

        numOfDimensionsQ = numOfElementsQ - 1;
        if (res == 1)
        {
            exit(1);
        }

        if (arrayOfQueryPoints.empty())
        {
            exit(1);
        }

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
                arrayOfHashTables[j]->findKNeighbors(&(arrayOfQueryPoints[i]), &list[i]);
                arrayOfHashTables[j]->findKNeighborsTrue(&(arrayOfQueryPoints[i]), &listTrue[i]);
                arrayOfHashTables[j]->findNeighborsR(&(arrayOfQueryPoints[i]), &listR[i], rad);
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
        outputFileStream.close();

        for (int i = 0; i < L; i++)
        {
            delete arrayOfHashTables[i];
        }
    }

    ////////////////////////////////////////////////////////////////
    //exit(1);

    // we have initialized an array of objects-points from query file...
    // now let's find their nearest neighbors...
    return 0;
}
