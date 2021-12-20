#include "lsh.h"

int lsh(char *inputFile, char *queryFile, char *outputFile, lshConstants &lshCon, char *frechet, char *metric, double delta)
{
    int numOfLines;
    int numOfElements;
    int numOfPointsInLine;

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

    vector<point> arrayOfCurves1D;
    int res = parsInit(inputFile, arrayOfCurves1D, &numOfLines, &numOfElements);

    numOfPointsInLine = numOfElements - 1;

    int numberOfNN = 1;
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

    list.resize(numOfLinesQ);
    listTrue.resize(numOfLinesQ);

    if (strcmp(frechet, "yes") == 0 && strcmp(metric, "discrete") == 0)
    {
        //init hash tables...
        vector<hashTable *> arrayOfHashTables(L);
        for (int i = 0; i < L; i++)
        {
            arrayOfHashTables[i] = new hashTable(numOfLines / 4, &lshCon, numOfPointsInLine, 1);
        }

        // need to create curves...
        //vector<map<float, float>> arrayOfCurves(numOfLines);
        vector<vector<pair<float, float>>> arrayOfCurves2D(numOfLines);
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
            for (int j = 0; j < numOfPointsInLine; j++)
            {
                float dValue = curPoint->pVector[j];
                arrayOfCurves2D[i].push_back(pair<float, float>(j + 1, dValue));
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
                temp = snap(2, arrayOfCurves2D[i], delta, arrayOfTxAndTy[j]);

                //  padding
                int sizeOfVector = temp.size();
                for (int i = sizeOfVector - 1; i < 2 * numOfPointsInLine; i++)
                {
                    temp.push_back(5000); // a number that does not exist in the data set
                }

                int key = arrayOfHashTables[j]->insert(&(arrayOfCurves1D[i]), temp);
                // in order to insert the curve in the hash table, we pass the modified curve "temp"
                // from the hash functions which will give us the bucket number. Then we insert the actual
                // curve.
                // I need to create a curve class
            }
        }

        // read query file...

        numberOfNN = 1;

        // create 2d curves to snap on grid...
        vector<vector<pair<float, float>>> arrayOfCurvesQ(numOfLinesQ);

        for (int i = 0; i < numOfLinesQ; i++)
        {
            point *curPoint = &arrayOfCurves1DQ[i];
            for (int j = 0; j < numOfPointsInLine; j++)
            {
                float dValue = curPoint->pVector[j];
                arrayOfCurvesQ[i].push_back(pair<float, float>(j + 1, dValue));
            }
        }

        // create snapped, vectorized, padded curves...
        vector<vector<vector<float>>> arrayOfCurves1DQSnapped(numOfLinesQ);
        for (int i = 0; i < numOfLinesQ; i++)
        {
            vector<vector<float>> arrayOfSnappedCurvePerHashTable(L);
            for (int j = 0; j < L; j++)
            {
                vector<float> temp;
                temp = snap(2, arrayOfCurvesQ[i], delta, arrayOfTxAndTy[j]);
                //  padding
                int sizeOfVector = temp.size();
                for (int i = sizeOfVector - 1; i < 2 * numOfPointsInLine; i++)
                {
                    temp.push_back(5000); // a number that does not exist in the data set
                }
                arrayOfSnappedCurvePerHashTable[j] = temp;
            }
            arrayOfCurves1DQSnapped[i] = arrayOfSnappedCurvePerHashTable;
        }

        // print to file...
        printToFile(outputFile, (char *)"LSH_Frechet_Discrete", list, listTrue, arrayOfHashTables, numberOfNN, arrayOfCurves1DQ, arrayOfCurves1DQSnapped);
    }
    else if (strcmp(frechet, "yes") == 0 && strcmp(metric, "continuous") == 0)
    {
        // filtering -> snapping -> minima/maxima -> vectorization -> padding
        // init hashtables...
        L = 1;
        vector<hashTable *> arrayOfHashTables(L);
        for (int i = 0; i < L; i++)
        {
            arrayOfHashTables[i] = new hashTable(numOfLines / 4, &lshCon, numOfPointsInLine, 1, 1);
        }

        vector<vector<pair<float, float>>> arrayOfCurves2D(numOfLines);
        pair<float, float> arrayOfTxAndTy;
        std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(0.0, delta);

        // create 2d curve from input line...
        for (int i = 0; i < numOfLines; i++)
        {
            point *curPoint = &arrayOfCurves1D[i];
            for (int j = 0; j < numOfPointsInLine - 1; j++)
            {
                float dValue = curPoint->pVector[j];
                arrayOfCurves2D[i].push_back(pair<int, float>(j + 1, dValue));
            }
        }

        float epsilon = 1.001;
        float tx = distribution(generator);
        float ty = distribution(generator);
        arrayOfTxAndTy = pair<float, float>(tx, ty);

        // for each line, filter it, snap it, minima-maxima it, pad it...
        for (int i = 0; i < numOfLines; i++)
        {
            int sizeOfMap = arrayOfCurves2D[i].size();
            
            //filtering
            //result is a vector with pairs but filtered
            for (int j = 0; j < sizeOfMap - 3; j++)
            {

                pair<float, float> pairA = arrayOfCurves2D[i][j];
                pair<float, float> pairB = arrayOfCurves2D[i][j + 1];
                pair<float, float> pairC = arrayOfCurves2D[i][j + 2];
                //if |a − b| ≤ e and |b − c| ≤ e
                if (abs(pairA.second - pairB.second) <= epsilon && abs(pairB.second - pairC.second) <= epsilon)
                {
                    j += 2;
                    //erase element
                    arrayOfCurves2D[i][j] = pair<float, float>(-1, -1);
                }
            }

            // erase all elements == pair<float,float>(-1,-1)
            arrayOfCurves2D[i].erase(std::remove(arrayOfCurves2D[i].begin(), arrayOfCurves2D[i].end(), pair<float, float>(-1, -1)), arrayOfCurves2D[i].end()); //erase remove technique

            // keep track of the filtered curve, save only the y's...
            vector<float> filteredCurve;
            for (int k = 0; k < arrayOfCurves2D[i].size(); k++)
            {
                filteredCurve.push_back(arrayOfCurves2D[i][k].second); //only y's
            }

            //keep track of the filtered curve...
            arrayOfCurves1D[i].filteredCurve = filteredCurve;

            // snaps the curves on the grid, deletes duplicates and returns a vector
            vector<float> temp;
            temp = snap(1, arrayOfCurves2D[i], delta, arrayOfTxAndTy);

            // padding
            int sizeOfVector = temp.size();
            for (int i = sizeOfVector - 1; i < numOfPointsInLine; i++)
            {
                // a number that does not exist in the data set
                temp.push_back(5000);
            }

            // insert input curve to hashTable
            int key = arrayOfHashTables[0]->insert(&(arrayOfCurves1D[i]), temp);
        }

        /// FOR QUERY ///
        vector<vector<pair<float, float>>> arrayOfCurvesQ2D(numOfLinesQ);

        // create 2d curve out of 1d query line
        for (int i = 0; i < numOfLinesQ; i++)
        {
            point *curPoint = &arrayOfCurves1DQ[i];
            for (int j = 0; j < numOfPointsInLine; j++)
            {
                float dValue = curPoint->pVector[j];
                arrayOfCurvesQ2D[i].push_back(pair<float, float>(j + 1, dValue));
            }
        }

        vector<vector<vector<float>>> arrayOfCurves1DQSnapped(numOfLinesQ);

        //for each query, filter it, snap it, minima-maxima it, pad it
        for (int i = 0; i < numOfLinesQ; i++)
        {
            int sizeOfMap = arrayOfCurvesQ2D[i].size();

            // filtering
            for (int j = 0; j < sizeOfMap - 3; j++)
            {

                pair<float, float> pairA = arrayOfCurvesQ2D[i][j];
                pair<float, float> pairB = arrayOfCurvesQ2D[i][j + 1];
                pair<float, float> pairC = arrayOfCurvesQ2D[i][j + 2];
                //if |a − b| ≤ e and |b − c| ≤ e
                if (abs(pairA.second - pairB.second) <= epsilon && abs(pairB.second - pairC.second) <= epsilon)
                {
                    j += 2;
                    //erase element
                    arrayOfCurvesQ2D[i][j] = pair<float, float>(-1, -1);
                }
            }

            // erase all elements == pair<float, float>(-1, -1)
            arrayOfCurvesQ2D[i].erase(std::remove(arrayOfCurvesQ2D[i].begin(), arrayOfCurvesQ2D[i].end(), pair<float, float>(-1, -1)), arrayOfCurvesQ2D[i].end());

            // save the filtered but keep y's only
            vector<float> filteredQueryCurve1D;
            for (int k = 0; k < arrayOfCurvesQ2D[i].size(); k++)
            {
                filteredQueryCurve1D.push_back(arrayOfCurvesQ2D[i][k].second);
            }
            arrayOfCurves1DQ[i].filteredCurve = filteredQueryCurve1D;

            // keep track of the snapped curves...
            vector<vector<float>> arrayOfSnappedCurveQPerHashTable(1);
            vector<float> temp;
            temp = snap(1, arrayOfCurvesQ2D[i], delta, arrayOfTxAndTy);

            // padding
            int sizeOfVector = temp.size();
            for (int i = sizeOfVector - 1; i < numOfPointsInLine; i++)
            {
                temp.push_back(5000); // a number that does not exist in the data set
            }
            arrayOfSnappedCurveQPerHashTable[0] = temp;
            arrayOfCurves1DQSnapped[i] = arrayOfSnappedCurveQPerHashTable;
        }

        printToFile(outputFile, (char *)"LSH_Frechet_Continuous", list, listTrue, arrayOfHashTables, numberOfNN, arrayOfCurves1DQ, arrayOfCurves1DQSnapped);
    }
    else if (strcmp(frechet, "no") == 0)
    {

        vector<hashTable *> arrayOfHashTables(L);
        for (int i = 0; i < L; i++)
        {
            arrayOfHashTables[i] = new hashTable(numOfLines / 4, &lshCon, numOfPointsInLine);
        }

        for (int i = 0; i < numOfLines; i++)
        {
            for (int j = 0; j < L; j++)
            {
                arrayOfHashTables[j]->insert(&arrayOfCurves1D[i]);
            }
        }

        printToFile(outputFile, (char *)"LSH_Algorithm", list, listTrue, arrayOfHashTables, numberOfNN, arrayOfCurves1DQ);

        // for (int i = 0; i < L; i++)
        // {
        //     delete arrayOfHashTables[i];
        // }
    }

    ////////////////////////////////////////////////////////////////
    //exit(1);

    // we have initialized an array of objects-points from query file...
    // now let's find their nearest neighbors...
    return 0;
}
