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
            printf("%02x ", paired[i/2]);
        }
        puts("\n");
        return paired;
    }
    
    printf("[-] Given hex is not multiple of two");
    return NULL;
}

char* do_xor(char* a, char* b) {
    unsigned long size = strlen(a);
    char* fixedA = pairHex(a, size);
    char* fixedB = pairHex(b, size);
    char* out = malloc(size / 2);

    for (int i = 0; i < size / 2; i++) {
        out[i] = fixedA[i] ^ fixedB[i];
    }
    
    return out;
}

void test() {
    char* a = "1c0111001f010100061a024b53535009181c";
    char* b = "686974207468652062756c6c277320657965";
    
    char* result = do_xor(a, b);
   
    char* tmp = result;
    for (int i = 0; i < strlen(result); i++) {
        printf("%x", tmp[i]);
    }
    
    printf("\n");
}
