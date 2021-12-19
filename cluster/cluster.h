#ifndef CLUSTER
#define CLUSTER

#include "../helpers/helpers.h"
#include "clusterhashtable.h"
#include "../helpers/includes.h"
#include "../importantStructs/curveMine.h"

class centroid
{
public:
    point *coordinates;
    //curveMine *coordinatesCurve;
    vector<point *> clusterPoints;
    //vector<curveMine *> clusterCurve;
    centroid(point *);
    void includePoint(point *);
    map<int, int> mapOfAssignedItems;
    void initMap(int clusterId);
};

class centroidCurves
{
public:
    curveMine *coordinates;
    vector<curveMine *> clusterPoints;
    centroidCurves(curveMine *);
    void includePoint(curveMine *);
    map<int, int> mapOfAssignedItems;
    void initMap(string clusterId);
};

void initCentroids(vector<centroid> &vecOfCentroids, int numOfClusters, vector<point> &vectorOfPoints, int frechet = 0);
//void initCentroidsFrechet(vector<centroid> &vecOfCentroids, int numOfClusters, vector<curveMine> &vectorOfPoints);
void lshReverse(vector<centroid> &vecOfCentroids, vector<point> &vecOfPoints, vector<hashTable *> &vecOfHashTables);
void hyperCubeReverse(vector<centroid> &vecOfCentroids, vector<point> &vecOfPoints, hashTableCube *hashT, int probes, int M);
void updateCentroids(vector<centroid> &vecOfCentroids, int numOfClusters);
void updateCentroidsCurve(vector<centroid> &vecOfCentroids, int numOfClusters);
void lloyd(vector<centroid> &vecOfCentroids, vector<point> &vecOfPoints, int frechet = 0);
void insertRestOfPoints(vector<point> &vecOfPoints, vector<centroid> &vecOfCentroids);
void silhouette(vector<centroid> &vecOfCentroids, ofstream &outputFiletStream);
void printToFile(ofstream &outputFileStream, vector<centroid> &vecOfCentroids, chrono::duration<double> sec, char *silh);

#endif