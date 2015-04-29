#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* hexLookup    = "0123456789abcdef";

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
    int score = 0;
    for (int i = 0; i < strlen(a); i++) {
        // Test if char is in ascii range
        if (a[i] > 48 && a[i] < 122) score++;
    }
    return score;
}

char* breakXOR(char* a) {
    char* test = NULL;
    int numOfKeys = 255;
    int scores[numOfKeys];
    for (int i = 0; i < numOfKeys; i++) {
        test = keyedXOR(a, i);
        scores[i] = scoreDecryptedText(test);
        printf("%d (%d): %s\n", i, scores[i], test);
        free(test);
    }
    
    int biggest = 0, key = 0;
    for (int i = 0; i < numOfKeys; i++) {
        if (scores[i] > biggest) {
            key = i;
            biggest = scores[i];
        }
    }

    printf("%d\n", key);

    // 88 is the key
    return keyedXOR(a, 88);
}

int main() {
    char* enc = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char* fixedEnc = pairHex(enc, strlen(enc));
    
    char* result = breakXOR(fixedEnc);
    printf("%s\n", result);
}
