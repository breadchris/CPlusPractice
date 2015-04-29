//
//  challenge_5.c
//  Matasano Crypto Challenge
//
//  Created by Christopher Thompson on 3/22/15.
//  Copyright (c) 2015 Christopher Thompson. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* HEX_LOOKUP = "0123456789abcdef";

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

int main() {
    char* text = "Burning 'em, if you ain't quick and nimble I go crazy when I hear a cymbal";
    char* key = "ICE";
    char* enc = malloc(strlen(text));
    
    repeatedXOR(enc, text, key);
    
    char* formattedEnc = malloc(strlen(enc) * 2);
    formatAsHex(formattedEnc, enc);
    
    printf("%s\n", formattedEnc);
}