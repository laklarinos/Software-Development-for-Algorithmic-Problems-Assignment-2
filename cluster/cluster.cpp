#include "cluster.h"

centroid::centroid(point *coordinates)
{
    this->coordinates = coordinates;
}

void centroid::includePoint(point *pGiven)
{
    this->clusterPoints.push_back(pGiven);
}

void updateCentroids(vector<centroid> &vecOfCentroids, int numOfClusters)
{
    vector<float> medVector;
    for (int i = 0; i < numOfClusters; i++)
    {
        centroid curCentroid = vecOfCentroids[i];
        vector<point *> vecOfClusterPoints = curCentroid.clusterPoints;
        int sizeOfCluster = vecOfClusterPoints.size();
        int centroidCoordinates = curCentroid.coordinates->pVector.size();
        int sum;
        medVector.push_back(0);
        for (int j = 1; j < centroidCoordinates; j++)
        {
            sum = 0;
            for (int z = 0; z < sizeOfCluster; z++)
            {
                sum = vecOfClusterPoints[z]->pVector[j] + sum;
            }
            medVector.push_back(sum);
        }
        if (sizeOfCluster != 0)
            for (int k = 0; k < centroidCoordinates; k++)
            {
                medVector[k] = medVector[k] / (sizeOfCluster);
            }

        point *pt = new point(medVector);
        vecOfCentroids[i].coordinates = pt;
        medVector.clear();
    }
}

void updateCentroidsCurve(vector<centroid> &vecOfCentroids, int numOfClusters)
{
    vector<float> medVector;
    for (int i = 0; i < numOfClusters; i++)
    {
        centroid curCentroid = vecOfCentroids[i];
        vector<point *> vecOfClusterPoints = curCentroid.clusterPoints;
        int sizeOfCluster = vecOfClusterPoints.size();
        int centroidCoordinates = curCentroid.coordinates->pVector.size();
        int sum;
        medVector.push_back(0);

        vector<vector<float>> vecOfArraysOfClusterPoints;
        for (int j = 0; j < sizeOfCluster; j++)
        {
            vecOfArraysOfClusterPoints.push_back(vecOfClusterPoints[j]->pVector);
        }

        // for (int j = 1; j < centroidCoordinates; j++)
        // {
        //     sum = 0;
        //     for (int z = 0; z < sizeOfCluster; z++)
        //     {
        //         sum = vecOfClusterPoints[z]->pVector[j] + sum;
        //     }
        //     medVector.push_back(sum);
        // }

        // if (sizeOfCluster != 0)
        //     for (int k = 0; k < centroidCoordinates; k++)
        //     {
        //         medVector[k] = medVector[k] / (sizeOfCluster);
        //     }

        point *pt = new point(medVector);
        vecOfCentroids[i].coordinates = pt;
        medVector.clear();
    }
}

