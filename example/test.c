
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <libtcc.h>

#define SIZE 256
#define VERSION "0.0.3"

#define byte uint8_t
byte bigendian;typedef union {
    float f;uint32_t i;uint8_t b[4];} Number;typedef uint8_t * Disk;void _set(Disk *disk, int index, uint8_t data);void _insert(Disk *disk, int index, char *str);void _remove(Disk *disk, int index, int size);void _move(Disk *disk, int origin, int destiny, int size);void _swap(Disk *disk, int index1, int index2, int size);void _shift(Disk *disk, int index, int size, int _shift);void _random(Disk *disk, int index, int size);void _copy(Disk *disk, int index, int destiny, int size);void _fill(Disk *disk, int index, int size, uint8_t data);Number get_number(uint8_t *bytes);uint8_t* get_bytes(Disk disk, int index, int size);uint8_t get_byte(Disk disk, int index);char* get_string(Disk disk, int index, int size);int get_int(Disk disk, int index);float get_float(Disk disk, int index);void set_byte(Disk *disk, int index, uint8_t data);void set_string(Disk *disk, int index, char *str, int size);void set_int(Disk *disk, int index, int data);void set_float(Disk *disk, int index, float data);void _set(Disk *disk, int index, uint8_t data);void _insert(Disk *disk, int index, char *str);void _remove(Disk *disk, int index, int size);void _move(Disk *disk, int origin, int destiny, int size);void _swap(Disk *disk, int index1, int index2, int size);void _shift(Disk *disk, int index, int size, int _shift);void _random(Disk *disk, int index, int size);void _copy(Disk *disk, int index, int destiny, int size);void _fill(Disk *disk, int index, int size, uint8_t data);char* disk_read(char *filename);void disk_write(char *filename, char *data);typedef struct 
{
    byte age;
    float weight;
    float height;
    char* name;
    int id;
} people;

people get_people(Disk *disk, int index)
{
    people _obj;
    _obj.age = get_byte(*disk, index + 0);
    _obj.weight = get_float(*disk, index + 1);
    _obj.height = get_float(*disk, index + 5);
    _obj.name = get_string(*disk, index + 9, 12);
    _obj.id = get_int(*disk, index + 21);
    return _obj;
}

void set_people(Disk *disk, int index, people _obj)
{
    set_byte(disk, index + 0, _obj.age);
    set_float(disk, index + 1, _obj.weight);
    set_float(disk, index + 5, _obj.height);
    set_string(disk, index + 9, _obj.name, 12);
    set_int(disk, index + 21, _obj.id);
}


    int main()
    {
        Disk disk0 = disk_read("test.img");//disk are always little endian, so we need to change the order of the bytes
        int size = strlen(disk0);printf("Disk0: %d\n", size);}
