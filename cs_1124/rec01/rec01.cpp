/*
 * Program Name: Recitation 1
 * Name: Christopher Thompson
 * Description:
 * Prints provided file out and 
 * counts number of words in file.
 */

#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream inputFile("rec01.txt");
    string line;

    cout << "*** START OF FILE ***" << endl;

    while (getline(inputFile, line)) {
        cout << line << endl;
    }

    inputFile.clear();
    inputFile.seekg(0);

    cout << "*** END OF FILE ***" << endl;

    int wordCount = 0;
    while (inputFile >> line) {
        wordCount++;
    }

    inputFile.close();

    cout << "\nFile word count: " << wordCount << endl;

    return 0;
}

