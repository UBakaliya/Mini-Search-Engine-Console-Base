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
    cout << GREEN_COLOR << "*** Welcome to \"Mini Search Engine\" ***\n"
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
        cout << "\n*** Invalid file '" << fileIn << "'. Default file has be choosen: \"" << defFile
             << "\" ***\n"
             << endl;
        fileIn = defFile;
    }
    isValid.close();
    // running the program by given the file name
    char menu = '\0';
    cout << "Menu:\n"
         << "  1. Search Queries\n"
         << "  2. Rate URLs\n"
         << "Your Choices(i.e 1..2)--> ";
    cin >> menu;
    menu = toupper(menu);
    cout << endl;
    cin.ignore();
    switch (menu)
    {
    case '1':
        searchEngine(fileIn);
        break;
    case '2':
        rateUrls(fileIn);
        break;
    default:
        cout << RED_COLOR << "*** Invalid Entry ***" << endl;
        exit(0);
        break;
    }
    return 0;
}
