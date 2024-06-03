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
    union {
        uint32_t i;
        char b[4];
    } u;

    if (bigendian) {
        u.b[0] = b0;
        u.b[1] = b1;
        u.b[2] = b2;
        u.b[3] = b3;
    } else {
        u.b[3] = b0;
        u.b[2] = b1;
        u.b[1] = b2;
        u.b[0] = b3;
    }

    return u.i;
}

float getFloat( unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3)
{
 
    union {
        float f;
        unsigned char b[4];
    } u;

    if (bigendian) {
        u.b[0] = b0;
        u.b[1] = b1;
        u.b[2] = b2;
        u.b[3] = b3;
    } else {
        u.b[3] = b0;
        u.b[2] = b1;
        u.b[1] = b2;
        u.b[0] = b3;
    }

    return u.f;
}

int main(int argc, char *argv[]) 
{
    char *inpath = (argc >= 2) ? argv[1] : "./tools/example.img";


    int n = 1;

    bigendian = !(*(char *)&n == 1);
    printf("bigendian: %d\n", bigendian);
    
    Machine machine = newMachine();
    machine.readDiskFile(machine.storage, inpath);//disk are always little endian, so we need to change the order of the bytes
    printf("content: %s\n", machine.storage->disk[0]);
    printf("size: %d\n", machine.storage->size);
    printf("as float: %f\n", getFloat(machine.storage->disk[0][0], machine.storage->disk[0][1], machine.storage->disk[0][2], machine.storage->disk[0][3]));
    printf("as int: %d\n", getInt(machine.storage->disk[0][0], machine.storage->disk[0][1], machine.storage->disk[0][2], machine.storage->disk[0][3]));
    
    union {
        float f;
        unsigned char b[4];
    } u;

    u.f = 3.999999;
    printf("float: %d %d %d %d\n", u.b[0], u.b[1], u.b[2], u.b[3]);
    
    return 0;
}
