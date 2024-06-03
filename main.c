#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

#define SIZE 256

#define byte unsigned char
byte bigendian;

typedef unsigned char * Disk;

typedef struct {
    Disk* disk;
    int size;
} DiskStorage;

typedef struct {
    DiskStorage *storage;
    int size;
    void (*addDisk)(DiskStorage *self, Disk *disk);
    void (*removeDisk)(DiskStorage *self, int index);
    void (*readDiskFile)(DiskStorage *self, char *filename);
} Machine;

void addDisk(DiskStorage *storage, Disk *disk)
{
    storage->size++;
    storage->disk = (Disk *)realloc(storage->disk, storage->size * sizeof(Disk));
    storage->disk[storage->size - 1] = *disk;
}

void removeDisk(DiskStorage *self, int index)
{
    if (index < 0 || index >= self->size) {
        printf("Error: Index out of bounds\n");
        return;
    }

    self->size--;
    for (int i = index; i < self->size; i++) {
        self->disk[i] = self->disk[i + 1];
    }
    self->disk = (Disk *)realloc(self->disk, self->size * sizeof(Disk));
}

void readDiskFile(DiskStorage *self, char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: File not found\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    Disk disk = (Disk)malloc(size);
    fread(disk, size, 1, file);
    fclose(file);

    self->size++;
    self->disk = (Disk *)realloc(self->disk, self->size * sizeof(Disk));
    self->disk[self->size - 1] = disk;
}

Machine newMachine()
{
    Machine machine;
    machine.size = 0;
    machine.storage = (DiskStorage *)malloc(sizeof(DiskStorage));
    machine.addDisk = addDisk;
    machine.removeDisk = removeDisk;
    machine.readDiskFile = readDiskFile;
    return machine;
}

//get int
int getInt(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3) 
{
    if (bigendian) {
        return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
    } else {
        return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
    }
}

//get float
float getFloat(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3) {
    union {
        float f;
        uint32_t i;
    } u;

    if (bigendian) {
        u.i = ((uint32_t)b0 << 24) | ((uint32_t)b1 << 16) | ((uint32_t)b2 << 8) | (uint32_t)b3;
    } else {
        u.i = ((uint32_t)b3 << 24) | ((uint32_t)b2 << 16) | ((uint32_t)b1 << 8) | (uint32_t)b0;
    }

    return u.f;
}

int main() 
{
    int n = 1;

    bigendian = !(*(char *)&n == 1);
    //RGB *disk0 = (RGB *)malloc(sizeof(RGB)*disk0size);

    // Alocando memória dinamicamente para a matriz
    Machine machine = newMachine();
    machine.readDiskFile(machine.storage, "./tools/example.img");
    printf("content: %s\n", machine.storage->disk[0]);
    printf("size: %d\n", machine.storage->size);
    printf("as float: %f\n", getFloat(machine.storage->disk[0][0], machine.storage->disk[0][1], machine.storage->disk[0][2], machine.storage->disk[0][3]));
    printf("as int: %d\n", getInt(machine.storage->disk[0][0], machine.storage->disk[0][1], machine.storage->disk[0][2], machine.storage->disk[0][3]));
    
    
    return 0;
}
