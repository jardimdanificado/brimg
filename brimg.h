#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <libtcc.h>

#define SIZE 256
#define VERSION "0.0.2"

#define byte uint8_t
byte bigendian;

typedef union {
    float f;
    uint32_t i;
    uint8_t b[4];
} Number;

typedef uint8_t * Disk;

typedef struct {
    void (*set)(Disk *disk, int index, uint8_t data);
    void (*insert)(Disk *disk, int index, char *str);
    void (*remove)(Disk *disk, int index, int size);
    void (*move)(Disk *disk, int origin, int destiny, int size);
    void (*swap)(Disk *disk, int index1, int index2, int size);
    void (*shift)(Disk *disk, int index, int size, int direction);
    void (*random)(Disk *disk, int index, int size);
    void (*copy)(Disk *disk, int index, int destiny, int size);
    void (*fill)(Disk *disk, int index, int size, uint8_t data);
} StandardFunctions;

Number get_number(uint8_t *bytes);
uint8_t* get_bytes(Disk disk, int index, int size);
uint8_t get_byte(Disk disk, int index);
char* get_string(Disk disk, int index, int size);
int get_int(Disk disk, int index);
float get_float(Disk disk, int index);


void _set(Disk *disk, int index, uint8_t data);
void _insert(Disk *disk, int index, char *str);
void _remove(Disk *disk, int index, int size);
void _move(Disk *disk, int origin, int destiny, int size);
void _swap(Disk *disk, int index1, int index2, int size);
void _shift(Disk *disk, int index, int size, int _shift);
void _random(Disk *disk, int index, int size);
void _copy(Disk *disk, int index, int destiny, int size);
void _fill(Disk *disk, int index, int size, uint8_t data);


char* diskread(char *filename);
void diskwrite(char *filename, char *data);