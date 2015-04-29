//
//  challenge_6.c
//  Matasano Crypto Challenge
//
//  Created by Christopher Thompson on 3/23/15.
//  Copyright (c) 2015 Christopher Thompson. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* HEX_LOOKUP = "0123456789abcdef";
int MIN_KEYSIZE = 2;
int MAX_KEYSIZE = 40;
int LINE_LENGTH = 60;
static const unsigned char pr2six[256] =
{
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};


void strip(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}

void formatAsHex(char* out, char* str) {
    for (int i = 0; i < strlen(str); ++i) {
        out[i*2] =   HEX_LOOKUP[(str[i] & 0xf0) >> 4];
        out[i*2+1] = HEX_LOOKUP[(str[i] & 0xf)];
    }
}

void repeatedXOR(char* encrypted, char* text, char* key) {
    for (int i = 0; i < strlen(text); i++) {
        encrypted[i] = text[i] ^ key[i % strlen(key)];
    }
}

void blockToSingleByte(char* byteBlocks[], char* str, unsigned int keysize) {
    for (int i = 0; i < strlen(str); i++) {
        byteBlocks[i % keysize][i / keysize] = str[i];
    }
}

int keyedEditDistance(char* enc, unsigned int nBytes) {
    if (strlen(enc) < 2 * nBytes) {
        perror("[-] Not enough characters in given encrypted string");
        return -1;
    }
    
    int distance = 0;
    for (int i = 0; i < nBytes; ++i) {
        int xor = enc[i] ^ enc[nBytes + i];
        do {
            if (xor & 0x1) distance++;
        } while((xor = xor >> 1));
    }
    
    return distance;
}

unsigned int findKeysize(char* enc) {
    unsigned int keysize = 0;
    unsigned int smallestDistance = (unsigned int) -1;
    unsigned int distance = 0;
    for (int i = MIN_KEYSIZE; i < MAX_KEYSIZE; i++) {
        if ((distance = keyedEditDistance(enc, i)) < smallestDistance) {
            smallestDistance = distance;
            keysize = i;
        }
    }
    return keysize;
}

int Base64decode(char *bufplain, const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register unsigned char *bufout;
    register int nprbytes;
    
    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;
    
    bufout = (unsigned char *) bufplain;
    bufin = (const unsigned char *) bufcoded;
    
    while (nprbytes > 4) {
        *(bufout++) =
        (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
        *(bufout++) =
        (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
        *(bufout++) =
        (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
        bufin += 4;
        nprbytes -= 4;
    }
    
    if (nprbytes > 1) {
        *(bufout++) =
        (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    }
    if (nprbytes > 2) {
        *(bufout++) =
        (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    }
    if (nprbytes > 3) {
        *(bufout++) =
        (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    }
    
    *(bufout++) = '\0';
    nbytesdecoded -= (4 - nprbytes) & 3;
    return nbytesdecoded;
}

char* readFile(const char* filename) {
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);
    
    string[fsize] = 0;
    return string;
}

int main() {
    char* b64EncFile = readFile("/Users/breadchris/Documents/Programming/CPlusPractice/Matasano Crypto Challenge/Matasano Crypto Challenge/challenge_6.txt");
    strip(b64EncFile);
    char* encFile = malloc(strlen(b64EncFile));
    Base64decode(encFile, b64EncFile);
    unsigned int keysize = findKeysize(encFile);
    char* byteBlocks[keysize];
    
    for (int i = 0; i < keysize - 1; i++) {
        byteBlocks[i] = malloc(strlen(encFile) / keysize);
    }
    blockToSingleByte(byteBlocks, encFile, keysize);
    // for each block:
    //      find key
}
