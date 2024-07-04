#include "brimg.h"

byte *disk_read(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == 0)
    {
        printf("Error: File not found\n");
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    byte *buffer = (byte *)malloc(size + 1);
    fread(buffer, size, 1, file);
    fclose(file);

    buffer[size] = '\0';
    return buffer;
}

void disk_write(char *filename, byte *data)
{
    FILE *file = fopen(filename, "w");
    if (file == 0)
    {
        printf("Error: File not found\n");
        exit(1);
    }

    fwrite(data, strlen(data), 1, file);
    fclose(file);
}

int main (int argc, char *argv[])
{
    Disk disk = disk_read(argv[1]);
    if (disk == NULL) {
        printf("Error: disk_read\n");
        return 1;
    }

    if (disk[2] == VERSION_X && disk[3] == VERSION_Y) 
    {
        printf("brimg v%d.%d\n", disk[2], disk[3]);
    } 
    else 
    {
        printf("Warning: version mismatch, expected brimg v%d.%d bytecode but got brimg v%d.%d bytecode\n", VERSION_X, VERSION_Y, disk[2], disk[3]);
    }
    _run(disk);
    return 0;
}