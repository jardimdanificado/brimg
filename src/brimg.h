#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>

#define SIZE 256
#define VERSION_X 0
#define VERSION_Y 0
#define VERSION_Z 22

#define byte unsigned char

typedef byte* Disk;

// C api
void _state(Disk *disk, byte state);
void _set(Disk *disk, int index, int size, byte *data);
void _insert(Disk *disk, int index, byte *str);
void _delete(Disk *disk, int index, int size);
void _move(Disk *disk, int origin, int destiny, int size);
void _swap(Disk *disk, int index1, int index2, int size);
void _shift(Disk *disk, int index, int size, int _shift);
void _random(Disk *disk, int index, int size);
void _copy(Disk *disk, int index, int destiny, int size);
void _fill(Disk *disk, int index, int size, byte data);
void _reverse(Disk *disk, int index, int size);
void _sort(Disk *disk, int index, int size);
void _find(Disk *disk, int index, int size, byte* data, int result);


void _print(Disk disk, int index, int size);
void _goto(Disk *disk, int position);

byte* get_bytes(Disk disk, int index, int size);
byte get_byte(Disk disk, int index);
byte* get_string(Disk disk, int index, int size);
int get_int(Disk disk, int index);
float get_float(Disk disk, int index);
short get_short(Disk disk, int index);
double get_double(Disk disk, int index);
long get_long(Disk disk, int index);
long double get_long_double(Disk disk, int index);

void set_byte(Disk *disk, int index, byte data);
void set_string(Disk *disk, int index, byte *str, int size);
void set_int(Disk *disk, int index, int data);
void set_float(Disk *disk, int index, float data);
void set_short(Disk *disk, int index, short data);
void set_double(Disk *disk, int index, double data);
void set_long(Disk *disk, int index, long data);
void set_long_double(Disk *disk, int index, long double data);

Disk _run(Disk disk);

byte* disk_read(char *filename);
void disk_write(char *filename, byte *data);