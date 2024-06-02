local starter = [[#include <stdio.h>
#include <stdlib.h>    
#include <stdbool.h>
#include <string.h>

char* read_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = malloc(length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }
    fread(buffer, 1, length, file);
    fclose(file);
    buffer[length] = '\0';
    return buffer;
}

void write_file(const char* path, const char* data) {
    FILE* file = fopen(path, "w");
    if (!file) {
        return;
    }
    fwrite(data, 1, strlen(data), file);
    fclose(file);
}

]]

local c = terralib.includecstring(starter)

return {read=c.read_file, write=c.write_file};