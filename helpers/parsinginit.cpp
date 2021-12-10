#include "parsingInit.h"
using namespace std;

int parsInit(char *inputFile, vector<point> &vecPoints, int *numLinesReturn, int *numOfElements)
{
    fstream inputFilePtr;

    inputFilePtr.open(inputFile);
    if (!inputFilePtr)
    {
        cout << "Error,  file does not exist\n";
        return 1;
    }

    int numOfLines = 0;
    char *pch;
    ssize_t read;
    string line;
    ssize_t len = 0;
    int lineCounter = 0;
    int elementCounter = 0;

    int elementsPerLine = 0;
    string token;

    while (getline(inputFilePtr, line))
    {
        numOfLines++;
        istringstream is(line);
        size_t space_pos = line.find("	");
        string key = line.substr(0, space_pos);
        line.erase(0, space_pos + 1);

        vector<float> vecFloat;
        string values;
        istringstream is2(line);
        vecFloat.push_back(0.0);
        while (getline(is2, values, '\t'))
        {
            vecFloat.push_back(atof(values.c_str()));
        }

        point pointToInsert(vecFloat, key);
        vecPoints.push_back(pointToInsert);
    }

    elementsPerLine = vecPoints[0].pVector.size();
    *numLinesReturn = numOfLines;
    *numOfElements = elementsPerLine;
    return 0;
}
