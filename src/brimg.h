#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>

#define SIZE 256
#define VERSION_X 0
#define VERSION_Y 27

typedef unsigned char byte;

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

void _print(Disk disk, int index, int size);
void _scan(Disk *disk, int index, int size);
void _goto(Disk *disk, int position);

byte* get_bytes(Disk disk, int index, int size);
byte get_byte(Disk disk, int index);
byte* get_string(Disk disk, int index, int size);
int get_int(Disk disk, int index);

void set_byte(Disk *disk, int index, byte data);
void set_string(Disk *disk, int index, byte *str, int size);

Disk _run(Disk disk);

byte* disk_read(char *filename);
void disk_write(char *filename, byte *data);
