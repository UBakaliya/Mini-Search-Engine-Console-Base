/**
 * @file application.cpp
 * @author Uvaish Bakaliya
 * @brief This is the function where we run our program!
 * @note This is the first version of the 'Mini-Search-Engine'
 * @version 1.0
 * @date 2022-12-12
 * @copyright Copyright (c)UB 2022
 */
#include "headerFiles/search.h"

int main()
{
    system("clear");
    cout << "*** Welcome to \"Mini Search Engine\" ***\n"
         << endl;
    string fileIn,
        defFile = "dataFiles/cplusplus.txt";
    cout << "Enter the file name you would like to search queries in: ";
    cin >> fileIn;
    // check file is valid to open if not then make the file name ad
    // default file name
    ifstream isValid(fileIn);
    if (isValid.fail())
    {
        cout << "Invalid file '" << fileIn << "'. Default file has be choosen: " << defFile << endl;
        fileIn = defFile;
    }
    isValid.close();
    cin.ignore();
    // running the program by given the file name
    searchEngine(fileIn);
    return 0;
}
