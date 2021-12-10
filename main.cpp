#include "helpers/checkCommandLine.h"
#include "helpers/parsinginit.h"
#include "lsh/lsh.h"

int main(int argc, char **argv)
{
    cout << " DFGHJKL:\n";

    int res = checkCommandLineArguments(argc, argv);
    if (res != 0)
        return EXIT_FAILURE;

    char *inputFile = argv[2];
    char *queryFile = argv[4];
    int k = atoi(argv[6]);
    int L = atoi(argv[8]);
    int M = atoi(argv[10]);
    int probes = atoi(argv[12]);
    char *outputFile = argv[14];
    char *algorithm = argv[16];
    char *metric = argv[18];
    double delta = atof(argv[20]);

    int numOfDimensions;
    int numOfElements;
    int numOfLines;

    //return EXIT_SUCCESS;

    if (res == 1)
    {
        cout << "Error when ParInit\n";
        exit(1);
    }

    if (strcmp(algorithm, "LSH") == 0)
    {
#ifndef LSH
#define LSH
#include "lsh/lsh.h"
#endif

        lshConstants lshCon;
        lshCon.k = k;
        lshCon.L = L;
        lshCon.w = 3;
        if(strcmp(algorithm, "Frechet") == 0)
            lsh(inputFile, queryFile, outputFile, lshCon, (char*)"yes", metric);
        else
            lsh(inputFile, queryFile, outputFile, lshCon,(char*)"no");
    }
}