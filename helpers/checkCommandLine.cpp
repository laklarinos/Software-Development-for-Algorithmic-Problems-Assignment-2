#include "checkCommandLine.h"
bool checkCommandLineArguments(int argcInt, char *argvArray[])
{
    if (argcInt != 21)
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
        if (strcmp(argvArray[3], "-q") == 0)
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
        if (strcmp(argvArray[5], "-k") == 0)
        {
            // we want integer
            int counter = 0;
            int flag = 0;
            while (argvArray[6][counter] != '\0')
            {
                if (!isdigit(argvArray[6][counter]))
                {
                    flag = 1;
                    break;
                }
                counter++;
            }
            if (flag)
            {
                // not an int...
                cout << "Argument 6 needs to be an int\n";
                return 1;
            }
            // else int
            //k = atoi(argvArray[6]);
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        if (strcmp(argvArray[7], "-L") == 0)
        {
            // we want integer
            int counter = 0;
            int flag = 0;
            while (argvArray[8][counter] != '\0')
            {
                if (!isdigit(argvArray[8][counter]))
                {
                    flag = 1;
                    break;
                }
                counter++;
            }
            if (flag)
            {
                // not an int...
                cout << "Argument 8 needs to be an int\n";
                return 1;
            }
            // else int
            //L = atoi(argvArray[8]);
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        if (strcmp(argvArray[9], "-M") == 0)
        {
            // we want integer
            int counter = 0;
            int flag = 0;
            while (argvArray[10][counter] != '\0')
            {
                if (!isdigit(argvArray[10][counter]))
                {
                    flag = 1;
                    break;
                }
                counter++;
            }
            if (flag)
            {
                // not an int...
                cout << "Argument 10 needs to be an int\n";
                return 1;
            }
            // else int
            //L = atoi(argvArray[8]);
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        if (strcmp(argvArray[11], "-probes") == 0)
        {
            // we want integer
            int counter = 0;
            int flag = 0;
            while (argvArray[12][counter] != '\0')
            {
                if (!isdigit(argvArray[12][counter]))
                {
                    flag = 1;
                    break;
                }
                counter++;
            }
            if (flag)
            {
                // not an int...
                cout << "Argument 12 needs to be an int\n";
                return 1;
            }
            // else int
            //L = atoi(argvArray[8]);
        }

        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        // -o outputfile

        if (strcmp(argvArray[13], "-o") == 0)
        {
            if (access(argvArray[14], F_OK) == 0)
            {
                // file exists
                printf("File exists\n");
            }
            else
            {
                // file doesn't exist
                cout << "File " << argvArray[14] << " does not exist\n";
                return 1;
            }
        }
        else
        {
            cout << "Incorrect arguments\n";
            return 1;
        }

        //outputFile = argvArray[10];
        if (strcmp(argvArray[15], "-algorithm") == 0)
        {
            if (strcmp(argvArray[16], "LSH") != 0 && strcmp(argvArray[16], "Hypercube") != 0 && strcmp(argvArray[16], "Frechet") != 0)
            {
                cout << "Argument 12 needs to be 1 of the 3 algorithms (LSH, Hypercube, Frechet), written in this way.\n";
                return 1;
            }
        }
        else
        {
            cout << "Argument 11 needs to be named \"algorithm\".\n";
            return 1;
        }

        if (strcmp(argvArray[17], "metric") == 0)
        {
            if (strcmp(argvArray[16], "Frechet") == 0)
            {
                if (strcmp(argvArray[18], "discrete") != 0 && strcmp(argvArray[18], "continuous") != 0)
                {
                    cout << "If Frechet is the algorithm, metric should be \"discrete\" or \"continuous\".\n";
                    return 1;
                }
            }
            else
            {
                cout << "Because Frechet algorithm was not selected, this argument will not be taken in to consideration.\n";
            }
        }

        if (strcmp(argvArray[19], "-delta") == 0)
        {
            //check if valid double...
            int len;
            double ignore = 0.0;
            char c;

            int ret = sscanf(argvArray[20], "%f %n", &ignore, &len);
            int res = (ret == 1 && len == (int)strlen(argvArray[20]));

            if (res != 1)
            {
                cout << "Argument 20 should be a valid double.\n";
                return 1;
            }
        }
    }
    else
    {
        cout << "Incorrect arguments\n";
        return 1;
    }
    return 0; //correct command line arguments
}