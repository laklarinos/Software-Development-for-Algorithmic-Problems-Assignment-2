#include "lshhashtable.h"
hashTable::hashTable(int size, lshConstants *lshCon, int numOfDimensions, int frechet, int continuous)
{
    this->array.resize(size);
    this->size = size;
    this->lshCon = lshCon;
    this->rVector.resize(lshCon->k);

    for (int j = 0; j < lshCon->k; j++)
    {
        auto itPos = this->rVector.begin() + j;
        this->rVector.insert(itPos, (std::rand() % 100));
    }

    // produce...
    std::random_device rd;
    std::mt19937 gen(rd());

    // uniform distribution
    std::random_device rd1;
    std::mt19937 gen1(rd1());

    this->vAndTVector.resize(lshCon->k);
    int sample;

    if (frechet == 1 && continuous == 0)
        numOfDimensions = numOfDimensions * 2;

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
            vAndTNode.vVector[z] = sample;
        }
        this->vAndTVector.insert(itPos1, vAndTNode);
    }
}

int hashTable::insert(point *pGiven)
{
    int IDp = hashFunction(pGiven);
    int key = euclideanRemainder(IDp, this->size);
    list<linkedListNode *> listPtr;
    linkedListNode *nodePtr = new linkedListNode(pGiven, IDp);
    listPtr.push_back(nodePtr);
    auto itPos = array.begin() + key;
    this->array[key].push_back(nodePtr);
}

int hashTable::insert(point *pGiven, vector<float> vecToFindKey)
{
    point p(vecToFindKey);
    int IDp = hashFunction(&p);
    int key = euclideanRemainder(IDp, this->size);
    list<linkedListNode *> listPtr;
    linkedListNode *nodePtr = new linkedListNode(pGiven, IDp);
    listPtr.push_back(nodePtr);
    auto itPos = array.begin() + key;
    this->array[key].push_back(nodePtr);
    return key;
}

int hashTable::search(point *pGiven) {}

int hashTable::hashFunction(point *pGiven)
{

    // g(p) = ID(p) mod Table_Size
    // ID(p) = (Sum |for i = 1 to k| ( h_i*r_i )) mod M, (M = 2^32 - 5)
    // h_i = lower_bound((v.p + t)/w)
    int M = myPow(2, 31);
    M -= 5;
    int res;
    int vp;
    float vpplust;
    int toLowerBound;
    int idP;
    int gP;
    vector<int> hi(lshCon->k);

    for (int i = 0; i < lshCon->k; i++)
    {
        vAndT *tempVAndT = &(this->vAndTVector[i]);
        vp = std::inner_product(pGiven->pVector.begin() + 1, pGiven->pVector.end(), tempVAndT->vVector.begin(), 0);
        vpplust = vp + tempVAndT->t;
        toLowerBound = vpplust / lshCon->w;
        auto itPos = hi.begin() + i;
        hi.insert(itPos, toLowerBound);
    }

    idP = std::inner_product(hi.begin(), hi.end(), rVector.begin(), 0);
    idP = euclideanRemainder(idP, M);
    //gP = euclideanRemainder(idP, this->size);
    return idP;
}

hashTable::~hashTable()
{
    for (int i = 0; i < this->size; i++)
    {
        for (std::list<linkedListNode *>::iterator it = (this->array[i]).begin(); it != (this->array[i]).end(); it++)
        {
            delete (*it);
        }
    }
}

void hashTable::print()
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
            //cout << "\n";
        }
    }
}

vector<list<linkedListNode *>> hashTable::getArray()
{
    return this->array;
}

