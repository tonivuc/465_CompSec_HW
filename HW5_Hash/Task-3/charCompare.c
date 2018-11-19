#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    size_t n = 0;
    int c;

    if (file == NULL) return NULL; //could not open file
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    code = malloc(f_size);

    while ((c = fgetc(file)) != EOF) {
        code[n++] = (char)c;
    }

    code[n] = '\0';        

    return code;
}

int main() {
    int i, diffCounter;
    char* string1 = readFile("oldHash.txt");
    char* string2 = readFile("newHash.txt");

    int biggestString = ( strlen(string1) >= strlen(string2) ? strlen(string1) : strlen(string2) );
    printf("oldHash length: %d\n", strlen(string1));
    printf("newHash length: %d\n", strlen(string2));

    for (i = 0; i < biggestString; i++) {
        if (string1[i] != string2[i]) {
            diffCounter++;
        }
    }
    printf("%d\n", diffCounter);
}