#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* findAndReplace(char* str, char* fStr, char* rStr) {
    char* itr = str;
    while (*itr) {
        char* look = itr;
        char* tmpFStr = fStr;
        int found = 0;
        while (*look == *tmpFStr) {
            *look++; *tmpFStr++;
        }
        if (*look == *tmpFStr) {
            found = 1;
        }
        if (found) {
            char* replPos = itr;
            char* tmpRStr = rStr;
            while (*tmpRStr) {
                *replPos = *tmpRStr++;
            }
        }
        itr++;
    }
    return str;
}

int main() {
    char* inFileName  = "inputFile";
    char* outFileName = "outputFile";
    
    FILE *inFile, *outFile;
    inFile  = fopen(inFileName, "r");
    outFile = fopen(outFileName, "w");
    
    size_t len   = 0;
    size_t read  = 0;
    char* line = NULL;
    while (read = getline(&line, &len, inFile) != -1) {
        printf("%s\n", findAndReplace(line, "LEEDLE", " "));
    }
    free(line);
}