void hashTable::findKNeighbors(point *queryPoint, kNearest *nearestList, vector<float> vecFloat, int continuous)
{
    int IDp;
    int key;
    int frechetDisc = 0;
    int frechetCont = 0;

    if (vecFloat.empty() != 1 && continuous == 1)
    {
        frechetCont = 1;
        point p(vecFloat);
        IDp = hashFunction(&p);
        key = euclideanRemainder(IDp, this->size);
    }
    else if (vecFloat.empty() != 1 && continuous == 0)
    {
        frechetDisc = 1;
        point p(vecFloat);
        IDp = hashFunction(&p);
        key = euclideanRemainder(IDp, this->size);
    }
    else
    {
        IDp = hashFunction(queryPoint);
        key = euclideanRemainder(IDp, this->size);
    }

    list<linkedListNode *>::iterator it;
    using clock = std::chrono::system_clock;
    auto begin = clock::now();
    int counter = 0;

    for (it = this->array[key].begin(); it != this->array[key].end(); ++it)
    {
        int IDpNode = (*it)->getIDp();

        if (IDp == IDpNode)
        {

            point *curPoint = (*it)->getPVector();
            float dist;
            if (frechetDisc == 1)
            {
                dist = DFD(queryPoint->pVector, curPoint->pVector);
            }
            else if (frechetCont == 1)
            {
                dist = CFD(queryPoint->filteredCurve, curPoint->filteredCurve);
            }
            else if (frechetDisc == 0 && frechetCont == 0)
            {
                dist = euclDistance(curPoint, queryPoint);
            }

            if (dist < nearestList->dist[nearestList->size - 1] && dist > 0)
            {
                counter++;
                using sec = std::chrono::duration<double, std::micro>;
                sec end = clock::now() - begin;
                nearestList->dist[nearestList->size - 1] = dist;
                nearestList->nearestPoints[nearestList->size - 1] = curPoint;
                nearestList->vecOfTimes[nearestList->size - 1] = end;
                sortNearest(nearestList);
            }
        }
    }
    if (counter < nearestList->dist.size())
    {
        // if we havent found K NN's
        // do the procedure again but w/o ID checking...
        auto begin = clock::now();

        for (it = this->array[key].begin(); it != this->array[key].end(); ++it)
        {
            int IDpNode = (*it)->getIDp();
            double dist;
            point *curPoint = (*it)->getPVector();
            if (frechetDisc == 1)
            {
                dist = DFD(queryPoint->pVector, curPoint->pVector);
            }
            else if (frechetCont == 1)
            {
                dist = CFD(queryPoint->filteredCurve, curPoint->filteredCurve);
            }
            else if (frechetDisc == 0 && frechetCont == 0)
            {
                dist = euclDistance(curPoint, queryPoint);
            }
            if (dist < nearestList->dist[nearestList->size - 1] && dist > 0)
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

void hashTable::findKNeighborsTrue(point *queryPoint, kNearest *nearestList, vector<float> vecFloat, int continuous)
{
    int IDp;
    int key;
    int frechetDisc = 0;
    int frechetCont = 0;
    if (vecFloat.empty() != 1 && continuous == 1)
    {
        frechetCont = 1;
        point p(vecFloat);
        IDp = hashFunction(&p);
        key = euclideanRemainder(IDp, this->size);
    }
    else if (vecFloat.empty() != 1 && continuous == 0)
    {
        frechetDisc = 1;
        point p(vecFloat);
        IDp = hashFunction(&p);
        key = euclideanRemainder(IDp, this->size);
    }
    else
    {
        IDp = hashFunction(queryPoint);
        key = euclideanRemainder(IDp, this->size);
    }

    list<linkedListNode *>::iterator it;
    using clock = std::chrono::system_clock;
    auto begin = clock::now();
    for (int i = 0; i < this->size; i++)
    {
        for (it = this->array[i].begin(); it != this->array[i].end(); ++it)
        {
            int IDpNode = (*it)->getIDp();
            point *curPoint = (*it)->getPVector();
            double dist;

            if (frechetDisc == 1)
            {
                dist = DFD(queryPoint->pVector, curPoint->pVector);
            }
            else if (frechetCont == 1)
            {
                dist = CFD(queryPoint->filteredCurve, curPoint->filteredCurve);
            }
            else if (frechetDisc == 0 && frechetCont == 0)
            {
                dist = euclDistance(curPoint, queryPoint);
            }

            if (dist < nearestList->dist[nearestList->size - 1] && dist > 0)
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

void hashTable::findNeighborsR(point *queryPoint, kNearest *nearestList, int R, vector<float> vecFloat, int frechet)
{
    int IDp;
    int key;
    int frechetDisc = 0;

    if (frechet == 1)
    {
        point p(vecFloat);
        IDp = hashFunction(&p);
        key = euclideanRemainder(IDp, this->size);
    }
    else if (frechet == 0)
    {
        IDp = hashFunction(queryPoint);
        key = euclideanRemainder(IDp, this->size);
    }

    list<linkedListNode *>::iterator it;
    for (it = this->array[key].begin(); it != this->array[key].end(); ++it)
    {
        int IDpNode = (*it)->getIDp();
        point *curPoint = (*it)->getPVector();
        int found = 0;
        for (int i = 0; i < nearestList->nearestPoints.size(); i++)
        {
            if (curPoint->getKey() == nearestList->nearestPoints[i]->getKey()){
                found = 1;
            }
        }
        if(found) continue;

        double dist;
        
        if (frechet == 1)
            dist = DFD(queryPoint->pVector, curPoint->pVector);
        if (frechet == 0)
            dist = euclDistance(queryPoint, curPoint);

        if (dist < R && dist > 0)
        {
            nearestList->dist.push_back(dist);
            nearestList->size++;
            nearestList->nearestPoints.push_back(curPoint);
        }
    }
}