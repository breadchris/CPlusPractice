#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE (32 * 1024)

int main() {
    char fileName[] = "inputFile";
    FILE * fp;
    fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("[-] Error when opening file: %s\n", fileName);
        exit(8);
    }
    
    int numOfLines = 0;
    char * tmpBuf = NULL;
    size_t len = BUF_SIZE;
    size_t read = 0;
    while (read = getline(&tmpBuf, &len, fp) != -1) {
        printf("%s\n", tmpBuf);
        numOfLines++;
    }
    free(tmpBuf);
    fclose(fp);
    
    printf("The file %s had %d number of lines.\n", fileName, numOfLines);
}