#include "hashtablehyper.h"
hashTableCube::hashTableCube(int size, lshConstants *lshCon, int numOfDimensions)
{
    this->array.resize(size);
    this->size = size;
    this->lshCon = lshCon;
    this->rVector.resize(lshCon->k);
    this->zeroAndOneVect.resize(size);

    for (int i = 0; i < this->size; i++)
    {
        list<linkedListNode *> l;
        this->array[i] = l;
    }
    for (int j = 0; j < lshCon->k; j++)
    {
        auto itPos = this->rVector.begin() + j;
        this->rVector.insert(itPos, (std::rand() % 100));
    }

    for (int i = 0; i < this->size; i++)
    {
        decToBin(&this->zeroAndOneVect[i], this->lshCon->k, i);
    }

    // produce...
    std::random_device rd;
    std::mt19937 gen(rd());

    // uniform distribution
    std::random_device rd1;
    std::mt19937 gen1(rd1());

    this->vAndTVector.resize(lshCon->k);
    int sample;

    for (int j = 0; j < lshCon->k; j++)
    {
        std::uniform_real_distribution<float> uniDis(0, lshCon->w - 1);
        auto itPos1 = this->vAndTVector.begin() + j;
        vAndT vAndTNode;
        vAndTNode.t = uniDis(gen1);
        vAndTNode.vVector.resize(numOfDimensions);
        for (int z = 0; z < numOfDimensions; z++)
        {
            std::normal_distribution<float> normDis(0, 1);
            sample = (int)round(normDis(gen));
            auto itPos2 = vAndTNode.vVector.begin() + z;
            vAndTNode.vVector.insert(itPos2, sample);
        }
        this->vAndTVector.insert(itPos1, vAndTNode);
    }
}

int hashTableCube::insert(point *pGiven)
{
    int IDp = hashFunction(pGiven);
    int key = IDp;
    //int key = euclideanRemainder(IDp, this->size);
    // if(pGiven->pVector[0] == 1)
    //     cout << IDp << endl;
    list<linkedListNode *> listPtr;
    linkedListNode *nodePtr = new linkedListNode(pGiven, pGiven->pVector[0], IDp);
    listPtr.push_back(nodePtr);
    auto itPos = array.begin() + key;
    this->array[key].push_back(nodePtr);
    // if(this->array[key].empty()){
    //     // empty list == first item...
    //     this->array.insert(itPos, listPtr);
    // }else{
    //     // list is not empty
    //     this->array[key].push_back(nodePtr);
    // }
}

int hashTableCube::search(point *pGiven) {}

// int hashTableCube::hashFunction(point *pGiven)
// {

//     // g(p) = ID(p) mod Table_Size
//     // ID(p) = (Sum |for i = 1 to k| ( h_i*r_i )) mod M, (M = 2^32 - 5)
//     // h_i = lower_bound((v.p + t)/w)
//     int M = myPow(2, 31);
//     M -= 5;
//     int res;
//     int vp;
//     float vpplust;
//     int toLowerBound;
//     int idP;
//     int gP;
//     vector<int> hi(lshCon->k);

//     for (int i = 0; i < lshCon->k; i++)
//     {
//         vAndT *tempVAndT = &(this->vAndTVector[i]);
//         vp = std::inner_product(pGiven->pVector.begin() + 1, pGiven->pVector.end(), tempVAndT->vVector.begin(), 0);
//         vpplust = vp + tempVAndT->t;
//         toLowerBound = vpplust / lshCon->w;
//         auto itPos = hi.begin() + i;
//         hi.insert(itPos, toLowerBound);
//     }

//     idP = std::inner_product(hi.begin(), hi.end(), rVector.begin(), 0);
//     idP = euclideanRemainder(idP, M);
//     //gP = euclideanRemainder(idP, this->size);

//     return idP;
// }

// hyper cube code...
int hashTableCube::hashFunction(point *pGiven)
{
    int M = myPow(2, 31);
    M -= 5;
    int res;
    int vp;
    float vpplust;
    int toLowerBound;
    int idP;
    int gP;
    int fValue;
    int hi;
    vector<int> vec;
    for (int i = 0; i < lshCon->k; i++)
    {
        vAndT *tempVAndT = &(this->vAndTVector[i]);
        vp = std::inner_product(pGiven->pVector.begin() + 1, pGiven->pVector.end(), tempVAndT->vVector.begin(), 0);
        vpplust = vp + tempVAndT->t;
        toLowerBound = vpplust / lshCon->w;
        hi = toLowerBound;
        auto it = this->hToF.find(hi);
        if (it == this->hToF.end())
        {
            // not found
            fValue = zeroOrOne(hi);
            this->hToF.emplace(hi, fValue);
            vec.push_back(fValue);
        }
        else
        {
            //found
            vec.push_back(it->second);
        }
    }
    res = binToDec(vec);
    if (this->zeroAndOneVect[res].empty())
    {
        auto itPos = res + this->zeroAndOneVect.begin();
        this->zeroAndOneVect.insert(itPos, vec);
    }
    return res;
}

hashTableCube::~hashTableCube()
{
}

void hashTableCube::print()
{
    for (int i = 0; i < this->size; i++)
    {
        if (!this->array[i].empty())
        {
            //cout << "Bucket: " << i << endl;
            for (std::list<linkedListNode *>::iterator it = (this->array[i]).begin(); it != (this->array[i]).end(); it++)
            {
                point *ptr = (*it)->pVector;
                if (ptr->pVector[0] == 1)
                {
                    cout << (*it)->getIDp() << "->";
                    cout << endl;
                }
            }
        }
    }
}