void initCentroids(vector<centroid> &vecOfCentroids, int numOfClusters, vector<point> &vecOfPoints, int frechet)
{
    // kmeans ++
    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::uniform_int_distribution<int> uniDis(0, vecOfPoints.size() - 1);

    int numOfPairsInPoint = vecOfPoints[0].pVector.size() - 1;
    int numOfPoints = vecOfPoints.size();
    vector<float> tempInt = vecOfPoints[uniDis(gen1)].pVector;
    point *pt = new point(tempInt);
    vecOfCentroids.push_back(centroid(pt));

    for (int i = 1; i < numOfClusters; i++)
    {
        vector<float> vecOfSums;
        vector<float> vecOfDistances;
        vector<float> vecOfProb;

        float min = MAXFLOAT;
        float max = -1;
        float dist;
        int numOfPointsInCluster = vecOfPoints.size();

        for (int i = 0; i < numOfPointsInCluster; i++)
        {
            vector<float> tempInt = vecOfPoints[i].pVector;
            point *curPoint = new point(tempInt);

            for (int j = 0; j < i; j++)
            {
                point *centroidPoint = vecOfCentroids[j].coordinates;
                if (frechet == 0)
                {
                    dist = euclDistance(curPoint, centroidPoint);
                }
                else if (frechet == 1)
                {
                    dist = DFD(curPoint->pVector, centroidPoint->pVector);
                }

                if (dist > 0)
                {
                    if (dist < min)
                    {
                        min = dist;
                    }
                    if (dist > max)
                    {
                        max = dist;
                    }
                }
            }
            vecOfDistances.push_back(min);
            delete curPoint;
        }

        for (int z = 0; z < vecOfDistances.size(); z++)
        {
            vecOfDistances.at(i) = sqrt(vecOfDistances[z] / max);
        }

        int t;
        float sum = 0;
        vecOfProb.push_back(0);
        for (t = 0; t < vecOfDistances.size(); t++)
        {
            sum = sum + vecOfDistances[t];
            vecOfSums.push_back(sum);
            if (sum != 0)
                vecOfProb.push_back(vecOfDistances[t] / sum);
            else
                vecOfProb.push_back(0);
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> uniDis(0, vecOfSums.back());

        float cumulativeProbability = 0.0;
        for (t = 0; t < vecOfDistances.size(); t++)
        {
            cumulativeProbability += vecOfProb[t];
            if (cumulativeProbability >= uniDis(gen))
            {
                point *pt = new point(vecOfPoints[t].pVector);
                pt->pVector[0] = 0;
                vecOfCentroids.push_back(pt);
                break;
            }
        }
    }
}

void lshReverse(vector<centroid> &vecOfCentroids, vector<point> &vecOfPoints, vector<hashTable *> &vecOfHashTables)
{
    int sizeOfVecOfCentroids = vecOfCentroids.size();
    int sizeOfVecOfHashTables = vecOfHashTables.size();
    int sizeOfVecOfPoints = vecOfPoints.size();

    vector<kNearest> list(sizeOfVecOfCentroids);
    vector<map<int, double>> vecOfMaps(sizeOfVecOfCentroids);
    float min = MAXFLOAT;

    for (int i = 0; i < sizeOfVecOfCentroids; i++)
    {
        for (int j = 0; j < sizeOfVecOfCentroids; j++)
        {
            if (i == j)
                continue;
            float dist;
            dist = euclDistance(vecOfCentroids[i].coordinates, vecOfCentroids[j].coordinates);

            if (dist < min)
            {
                min = dist;
            }
        }
    }

    for (int i = 0; i < sizeOfVecOfCentroids; i++)
    {
        int R = min / 2;
        initKNearest(0, &list[i]);
        int prevSize = -1;
        int curSize = list[i].nearestPoints.size();
        int counter = 0;

        while (R > 0 && counter < 10)
        {
            counter++;
            point *centroidPoint = vecOfCentroids[i].coordinates;
            for (int j = 0; j < sizeOfVecOfHashTables; j++)
            {
                vecOfHashTables[j]->findNeighborsR(centroidPoint, &list[i], R);
            }
            prevSize = curSize;
            curSize = list[i].nearestPoints.size();
            R *= 2;
        }

        for (int k = 0; k < list[i].nearestPoints.size(); k++)
        {
            point *pointInserted = list[i].nearestPoints[k];
            int idOfPointInserted = pointInserted->pVector[0];
            double distOfPointInserted = list[i].dist[k];
            vecOfMaps[i].insert(pair<int, double>(idOfPointInserted, distOfPointInserted));
        }
    }

    for (int i = 0; i < sizeOfVecOfCentroids; i++)
    {
        int sizeOfNearestPointsList = list[i].nearestPoints.size();
        for (int j = 0; j < sizeOfNearestPointsList; j++)
        {
            point *pointToFind = list[i].nearestPoints[j];
            int idOfPointToFind = pointToFind->pVector[0];
            double distOfPointToFind = list[i].dist[j];

            vector<int> centroidsFound;
            centroidsFound.push_back(i);

            for (int z = 0; z < sizeOfVecOfCentroids; z++)
            {
                if (z == i)
                    continue;

                auto itOfOtherCentroid = vecOfMaps[z].find(idOfPointToFind);
                if (itOfOtherCentroid != vecOfMaps[z].end())
                {
                    // it was found...
                    centroidsFound.push_back(z);
                }
            }

            // now lets check which of the k centroids are closer to our point...
            double min = MAXFLOAT;
            int minValueCentroid;

            for (int w = 0; w < centroidsFound.size(); w++)
            {
                auto it = vecOfMaps[centroidsFound[w]].find(idOfPointToFind);
                double dist = it->second;
                if (dist < min)
                {
                    min = dist;
                    minValueCentroid = centroidsFound[w];
                }
            }

            // time to erase...
            for (int w = 0; w < centroidsFound.size(); w++)
            {
                auto it = vecOfMaps[centroidsFound[w]].find(idOfPointToFind);
                if (centroidsFound[w] != minValueCentroid)
                {
                    int sizeOfNearestPointsVec = list[w].nearestPoints.size();
                    for (int ii = 0; ii < sizeOfNearestPointsVec; ii++)
                    {
                        point *curPoint = list[w].nearestPoints[ii];
                        auto itToEraseNearestPoint = ii + list[w].nearestPoints.begin();
                        auto itToEraseDist = ii + list[w].dist.begin();
                        if (curPoint->pVector[0] == idOfPointToFind)
                        {
                            //erase
                            list[w].nearestPoints.erase(itToEraseNearestPoint);
                            list[w].dist.erase(itToEraseDist);
                            break;
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < sizeOfVecOfCentroids; i++)
    {
        vecOfCentroids[i].clusterPoints = list[i].nearestPoints;
        vecOfCentroids[i].initMap(i);
    }
}

void centroid::initMap(int clusterId)
{
    if (this->mapOfAssignedItems.empty() == 0)
    {
        this->mapOfAssignedItems.clear();
        int sizeOfVecOfPoints = this->clusterPoints.size();

        for (int i = 0; i < sizeOfVecOfPoints; i++)
        {
            point *pt = this->clusterPoints[i];
            this->mapOfAssignedItems.insert(pair<int, int>(pt->pVector[0], clusterId));
        }
    }
    else
    {
        int sizeOfVecOfPoints = this->clusterPoints.size();

        for (int i = 0; i < sizeOfVecOfPoints; i++)
        {
            point *pt = this->clusterPoints[i];
            this->mapOfAssignedItems.insert(pair<int, int>(pt->pVector[0], clusterId));
        }
    }
}

void insertRestOfPoints(vector<point> &vecOfPoints, vector<centroid> &vecOfCentroids)
{
    int numOfCentroids = vecOfCentroids.size();
    int numOfPoints = vecOfPoints.size();

    for (int i = 0; i < numOfPoints; i++)
    {
        point *pt = &vecOfPoints[i];
        int idPoint = pt->pVector[0];
        int flag = 0;
        for (int j = 0; j < numOfCentroids; j++)
        {
            if (vecOfCentroids[j].mapOfAssignedItems.find(idPoint) != vecOfCentroids[j].mapOfAssignedItems.end())
            {
                flag = 1;
            }
        }
        if (flag)
            continue;

        float min = MAXFLOAT;
        int cent;
        for (int j = 0; j < numOfCentroids; j++)
        {
            float dist = euclDistance(pt, vecOfCentroids[j].coordinates);
            if (dist < min)
            {
                min = dist;
                cent = j;
            }
        }
        vecOfCentroids[cent].clusterPoints.push_back(pt);
        vecOfCentroids[cent].mapOfAssignedItems.insert(pair<int, int>(idPoint, cent));
    }
}

void printToFile(ofstream &outputFileStream, vector<centroid> &vecOfCentroids, chrono::duration<double> sec, char *silh)
{
    //     CLUSTER-1 {size: <int>, centroid: πίνακας με τις συντεταγμένες του centroid}
    // . . . . . . .
    // CLUSTER-Κ {size: <int>, centroid: πίνακας με τις συντεταγμένες του centroid}
    // clustering_time: <double> //in seconds
    int numOfClusters = vecOfCentroids.size();

    for (int i = 0; i < numOfClusters; i++)
    {
        int sizeOfCluster = vecOfCentroids[i].clusterPoints.size();
        int sizeOfCoordinatesVector = vecOfCentroids[i].coordinates->pVector.size();
        outputFileStream << " CLUSTER-" << i + 1 << "{size:" << sizeOfCluster << ", centroid: ";
        for (int j = 1; j < sizeOfCoordinatesVector; j++)
        {
            // first item is 0 (key) for centroids...
            outputFileStream << vecOfCentroids[i].coordinates->pVector[j] << " ";
        }
        outputFileStream << endl;
    }
    outputFileStream << "Clustering_time: " << sec.count() << " s" << endl
                     << endl;

    if (strcmp(silh, "yes") == 0)
    {
        silhouette(vecOfCentroids, outputFileStream);
    }
    for (int i = 0; i < numOfClusters; i++)
    {
        int sizeOfCluster = vecOfCentroids[i].clusterPoints.size();
        outputFileStream << " CLUSTER-" << i + 1 << ", centroid: ";
        for (int j = 0; j < sizeOfCluster; j++)
        {
            outputFileStream << "item_id_" << j + 1 << ": " << vecOfCentroids[i].clusterPoints[j]->pVector[0] << ", ";
        }
        outputFileStream << endl;
    }
}

void hyperCubeReverse(vector<centroid> &vecOfCentroids, vector<point> &vecOfPoints, hashTableCube *hashT, int probes, int M)
{
    int sizeOfVecOfCentroids = vecOfCentroids.size();
    int sizeOfVecOfPoints = vecOfPoints.size();

    vector<kNearest> list(sizeOfVecOfCentroids);
    vector<map<int, double>> vecOfMaps(sizeOfVecOfCentroids);
    float min = MAXFLOAT;
    for (int i = 0; i < sizeOfVecOfCentroids; i++)
    {
        for (int j = 0; j < sizeOfVecOfCentroids; j++)
        {
            if (i == j)
                continue;
            float dist = euclDistance(vecOfCentroids[i].coordinates, vecOfCentroids[j].coordinates);
            if (dist < min)
            {
                min = dist;
            }
        }
    }

    for (int i = 0; i < sizeOfVecOfCentroids; i++)
    {
        int R = min / 2;
        initKNearest(0, &list[i]);
        int prevSize = -1;
        int curSize = list[i].nearestPoints.size();
        int counter = 0;

        while (R < INT_MAX && counter < 10 && R > 0)
        {
            point *centroidPoint = vecOfCentroids[i].coordinates;
            hashT->findNeighborsR(centroidPoint, &list[i], R, probes, M);
            prevSize = curSize;
            curSize = list[i].nearestPoints.size();
            counter++;
            R *= 2;
        }
        for (int k = 0; k < list[i].nearestPoints.size(); k++)
        {
            point *pointInserted = list[i].nearestPoints[k];
            int idOfPointInserted = pointInserted->pVector[0];
            double distOfPointInserted = list[i].dist[k];
            vecOfMaps[i].insert(pair<int, double>(idOfPointInserted, distOfPointInserted));
        }
    }

    for (int i = 0; i < sizeOfVecOfCentroids; i++)
    {
        int sizeOfNearestPointsList = list[i].nearestPoints.size();
        for (int j = 0; j < sizeOfNearestPointsList; j++)
        {
            point *pointToFind = list[i].nearestPoints[j];
            int idOfPointToFind = pointToFind->pVector[0];
            double distOfPointToFind = list[i].dist[j];

            vector<int> centroidsFound;
            centroidsFound.push_back(i);

            for (int z = 0; z < sizeOfVecOfCentroids; z++)
            {
                if (z == i)
                    continue;

                auto itOfOtherCentroid = vecOfMaps[z].find(idOfPointToFind);
                if (itOfOtherCentroid != vecOfMaps[z].end())
                {
                    // it was found...
                    centroidsFound.push_back(z);
                }
            }

            // now lets check which of the k centroids are closer to our point...
            double min = MAXFLOAT;
            int minValueCentroid;

            for (int w = 0; w < centroidsFound.size(); w++)
            {
                auto it = vecOfMaps[centroidsFound[w]].find(idOfPointToFind);
                double dist = it->second;
                if (dist < min)
                {
                    min = dist;
                    minValueCentroid = centroidsFound[w];
                }
            }

            // time to erase...
            for (int w = 0; w < centroidsFound.size(); w++)
            {
                auto it = vecOfMaps[centroidsFound[w]].find(idOfPointToFind);
                if (centroidsFound[w] != minValueCentroid)
                {
                    int sizeOfNearestPointsVec = list[w].nearestPoints.size();
                    for (int ii = 0; ii < sizeOfNearestPointsVec; ii++)
                    {
                        point *curPoint = list[w].nearestPoints[ii];
                        auto itToEraseNearestPoint = ii + list[w].nearestPoints.begin();
                        auto itToEraseDist = ii + list[w].dist.begin();
                        if (curPoint->pVector[0] == idOfPointToFind)
                        {
                            //erase
                            list[w].nearestPoints.erase(itToEraseNearestPoint);
                            list[w].dist.erase(itToEraseDist);
                            break;
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < sizeOfVecOfCentroids; i++)
    {
        vecOfCentroids[i].clusterPoints = list[i].nearestPoints;
        vecOfCentroids[i].initMap(i);
    }
}

void lloyd(vector<centroid> &vecOfCentroids, vector<point> &vecOfPoints, int frechet)
{
    int sizeOfVecOfCentroids = vecOfCentroids.size();
    int sizeOfVecOfPoints = vecOfPoints.size();
    for (int i = 0; i < sizeOfVecOfCentroids; i++)
    {
        if (vecOfCentroids[i].clusterPoints.empty() == 0)
        {
            vecOfCentroids[i].clusterPoints.clear();
        }
    }

    for (int i = 0; i < sizeOfVecOfPoints; i++)
    {
        float min = MAXFLOAT;
        int centr;
        point *currPoint = &vecOfPoints[i];
        for (int j = 0; j < sizeOfVecOfCentroids; j++)
        {
            centroid *currCentroid = &vecOfCentroids[j];
            point *currCentroidCoo = currCentroid->coordinates;

            float dist;
            if (frechet == 0)
                dist = euclDistance(currCentroidCoo, currPoint);

            if (frechet == 1)
                dist = DFD(currCentroidCoo->pVector, currPoint->pVector);

            if (dist < min && dist > 0)
            {
                min = dist;
                centr = j;
            }
        }
        vecOfCentroids[centr].clusterPoints.push_back(currPoint);
    }
}

void silhouette(vector<centroid> &vecOfCentroids, ofstream &outputFiletStream)
{
    int numOfClusters = vecOfCentroids.size();
    outputFiletStream << "Silhouette: [";
    for (int i = 0; i < numOfClusters; i++)
    {
        int sizeOfCluster = vecOfCentroids[i].clusterPoints.size();
        centroid *curCentroid = &vecOfCentroids[i];
        vector<float> aiDistances;
        float a_i;

        for (int j = 0; j < sizeOfCluster; j++)
        {
            a_i = 0.0;
            point *pt1 = curCentroid->clusterPoints[j];
            for (int k = 0; k < sizeOfCluster; k++)
            {
                point *pt2 = curCentroid->clusterPoints[k];
                a_i = a_i + euclDistance(curCentroid->clusterPoints[j], curCentroid->coordinates);
            }
            aiDistances.push_back(a_i / sizeOfCluster);
        }
        //find next closest centroid

        float min = MAXFLOAT;
        int centr;

        for (int j = 0; j < numOfClusters; j++)
        {
            if (j == i)
                continue;
            centroid *tempCentroid = &vecOfCentroids[j];
            float dist = euclDistance(curCentroid->coordinates, tempCentroid->coordinates);
            if (dist < min)
            {
                min = dist;
                centr = j;
            }
        }

        centroid *secBestCentroid = &vecOfCentroids[centr];
        int sizeOfSecBestCluster = secBestCentroid->clusterPoints.size();
        vector<float> biDistances;
        for (int j = 0; j < sizeOfCluster; j++)
        {
            float b_i = 0.0;
            for (int k = 0; k < sizeOfSecBestCluster; k++)
            {
                b_i = b_i + euclDistance(secBestCentroid->clusterPoints[k], curCentroid->clusterPoints[j]);
            }
            biDistances.push_back(b_i / sizeOfSecBestCluster);
        }

        float sp = 0.0;
        for (int j = 0; j < sizeOfCluster; j++)
        {
            sp = sp + (biDistances[j] - aiDistances[j]) / (max(biDistances[j], aiDistances[j]));
        }
        outputFiletStream << sp / sizeOfCluster << ", ";
    }

    outputFiletStream << "]\n";
}