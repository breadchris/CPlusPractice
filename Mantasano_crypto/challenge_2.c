#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* hexLookup    = "0123456789abcdef";

int asciiToHex(char ascii) {
    char* tmp = hexLookup;
    int i = 1;
    
    if (ascii == '0')
        return 0;
    while (tmp = ((char*) ++tmp))
        if (*tmp == ascii)
            return i;
        else
            i++;
    return -1;
}

char* pairHex(char* hex) {
    if (strlen(hex) % 2 == 0) {
        char* paired = (char *) malloc(strlen(hex) / 2);
        for (int i = 0; i < strlen(hex); i += 2) {
            paired[i/2] = (asciiToHex(hex[i]) * 16) + asciiToHex(hex[i+1]);
        }
        return paired;
    }
    
    printf("[-] Given hex is not multiple of two");
    return NULL;
}

char* do_xor(char* a, char* b) {
    char* fixedA = pairHex(a);
    char* fixedB = pairHex(b);
    char* out = malloc(sizeof(fixedA));
    
    for (int i = 0; i < sizeof(fixedA); i++) {
        out[i] = fixedA[i] ^ fixedB[i];
    }
    
    return out;
}

int main() {
    char* a = "1c0111001f010100061a024b53535009181c";
    char* b = "686974207468652062756c6c277320657965";
    
    char* result = do_xor(a, b);
   
    char* tmp = result;
    for (int i = 0; i < sizeof(result); i++) {
        printf("%c ", *tmp);
    }
    
    printf("\n");
}