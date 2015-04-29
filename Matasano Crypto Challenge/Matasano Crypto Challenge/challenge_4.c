//
//  challenge_4.c
//  Matasano Crypto Challenge
//
//  Created by Christopher Thompson on 3/22/15.
//  Copyright (c) 2015 Christopher Thompson. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

struct Line {
    int lineNumber;
    int score;
    int key;
    char* text;
    char* decryptedText;
} xorLine;

static char* hexLookup    = "0123456789abcdef";
int LINE_LENGTH = 60;
int NUMBER_OF_LINES = 326;
float letterFreq[] = {0.0816, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987, 0.06326, 0.09055, 0.02758, 0.00978, 0.0236, 0.0015, 0.01974, 0.00074};
int expectedFreq[sizeof(letterFreq) / sizeof(float)];

void setupExpectedFreq() {
    int i;
    for (i = 0; i < sizeof(expectedFreq)/sizeof(int); ++i) {
        expectedFreq[i] = letterFreq[i] * LINE_LENGTH;
    }
}

int asciiToHex(char ascii) {
    char* tmp = hexLookup;
    
    for (int i = 0; tmp != NULL; i++, tmp++) {
        if (*tmp == ascii)
            return i;
    }
    
    return -1;
}

char* pairHex(char* hex, unsigned long size) {
    if (size % 2 == 0) {
        char* paired = (char *) malloc(size / 2);
        for (int i = 0; i < strlen(hex); i += 2) {
            paired[i/2] = (asciiToHex(hex[i]) * 16) + asciiToHex(hex[i+1]);
        }
        return paired;
    }
    
    printf("[-] Given hex is not multiple of two");
    return NULL;
}

char* keyedXOR(char* a, int n) {
    unsigned long size = strlen(a);
    char* out = malloc(size);
    
    for (int i = 0; i < size; i++) {
        out[i] = a[i] ^ n;
    }
    
    return out;
}

int scoreDecryptedText(char* a) {
    int freqDelta = 0;
    int charFreq[sizeof(expectedFreq)/sizeof(int)];
    memset(charFreq, 0, sizeof(charFreq));
    
    // Determine freqency of all characters in line
    for (int i = 0; i < strlen(a); i++) {
        // Test if char is in ascii range
        if (a[i] > 'a' && a[i] < 'z') {
            char fixed = a[i] - 'a';
            charFreq[fixed] += 1;
        } else if(a[i] > 'A' && a[i] < 'Z') {
            char fixed = a[i] - 'A';
            charFreq[fixed] += 1;
        }
    }
    
    // Compute deltas for all freqencies
    for (int i = 0; i < sizeof(expectedFreq)/sizeof(int); i++) {
        freqDelta += (int) abs(expectedFreq[i] - charFreq[i]);
    }
    
    return freqDelta;
}

int breakXOR(struct Line* line) {
    char* test = NULL;
    int numOfKeys = 255;
    int scores[numOfKeys];
    for (int i = 0; i < numOfKeys; i++) {
        test = keyedXOR(line->text, i);
        if (strlen(test) < 30) {
            scores[i] = 1000000000;
            continue;
        }
        scores[i] = scoreDecryptedText(test);
        free(test);
    }
    
    int smallestDelta = scores[0], key = 0;
    for (int i = 0; i < numOfKeys; i++) {
        if (scores[i] < smallestDelta) {
            key = i;
            smallestDelta = scores[i];
        }
    }
    
    line->key = key;
    line->decryptedText = keyedXOR(line->text, key);

    return smallestDelta;
}

void testEachLine(char* filename) {
    char* line = malloc(LINE_LENGTH);
    
    FILE* f = fopen(filename, "r");
    
    if (f == NULL) {
        perror("[-] Could not open specified file");
        exit(1);
    }
    
    int smallestDelta = 100000;
    int smallestLine = 0;
    struct Line* lines[NUMBER_OF_LINES];
    int x = 0;
    while(fgets(line, 62, f) != NULL)
    {
        lines[x] = malloc(sizeof(xorLine));
        lines[x]->lineNumber = x;
        if (line[strlen(line) - 1] == '\n') line[strlen(line)-1] = 0;
        lines[x]->text = pairHex(line, strlen(line));
        lines[x]->score = breakXOR(lines[x]);
        if (lines[x]->score < smallestDelta) {
            smallestDelta = lines[x]->score;
            smallestLine = x;
        }
        x++;
    }
    
    for (int i = 0; i < NUMBER_OF_LINES - 1; i++) {
        if (lines[i]->lineNumber == smallestLine) {
            printf("Line %d, with key %d: %s\n", lines[i]->lineNumber, lines[i]->key, lines[i]->decryptedText);
        }
        free(lines[i]->decryptedText);
        free(lines[i]->text);
        free(lines[i]);
    }
    
    fclose(f);
}

int main() {
    setupExpectedFreq();
    testEachLine("/Users/breadchris/Documents/Programming/CPlusPractice/Matasano Crypto Challenge/Matasano Crypto Challenge/challenge_4.txt");
}
