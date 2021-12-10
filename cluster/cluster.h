#ifndef CLUSTER
#define CLUSTER
#include "hashtable.h"
#include "hashtablehyper.h"

class centroid
{
public:
    point *coordinates;
    vector<point *> culsterPoints;
    centroid(point *);
    void includePoint(point *);
    map<int, int> mapOfAssignedItems;
    void initMap(int clusterId);
};

void initCentroids(vector<centroid> &vecOfCentroids, int numOfClusters, vector<point> &vectorOfPoints);
void lshReverse(vector<centroid> &vecOfCentroids, vector<point> &vecOfPoints, vector<hashTable *> &vecOfHashTables);
void hyperCubeReverse(vector<centroid> &vecOfCentroids, vector<point> &vecOfPoints, hashTableCube *hashT, int probes, int M);
void updateCentroids(vector<centroid> &vecOfCentroids, int numOfClusters);
void lloyd(vector<centroid> & vecOfCentroids, vector<point> &vecOfPoints);
void insertRestOfPoints(vector<point> &vecOfPoints, vector<centroid> &vecOfCentroids);
void silhouette(vector<centroid> &vecOfCentroids, ofstream &outputFiletStream);
void printToFile(ofstream &outputFileStream, vector<centroid> &vecOfCentroids, chrono::duration<double> sec, char *complete);

#endif