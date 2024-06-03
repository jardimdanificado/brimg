#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256

#define byte unsigned char
byte bigendian;

typedef unsigned char * Disk;

typedef struct {
    Disk* disks;
    int size;
} Deck;

typedef struct {
    Deck *disks;
    int size;
    void (*addDisk)(Deck *self, Disk *disk);
    void (*removeDisk)(Deck *self, int index);
    void (*readDiskFile)(Deck *self, char *filename);
} Machine;

void addDisk(Deck *self, Disk *disk)
{
    self->size++;
    self->disks = (Disk *)realloc(self->disks, self->size * sizeof(Disk));
    self->disks[self->size - 1] = *disk;
}

void removeDisk(Deck *self, int index)
{
    if (index < 0 || index >= self->size) {
        printf("Error: Index out of bounds\n");
        return;
    }

    self->size--;
    for (int i = index; i < self->size; i++) {
        self->disks[i] = self->disks[i + 1];
    }
    self->disks = (Disk *)realloc(self->disks, self->size * sizeof(Disk));
}

void readDiskFile(Deck *self, char *filename)
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
    self->disks = (Disk *)realloc(self->disks, self->size * sizeof(Disk));
    self->disks[self->size - 1] = disk;
}

Machine newMachine()
{
    Machine machine;
    machine.size = 0;
    machine.disks = (Deck *)malloc(sizeof(Deck));
    machine.addDisk = addDisk;
    machine.removeDisk = removeDisk;
    machine.readDiskFile = readDiskFile;
    return machine;
}

int main() 
{
    int n = 1;

    bigendian = !(*(char *)&n == 1);
    //RGB *disk0 = (RGB *)malloc(sizeof(RGB)*disk0size);

    // Alocando memória dinamicamente para a matriz
    Machine machine = newMachine();
    machine.readDiskFile(machine.disks, "./tools/example.img");
    printf("content: %s\n", machine.disks->disks[0]);
    
    
    return 0;
}
