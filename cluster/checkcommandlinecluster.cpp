#include "checkcommandlinecluster.h"


// $./cluster –i <input file> –c <configuration file> -o <output file> -update <Mean
// Frechet or Mean Vector> –assignment <Classic or LSH or Hypercube or LSH_Frechet>
// -complete <optional> -silhouette <optional>

bool checkCommandLineCluster(int argcInt, char *argvArray[])
{

    int k;
    int L;
    int numberOfNN;
    int rad;
    int numOfLines = 0;
    int numOfElements = 0;
    char *pch;
    ssize_t read;
    char *line;
    ssize_t len = 0;
    int lineCounter = 0;
    int elementCounter = 0;
    char *inputFile;
    char *queryFile;
    char *outputFile; // na to doume

    if (argcInt != 13)
    {
        cout << "not enough arguments\n";
        return 1;
    }

    //–i <input file>
    if (strcmp(argvArray[1], "-i") == 0)
    {
        // the next argument should be the dataset path
        if (access(argvArray[2], F_OK) == 0)
        {
            // file exists
            cout << "file exists\n";
        }
        else
        {
            // file doesn't exist
            cout << "File: " << argvArray[2] << "does not exist\n";
            return 1;
        }
        //–q <query file>
        //inputFile = argvArray[2];
        if (strcmp(argvArray[3], "-c") == 0)
        {
            if (access(argvArray[4], F_OK) == 0)
            {
                // file exists
                cout << "File Exists\n";
            }
            else
            {
                // file doesn't exist
                cout << "File: " << argvArray[4] << " does not exist\n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        //queryFile = argvArray[4];
        //–k <int>
        if (strcmp(argvArray[5], "-o") == 0)
        {
            if (access(argvArray[6], F_OK) == 0)
            {
                // file exists
                cout << "File Exists\n";
            }
            else
            {
                // file doesn't exist
                cout << "File: " << argvArray[6] << " does not exist\n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        if (strcmp(argvArray[7], "-update") == 0)
        {
            if (strcmp(argvArray[8], "Mean_Frechet") != 0 && strcmp(argvArray[8], "Mean_Vector") != 0)
            {
                cout << "-update argument must be followed by a \"Mean Frechet\" or \"Mean Vector\" \n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        // -o outputfile

        if (strcmp(argvArray[9], "-assignment") == 0)
        {
            if (strcmp(argvArray[10], "Classic") != 0 && 
                strcmp(argvArray[10], "LSH") != 0 && 
                strcmp(argvArray[10], "Hypercube")!=0 &&
                strcmp(argvArray[10], "LSH_Frechet")!=0)
            {
                cout << "-assignment Argument must be followed by a \"Classic\", \"LSH\", \"Hypercube\" or \"LSH_Frechet\".\n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        if (strcmp(argvArray[11], "-silhouette") == 0)
        {
            if (strcmp(argvArray[12], "yes") != 0 &&
                strcmp(argvArray[12], "no") != 0)
            {
                cout << "-silhouette Argument must be followed by a \"yes\" or \"no\".\n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }
    }
    return 0; //correct command line arguments
}