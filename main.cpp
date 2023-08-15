#include "helpers/helpers.h"
#include "lsh/lsh.h"
#include "hypercube/cube.h"

int main(int argc, char **argv)
{
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
        lshConstants lshCon;
        lshCon.k = k;
        lshCon.L = L;
        lshCon.w = 3;
        lsh(inputFile, queryFile, outputFile, lshCon, (char *)"no");
    }
    else if (strcmp(algorithm, "Frechet") == 0)
    {

        lshConstants lshCon;
        lshCon.k = k;
        lshCon.L = L;
        lshCon.w = 3;
        lsh(inputFile, queryFile, outputFile, lshCon, (char *)"yes", metric, delta);
    }
    else if (strcmp(algorithm, "Hypercube") == 0)
    {
        cubeConstants cubeCon;
        cubeCon.k = k;
        cubeCon.L = L;
        cubeCon.w = 3;
        cubeCon.probes = probes;
        cubeCon.M = M;
        cube(inputFile, queryFile, outputFile, cubeCon);
    }
}