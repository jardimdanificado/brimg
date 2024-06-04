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
    void (*set)(Disk *disk, int index, unsigned char data);
    void (*insert)(Disk *disk, int index, char *str);
    void (*remove)(Disk *disk, int index, int size);
    void (*move)(Disk *disk, int origin, int destiny, int size);
    void (*swap)(Disk *disk, int index1, int index2, int size);
    void (*shift)(Disk *disk, int index, int size, int direction);
    void (*random)(Disk *disk, int index, int size);
    void (*copy)(Disk *disk, int index, int size);
    void (*fill)(Disk *disk, int index, int size, unsigned char data);
} StandardFunctions;

typedef struct {
    DiskStorage *storage;
    StandardFunctions *std;
    int size;
    void (*addDisk)(DiskStorage *self, Disk *disk);
    void (*removeDisk)(DiskStorage *self, int index);
    void (*readDiskFile)(DiskStorage *self, char *filename);
} Machine;

// StandardFunctions
// StandardFunctions
// StandardFunctions

void _set(Disk *disk, int index, unsigned char data)
{
    (*disk)[index] = data;
}

void _insert(Disk *disk, int index, char *str)
{
    int size = strlen(str);
    int disksize = strlen(*disk);
    *disk = (Disk)realloc(*disk, (disksize + size) * sizeof(Disk));
    for (int i = disksize - 1; i >= index; i--)
    {
        (*disk)[i + size] = (*disk)[i];
    }
    for (int i = 0; i < size; i++)
    {
        (*disk)[index + i] = str[i];
    }

}

void _remove(Disk *disk, int index, int size)
{
    for (int i = index; i < size; i++) {
        (*disk)[i] = (*disk)[i + size];
    }
    *disk = (Disk)realloc(*disk, (size - size) * sizeof(Disk));
}

void _move(Disk *disk, int origin, int destiny, int size)
{
    int disksize = strlen(*disk);
    if (origin > destiny)
    {
        for (int i = 0; i < size; i++)
        {
            (*disk)[destiny + i] = (*disk)[origin + i];
        }
        for (int i = destiny + size; i < disksize; i++)
        {
            (*disk)[i] = (*disk)[i + size];
        }
    }
    else
    {
        for (int i = size - 1; i >= 0; i--)
        {
            (*disk)[destiny + i] = (*disk)[origin + i];
        }
        for (int i = origin; i < disksize; i++)
        {
            (*disk)[i] = (*disk)[i + size];
        }
    }
}

void _swap(Disk *disk, int index1, int index2, int size)
{
    int disksize = strlen(*disk);
    if (index1 > index2)
    {
        int temp = index1;
        index1 = index2;
        index2 = temp;
    }
    if (index1 + size > index2)
    {
        printf("Error: Swap indexes overlap\n");
        return;
    }
    else if (index1 < 0 || index2 < 0 || index1 >= disksize || index2 >= disksize || index2 + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        unsigned char temp = (*disk)[index1 + i];
        (*disk)[index1 + i] = (*disk)[index2 + i];
        (*disk)[index2 + i] = temp;
    }
}

void _shift(Disk *disk, int index, int size, int _shift)
{
    int disksize = strlen(*disk);
    if (index < 0 || index >= disksize || index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        return;
    }
    if (_shift > 0)
    {
        for (int i = disksize - 1; i >= index + size; i--)
        {
            // if going out shift size shift them to the start of the shift
            if (i > index + size)
            {
                (*disk)[i] = (*disk)[i - size + _shift];
            }
            else
            {
                (*disk)[i] = 0;
            }
        }
    }
    else
    {
        for (int i = index; i < disksize; i++)
        {
            // if going out shift size shift them to the start of the shift
            if (i < index + size)
            {
                (*disk)[i] = (*disk)[i - _shift];
            }
            else
            {
                (*disk)[i] = 0;
            }
        }
    }
}

void _random(Disk *disk, int index, int size)
{
    for (int i = 0; i < size; i++)
    {
        (*disk)[index + i] = rand() % 256;
    }
}

void _copy(Disk *disk, int index, int size)
{
    int disksize = strlen(*disk);
    if (index < 0 || index >= disksize || index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        (*disk)[index + i] = (*disk)[index + i - size];
    }
}

void _fill(Disk *disk, int index, int size, unsigned char data)
{
    for (int i = 0; i < size; i++)
    {
        (*disk)[index + i] = data;
    }
}

StandardFunctions newStandardFunctions()
{
    StandardFunctions std;
    std.set = _set;
    std.insert = _insert;
    std.remove = _remove;
    std.move = _move;
    std.swap = _swap;
    std.shift = _shift;
    std.random = _random;
    std.copy = _copy;
    std.fill = _fill;
    return std;
}

// DiskManagement functions
// DiskManagement functions
// DiskManagement functions

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
    machine.std = (StandardFunctions *)malloc(sizeof(StandardFunctions));
    *machine.std = newStandardFunctions();
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
    printf("size: %d\n", machine.storage->size);
    printf("as float: %f\n", getFloat(machine.storage->disk[0][0], machine.storage->disk[0][1], machine.storage->disk[0][2], machine.storage->disk[0][3]));
    printf("as int: %d\n", getInt(machine.storage->disk[0][0], machine.storage->disk[0][1], machine.storage->disk[0][2], machine.storage->disk[0][3]));
    
    machine.std->set(&machine.storage->disk[0], 0, 123);
    machine.std->insert(&machine.storage->disk[0], 1, "hello");
    printf("content: %s\n", machine.storage->disk[0]);


    union {
        float f;
        unsigned char b[4];
    } u;

    u.f = 3.999999;
    printf("float: %d %d %d %d\n", u.b[0], u.b[1], u.b[2], u.b[3]);
    
    return 0;
}
