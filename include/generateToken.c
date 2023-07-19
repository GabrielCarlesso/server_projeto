#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX_TOKEN_LENGTH 64

void generateToken(const char* userId, const char* secret, char* token) {
    // Concatenar o ID do usuário e o segredo em uma única string
    char combinedString[MAX_TOKEN_LENGTH];
    snprintf(combinedString, sizeof(combinedString), "%s%s", userId, secret);

    // Calcular o hash da string combinada
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)combinedString, strlen(combinedString), hash);

    // Converter o hash para uma string hexadecimal
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        snprintf(&token[i * 2], MAX_TOKEN_LENGTH - (i * 2), "%02x", hash[i]);
    }
}

int main() {
    const char* userId = "12345";
    const char* secret = "secreto";

    char token[MAX_TOKEN_LENGTH];
    generateToken(userId, secret, token);

    printf("Token: %s\n", token);

    return 0;
}