vector<list<linkedListNode *>> hashTableCube::getArray()
{
    return this->array;
}

void hashTableCube::findKNeighbors(point *queryPoint, kNearest *nearestList, int probes, int M)
{
    int key = hashFunction(queryPoint);
    //int key = euclideanRemainder(IDp, this->size);
    vector<int> tempVector = this->zeroAndOneVect[key];
    vector<int> temp = tempVector;
    vector<vector<int>> hamDistVecs;
    list<linkedListNode *>::iterator it;

    int probesCounted = 0;
    int mCounted = 0; //points counted
    int sizeOfZeroAndOneVect = this->zeroAndOneVect[key].size();
    int counter = hamDistVecs.size();
    int indexVector = 0;
    int reachedM = 0;

    using clock = std::chrono::system_clock;
    auto begin = clock::now();

    while (probesCounted <= probes && mCounted < M)
    {
        for (it = this->array[key].begin(); it != this->array[key].end(); ++it)
        {
            if (mCounted > M)
            {
                reachedM = 1;
            }
            mCounted++;
            point *curPoint = (*it)->getPVector();
            double dist = calculateDistance(queryPoint, curPoint);
            if (dist < nearestList->dist[nearestList->size - 1] && dist > 0)
            {
                using sec = std::chrono::duration<double, std::micro>;
                sec end = clock::now() - begin;

                nearestList->vecOfTimes[nearestList->size - 1] = end;
                nearestList->dist[nearestList->size - 1] = dist;
                nearestList->nearestPoints[nearestList->size - 1] = curPoint;
                sortNearest(nearestList);
            }
        }
        if (reachedM == 1)
            break;

        if (mCounted < M)
        {
            // we now need to change probe...
            if (counter == 0)
            {
                //the first time in this new probe...
                if (probesCounted > 0)
                {
                    hamDistVecs.clear();
                }

                probesCounted++;
                findVectorsOfHamDist(this->zeroAndOneVect[key], probesCounted, &hamDistVecs, sizeOfZeroAndOneVect - 1);
                counter = hamDistVecs.size();
                key = binToDec(hamDistVecs[counter - 1]);
                counter--;
            }
            else if (counter > 0)
            {
                // give him another vector...
                key = binToDec(hamDistVecs[counter - 1]);
                counter--;
            }
        }
    }
}

void hashTableCube::findKNeighborsTrue(point *queryPoint, kNearest *nearestList, int probes, int M)
{

    int probesCounted = 0;
    int mCounted = 0; //points counted
    int reachedM = 0;

    //int key = euclideanRemainder(IDp, this->size);
    list<linkedListNode *>::iterator it;
    using clock = std::chrono::system_clock;
    auto begin = clock::now();

    for (int i = 0; i < this->size; i++)
    {
        for (it = this->array[i].begin(); it != this->array[i].end(); ++it)
        {
            point *curPoint = (*it)->getPVector();
            double dist = calculateDistance(queryPoint, curPoint);
            if (dist < nearestList->dist[nearestList->size - 1] && dist  > 0)
            {
                using sec = std::chrono::duration<double, std::micro>;
                sec end = clock::now() - begin;
                nearestList->dist[nearestList->size - 1] = dist;
                nearestList->nearestPoints[nearestList->size - 1] = curPoint;
                nearestList->vecOfTimes[nearestList->size - 1] = end;
                sortNearest(nearestList);
            }
        }
    }
}

void hashTableCube::findNeighborsR(point *queryPoint, kNearest *nearestList, int R, int probes, int M)
{
    int key = hashFunction(queryPoint);
    //int key = euclideanRemainder(IDp, this->size);
    vector<int> tempVector = this->zeroAndOneVect[key];
    vector<int> temp = tempVector;
    vector<vector<int>> hamDistVecs;
    list<linkedListNode *>::iterator it;

    int probesCounted = 0;
    int mCounted = 0; //points counted
    int sizeOfZeroAndOneVect = this->zeroAndOneVect[key].size();
    int counter = hamDistVecs.size();
    int indexVector = 0;
    int reachedM = 0;

    while (probesCounted <= probes && mCounted < M)
    {
        for (it = this->array[key].begin(); it != this->array[key].end(); ++it)
        {
            if (mCounted > M)
            {
                reachedM = 1;
                break;
            }
            mCounted++;
            point *curPoint = (*it)->getPVector();
            double dist = calculateDistance(queryPoint, curPoint);
            if (dist < R && dist > 0)
            {
                nearestList->dist.push_back(dist);
                nearestList->size++;
                nearestList->nearestPoints.push_back(curPoint);
            }
        }
        if (reachedM == 1)
            break;

        if (mCounted < M)
        {
            // we now need to change probe...
            if (counter == 0)
            {
                //the first time in this new probe...
                if (probesCounted > 0)
                {
                    hamDistVecs.clear();
                }

                probesCounted++;
                findVectorsOfHamDist(this->zeroAndOneVect[key], probesCounted, &hamDistVecs, sizeOfZeroAndOneVect - 1);
                counter = hamDistVecs.size();
                key = binToDec(hamDistVecs[counter - 1]);
                counter--;
            }
            else if (counter > 0)
            {
                // give him another vector...
                key = binToDec(hamDistVecs[counter - 1]);
                counter--;
            }
        }
    }
}

int hashTableCube::getSize()
{
    return this->size;
}