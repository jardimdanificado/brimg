#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <libtcc.h>

#define SIZE 256
#define VERSION "0.0.4"

#define byte unsigned char
byte bigendian;

typedef union {
    float f;
    uint32_t i;
    unsigned char b[4];
} Number;

typedef unsigned char * Disk;

void _set(Disk *disk, int index, unsigned char data);
void _insert(Disk *disk, int index, byte *str);
void _remove(Disk *disk, int index, int size);
void _move(Disk *disk, int origin, int destiny, int size);
void _swap(Disk *disk, int index1, int index2, int size);
void _shift(Disk *disk, int index, int size, int _shift);
void _random(Disk *disk, int index, int size);
void _copy(Disk *disk, int index, int destiny, int size);
void _fill(Disk *disk, int index, int size, unsigned char data);

Number get_number(unsigned char *bytes);
unsigned char* get_bytes(Disk disk, int index, int size);
unsigned char get_byte(Disk disk, int index);
char get_char(Disk disk, int index);
byte* get_string(Disk disk, int index, int size);
int get_int(Disk disk, int index);
float get_float(Disk disk, int index);

void set_byte(Disk *disk, int index, unsigned char data);
void set_char(Disk *disk, int index, char data);
void set_string(Disk *disk, int index, byte *str, int size);
void set_int(Disk *disk, int index, int data);
void set_float(Disk *disk, int index, float data);


void _set(Disk *disk, int index, unsigned char data);
void _insert(Disk *disk, int index, byte *str);
void _remove(Disk *disk, int index, int size);
void _move(Disk *disk, int origin, int destiny, int size);
void _swap(Disk *disk, int index1, int index2, int size);
void _shift(Disk *disk, int index, int size, int _shift);
void _random(Disk *disk, int index, int size);
void _copy(Disk *disk, int index, int destiny, int size);
void _fill(Disk *disk, int index, int size, unsigned char data);


byte* disk_read(char *filename);
void disk_write(char *filename, byte *data);