#include "brimg.h"

// size of the pointer in bytes
// 1,2 or 4 bytes, 1 is 256bytes, 2 is 64kb, 4 is 4gb
#define MEMSIZE 2

// StandardFunctions C Api
// StandardFunctions C Api
// StandardFunctions C Api










void _state(Disk *disk, byte state)
{
    (*disk)[0] = state;
}


void _set(Disk *disk, int index, int size,byte *data)
{
    for (int i = 0; i < size; i++)
    {
        (*disk)[index + i] = data[i];
    }
}


void _insert(Disk *disk, int index, byte *str)
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


void _delete(Disk *disk, int index, int size)
{
    int disksize = strlen(*disk);
    for (int i = index; i < disksize; i++)
    {
        if (i + size < disksize)
        {
            (*disk)[i] = (*disk)[i + size];
        }
        else
        {
            (*disk)[i] = 0;
        }
    }
    *disk = (Disk)realloc(*disk, (disksize - size) * sizeof(Disk));
}


void _move(Disk *disk, int origin, int destiny, int size)
{
    int disksize = strlen(*disk);
    if (origin > destiny)
    {
        int temp = origin;
        origin = destiny;
        destiny = temp;
    }
    if (origin < 0 || destiny < 0 || origin >= disksize || destiny >= disksize || origin + size > disksize || destiny + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    Disk temp = (Disk)malloc(size * sizeof(Disk));
    for (int i = 0; i < size; i++)
    {
        temp[i] = (*disk)[origin + i];
    }
    for (int i = 0; i < size; i++)
    {
        (*disk)[origin + i] = (*disk)[destiny + i];
    }
    for (int i = 0; i < size; i++)
    {
        (*disk)[destiny + i] = temp[i];
    }
    free(temp);

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
        exit(1);
    }
    else if (index1 < 0 || index2 < 0 || index1 >= disksize || index2 >= disksize || index2 + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    for (int i = 0; i < size; i++)
    {
        byte temp = (*disk)[index1 + i];
        (*disk)[index1 + i] = (*disk)[index2 + i];
        (*disk)[index2 + i] = temp;
    }
}

void _shift(Disk *disk, int index, int size, int _shift)
{
    int disksize = strlen(*disk);
    // slide items based of _shift - to the left, + to the right, 0 change nothing, 0123456789 -> 0781234569 with shift 3 in a interval from 1 to 8
    if (index < 0 || index >= disksize || index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    if (_shift > 0)
    {
        for (int i = 0; i < _shift; i++)
        {
            byte temp = (*disk)[index];
            for (int j = index; j < index + size - 1; j++)
            {
                (*disk)[j] = (*disk)[j + 1];
            }
            (*disk)[index + size - 1] = temp;
        }
    }
    else if (_shift < 0)
    {
        for (int i = 0; i < -_shift; i++)
        {
            byte temp = (*disk)[index + size - 1];
            for (int j = index + size - 1; j > index; j--)
            {
                (*disk)[j] = (*disk)[j - 1];
            }
            (*disk)[index] = temp;
        }
    }
}

void _random(Disk *disk, int index, int size)
{
    //set seed 
    srand(time(NULL));

    for (int i = 0; i < size; i++)
    {
        (*disk)[index + i] = rand() % 256;
    }
}

void _copy(Disk *disk, int index, int destiny, int size)
{
    int disksize = strlen(*disk);
    if (index < 0 || destiny < 0 || index >= disksize || destiny >= disksize || index + size > disksize || destiny + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    for (int i = 0; i < size; i++)
    {
        (*disk)[destiny + i] = (*disk)[index + i];
    }
}

void _fill(Disk *disk, int index, int size, byte data)
{
    for (int i = 0; i < size; i++)
    {
        (*disk)[index + i] = data;
    }
}

void _reverse(Disk *disk, int index, int size)
{
    int disksize = strlen(*disk);
    if (index < 0 || index >= disksize || index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    for (int i = 0; i < size / 2; i++)
    {
        byte temp = (*disk)[index + i];
        (*disk)[index + i] = (*disk)[index + size - 1 - i];
        (*disk)[index + size - 1 - i] = temp;
    }
}

void _sort(Disk *disk, int index, int size)
{
    int disksize = strlen(*disk);
    if (index < 0 || index >= disksize || index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if ((*disk)[index + i] > (*disk)[index + j])
            {
                byte temp = (*disk)[index + i];
                (*disk)[index + i] = (*disk)[index + j];
                (*disk)[index + j] = temp;
            }
        }
    }
}







//conditions
//conditions
//conditions








void _equal(Disk *disk, int posi1, int posi2, int size, int result)
{
    byte _true = 1;
    for (int i = 0; i < size; i++)
    {
        if ((*disk)[posi1 + i] != (*disk)[posi2 + i])
        {
            _true = 0;
            break;
        }
    }
    (*disk)[result] = _true;
}

void _not_equal(Disk *disk, int posi1, int posi2, int size, int result)
{
    byte _true = 1;
    for (int i = 0; i < size; i++)
    {
        if ((*disk)[posi1 + i] == (*disk)[posi2 + i])
        {
            _true = 0;
            break;
        }
    }
    (*disk)[result] = _true;
}

void _greater(Disk *disk, int posi1, int posi2, int size, int result)
{
    byte _true = 1;
    for (int i = 0; i < size; i++)
    {
        if ((*disk)[posi1 + i] <= (*disk)[posi2 + i])
        {
            _true = 0;
            break;
        }
    }
    (*disk)[result] = _true;
}

void _less(Disk *disk, int posi1, int posi2, int size, int result)
{
    byte _true = 1;
    for (int i = 0; i < size; i++)
    {
        if ((*disk)[posi1 + i] >= (*disk)[posi2 + i])
        {
            _true = 0;
            break;
        }
    }
    (*disk)[result] = _true;
}

void _greater_or_equal(Disk *disk, int posi1, int posi2, int size, int result)
{
    byte _true = 1;
    for (int i = 0; i < size; i++)
    {
        if ((*disk)[posi1 + i] < (*disk)[posi2 + i])
        {
            _true = 0;
            break;
        }
    }
    (*disk)[result] = _true;
}

void _less_or_equal(Disk *disk, int posi1, int posi2, int size, int result)
{
    byte _true = 1;
    for (int i = 0; i < size; i++)
    {
        if ((*disk)[posi1 + i] > (*disk)[posi2 + i])
        {
            _true = 0;
            break;
        }
    }
    (*disk)[result] = _true;
}

void _and(Disk *disk, int posi, int size, int result)
{
    byte _true = 1;
    for (int i = 0; i < size; i++)
    {
        if ((*disk)[posi + i] == 0)
        {
            _true = 0;
            break;
        }
    }
    (*disk)[result] = _true;
}

void _or(Disk *disk, int posi, int size, int result)
{
    byte _true = 0;
    for (int i = 0; i < size; i++)
    {
        if ((*disk)[posi + i] != 0)
        {
            _true = 1;
            break;
        }
    }
    (*disk)[result] = _true;
}







// etc
// etc
// etc






void _print(Disk disk, int index, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%c", disk[index + i]);
    }
    printf("\n");
}

void _scan(Disk *disk, int index, int size)
{
    char *str = (char *)malloc(size * sizeof(char));
    scanf("%s", str);
    _set(disk, index, size, (byte *)str);
}

void _goto(Disk *disk, int position)
{
    set_int(disk, 4, position);
}







// DiskManagement functions
// DiskManagement functions
// DiskManagement functions







byte *disk_read(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: File not found\n");
        return NULL;
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
    if (file == NULL)
    {
        printf("Error: File not found\n");
        exit(1);
    }

    fwrite(data, strlen(data), 1, file);
    fclose(file);
}







// type gets
// type gets
// type gets








byte* get_bytes(Disk disk, int index, int size)
{
    byte *bytes = (byte *)malloc(size * sizeof(byte));
    for (int i = 0; i < size; i++)
    {
        bytes[i] = disk[index + i];
    }
    return bytes;
}


byte get_byte(Disk disk, int index)
{
    return disk[index];
}

short get_short(Disk disk, int index)
{
    return disk[index] | disk[index + 1] << 8;
}

long get_long(Disk disk, int index)
{
    union {
        long l;
        byte b[4];
    } u;
    u.b[0] = disk[index + 0];
    u.b[1] = disk[index + 1];
    u.b[2] = disk[index + 2];
    u.b[3] = disk[index + 3];
    return u.l;
}

double get_double(Disk disk, int index)
{
    union {
        double d;
        byte b[8];
    } u;
    u.b[0] = disk[index + 0];
    u.b[1] = disk[index + 1];
    u.b[2] = disk[index + 2];
    u.b[3] = disk[index + 3];
    u.b[4] = disk[index + 4];
    u.b[5] = disk[index + 5];
    u.b[6] = disk[index + 6];
    u.b[7] = disk[index + 7];
    return u.d;
}

long double get_long_double(Disk disk, int index)
{
    union {
        long double d;
        byte b[10];
    } u;
    u.b[0] = disk[index + 0];
    u.b[1] = disk[index + 1];
    u.b[2] = disk[index + 2];
    u.b[3] = disk[index + 3];
    u.b[4] = disk[index + 4];
    u.b[5] = disk[index + 5];
    u.b[6] = disk[index + 6];
    u.b[7] = disk[index + 7];
    u.b[8] = disk[index + 8];
    u.b[9] = disk[index + 9];
    return u.d;
}

byte* get_string(Disk disk, int index, int size)
{
    byte *str = (byte *)malloc(size * sizeof(byte));
    for (int i = 0; i < size; i++)
    {
        str[i] = disk[index + i];
    }
    return str;
}

int get_int(Disk disk, int index)
{
    union {
        int i;
        byte b[4];
    } u;
    u.b[0] = disk[index + 0];
    u.b[1] = disk[index + 1];
    u.b[2] = disk[index + 2];
    u.b[3] = disk[index + 3];
    return u.i;
}

// p(o)int(er) or p_int or pointer_int
// this get a pointer based on the default memory size
int get_ptr(Disk disk, int index)
{
    int position = 0;
    if (MEMSIZE == 1)
    {
        position = get_byte(disk, index);
    }
    else if (MEMSIZE == 2)
    {
        position = get_short(disk, index);
    }
    else if (MEMSIZE == 4)
    {
        position = get_int(disk, index);
    }
    return position;
}

float get_float(Disk disk, int index)
{
    union {
        float f;
        byte b[4];
    } u;
    u.b[0] = disk[index + 0];
    u.b[1] = disk[index + 1];
    u.b[2] = disk[index + 2];
    u.b[3] = disk[index + 3];
    return u.f;
}








// sets 
// sets 
// sets 








void set_byte(Disk *disk, int index, byte data)
{
    _set(disk, index, 1,(byte[1]){data});
}

void set_string(Disk *disk, int index, byte *str, int size)
{
    _set(disk, index, strlen(str), str);
}

void set_int(Disk *disk, int index, int data)
{
    union {
        int i;
        byte b[4];
    } u;
    u.i = data;
    _set(disk, index, 4, u.b);
}

void set_ptr(Disk *disk, int index, int data)
{
    int size = MEMSIZE;
    for (int i = 0; i < size; i++)
    {
        (*disk)[index + i] = (data / (int)pow(256, size - i - 1)) % 256;
    }
}

void set_float(Disk *disk, int index, float data)
{
    union {
        float f;
        byte b[4];
    } u;
    u.f = data;
    byte *bytes = get_bytes((byte *)&u, 0, 4);
    _set(disk, index, 4, bytes);
}

void set_short(Disk *disk, int index, short data)
{
    union {
        short s;
        byte b[2];
    } u;
    u.s = data;
    _set(disk, index, 2, (byte[2]){u.b[0],u.b[1]});
}

void set_double(Disk *disk, int index, double data)
{
    union {
        double d;
        byte b[8];
    } u;
    u.d = data;
    _set(disk, index, 8, (byte[8]){u.b[0],u.b[1],u.b[2],u.b[3],u.b[4],u.b[5],u.b[6],u.b[7]});
}

void set_long(Disk *disk, int index, long data)
{
    union {
        long l;
        byte b[8];
    } u;
    u.l = data;
    _set(disk, index, 8, (byte[8]){u.b[0],u.b[1],u.b[2],u.b[3],u.b[4],u.b[5],u.b[6],u.b[7]});
}

void set_long_double(Disk *disk, int index, long double data)
{
    union {
        long double d;
        byte b[10];
    } u;
    u.d = data;
    _set(disk, index, 10, (byte[10]){u.b[0],u.b[1],u.b[2],u.b[3],u.b[4],u.b[5],u.b[6],u.b[7],u.b[8],u.b[9]});
}










// callers
// callers
// callers











Disk caller_state(Disk disk, int index)
{
    disk[0] = get_byte(disk, index + 1);
    _goto(&disk, index + 2);
    return disk;
}

Disk caller_set(Disk disk, int index)
{
    int size = get_ptr(disk, index + 1 + MEMSIZE);
    byte *data = get_bytes(disk, index + 1 + MEMSIZE*2, size);
    int _index = get_ptr(disk, index + 1);
    for (int i = 0; i < size; i++)
    {
        disk[_index + i] = data[i];
    }
    _goto(&disk, index + 1 + MEMSIZE*2 + size);
    return disk;
}

Disk caller_insert(Disk disk, int index)
{
    int size = get_ptr(disk, index + 1 + MEMSIZE);
    byte *str = get_bytes(disk, index + 1 + MEMSIZE*2, size);
    
    int disksize = strlen(disk);
    int _index = get_ptr(disk, index + 1);
    disk = (Disk)realloc(disk, (disksize + size) * sizeof(Disk));
    
    for (int i = disksize - 1; i >= _index; i--)
    {
        disk[i + size] = disk[i];
    }
    for (int i = 0; i < size; i++)
    {
        disk[_index + i] = str[i];
    }
    
    _goto(&disk, index + 1 + MEMSIZE*2 + size);
    return disk;
}

Disk caller_delete(Disk disk, int index)
{
    int disksize = strlen(disk);
    int size = get_ptr(disk, index + 1 + MEMSIZE);
    int _index = get_ptr(disk, index + 1);
    for (int i = _index; i < size; i++)
    {
        if (i + size < disksize)
        {
            disk[i] = disk[i + size];
        }
        else
        {
            disk[i] = 0;
        }
    }
    disk = (Disk)realloc(disk, (disksize - size) * sizeof(Disk));
    _goto(&disk, index + 9);
    return disk;
}

Disk caller_move(Disk disk, int index)
{
    int disksize = strlen(disk);
    int size = get_ptr(disk, index + 1 + MEMSIZE*2);
    int origin = get_ptr(disk, index + 1);
    int destiny = get_ptr(disk, index + 1 + MEMSIZE);

    if (origin > destiny)
    {
        int temp = origin;
        origin = destiny;
        destiny = temp;
    }
    if (origin < 0 || destiny < 0 || origin >= disksize || destiny >= disksize || origin + size > disksize || destiny + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    Disk temp = (Disk)malloc(size * sizeof(Disk));
    for (int i = 0; i < size; i++)
    {
        temp[i] = disk[origin + i];
    }
    for (int i = 0; i < size; i++)
    {
        (disk)[origin + i] = (disk)[destiny + i];
    }
    for (int i = 0; i < size; i++)
    {
        (disk)[destiny + i] = temp[i];
    }
    free(temp);
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_swap(Disk disk, int index)
{
    int disksize = strlen(disk);
    int index1 = get_ptr(disk, index + 1);
    int index2 = get_ptr(disk, index + 1 + MEMSIZE);
    int size = get_ptr(disk, index + 1 + MEMSIZE*2);
    if (index1 > index2)
    {
        int temp = index1;
        index1 = index2;
        index2 = temp;
    }
    if (index1 + size > index2)
    {
        printf("Error: Swap indexes overlap\n");
        exit(1);
    }
    else if (index1 < 0 || index2 < 0 || index1 >= disksize || index2 >= disksize || index2 + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    for (int i = 0; i < size; i++)
    {
        byte temp = disk[index1 + i];
        disk[index1 + i] = disk[index2 + i];
        disk[index2 + i] = temp;
    }
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_shift(Disk disk, int index)
{
    int size = get_ptr(disk, index + 1 + MEMSIZE);
    int _shift = get_ptr(disk, index + 1 + MEMSIZE*2);
    int _index = get_ptr(disk, index + 1);
    int disksize = strlen(disk);
    if (_index < 0 || _index >= disksize || _index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    if (_shift > 0)
    {
        for (int i = 0; i < _shift; i++)
        {
            byte temp = disk[_index];
            for (int j = _index; j < _index + size - 1; j++)
            {
                disk[j] = disk[j + 1];
            }
            disk[_index + size - 1] = temp;
        }
    }
    else if (_shift < 0)
    {
        for (int i = 0; i < -_shift; i++)
        {
            byte temp = disk[_index + size - 1];
            for (int j = _index + size - 1; j > _index; j--)
            {
                disk[j] = disk[j - 1];
            }
            disk[_index] = temp;
        }
    }
    
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_randomize(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    srand(time(NULL));
    int size = get_ptr(disk, _index + 1 + MEMSIZE);
    for (int i = 0; i < size; i++)
    {
        disk[_index + i] = rand() % 256;
    }
    _goto(&disk, _index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_copy(Disk disk, int index)
{
    int disksize = strlen(disk);
    int size = get_ptr(disk, index + 1 + MEMSIZE*2);
    int _index = get_ptr(disk, index + 1);
    int destiny = get_ptr(disk, index + 1 + MEMSIZE);
    if (_index < 0 || destiny < 0 || _index >= disksize || destiny >= disksize || _index + size > disksize || destiny + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    for (int i = 0; i < size; i++)
    {
        disk[destiny + i] = disk[_index + i];
    }
    _goto(&disk, _index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_fill(Disk disk, int index)
{
    int size = get_ptr(disk, index + 1 + MEMSIZE);
    byte data = get_byte(disk, index + 1 + MEMSIZE*2);
    int _index = get_ptr(disk, index + 1);
    for (int i = 0; i < size; i++)
    {
        disk[_index + i] = data;
    }
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_reverse(Disk disk, int index)
{
    int disksize = strlen(disk);
    int size = get_ptr(disk, index + 1 + MEMSIZE);
    int _index = get_ptr(disk, index + 1);

    if (_index < 0 || _index >= disksize || _index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }

    for (int i = 0; i < size / 2; i++)
    {
        byte temp = disk[_index + i];
        disk[_index + i] = disk[_index + size - 1 - i];
        disk[_index + size - 1 - i] = temp;
    }
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_sort(Disk disk, int index)
{
    int disksize = strlen(disk);
    int size = get_ptr(disk, index + 1 + MEMSIZE);
    int _index = get_ptr(disk, index + 1);

    if (_index < 0 || _index >= disksize || _index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (disk[_index + i] > disk[_index + j])
            {
                byte temp = disk[_index + i];
                disk[_index + i] = disk[_index + j];
                disk[_index + j] = temp;
            }
        }
    }
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

//find (position, size, result, patternsize, pattern)
Disk caller_find(Disk disk, int index)
{
    int disksize = strlen(disk);
    int position = get_ptr(disk, index + 1);
    int size = get_ptr(disk, index + 1 + MEMSIZE);
    int result = get_ptr(disk, index + 1 + MEMSIZE*2);
    int patternsize = get_ptr(disk, index + 1 + MEMSIZE*3);
    byte *pattern = get_bytes(disk, index + 1 + MEMSIZE*4, patternsize);
    char *point = strstr(disk + position, pattern);
    position = (int*)(point) - (int*)(disk);
    if (position < 0 || position >= disksize || position + size > disksize)
    {
        printf("Error: Index out of bounds\n");
    }
    else
    {
        set_ptr(&disk, result, position);
    }
    _goto(&disk, index + 1 + MEMSIZE*4 + patternsize);
    return disk;
}

/*
types:
0 = void 0 bytes
1 = byte 1 byte
2 = short 2 bytes
3 = int 4 bytes
4 = long 8 bytes
5 = float 4 bytes
6 = double 8 bytes
7 = long double 10 bytes
*/

Disk caller_convert(Disk disk, int index)
{
    int disksize = strlen(disk);
    int position = get_ptr(disk, index + 1);
    int oldtype = get_byte(disk, index + 1 + MEMSIZE);
    int newtype = get_byte(disk, index + 2 + MEMSIZE);
    if (oldtype == newtype)
    {
        _goto(&disk, index + 2 + MEMSIZE*2);
        return disk;
    }
    if (oldtype == 0)
    {
        printf("Error: Cannot convert void type\n");
        exit(1);
    }
    if (newtype == 0)
    {
        printf("Error: Cannot convert to void type\n");
        exit(1);
    }
    if (oldtype == 1)
    {
        if (newtype == 2)
        {
            set_short(&disk, position, (short)get_byte(disk, position));
        }
        else if (newtype == 3)
        {
            set_int(&disk, position, (int)get_byte(disk, position));
        }
        else if (newtype == 4)
        {
            set_long(&disk, position, (long)get_byte(disk, position));
        }
        else if (newtype == 5)
        {
            set_float(&disk, position, (float)get_byte(disk, position));
        }
        else if (newtype == 6)
        {
            set_double(&disk, position, (double)get_byte(disk, position));
        }
        else if (newtype == 7)
        {
            set_long_double(&disk, position, (long double)get_byte(disk, position));
        }
    }
    else if (oldtype == 2)
    {
        if (newtype == 1)
        {
            set_byte(&disk, position, (byte)get_short(disk, position));
        }
        else if (newtype == 3)
        {
            set_int(&disk, position, (int)get_short(disk, position));
        }
        else if (newtype == 4)
        {
            set_long(&disk, position, (long)get_short(disk, position));
        }
        else if (newtype == 5)
        {
            set_float(&disk, position, (float)get_short(disk, position));
        }
        else if (newtype == 6)
        {
            set_double(&disk, position, (double)get_short(disk, position));
        }
        else if (newtype == 7)
        {
            set_long_double(&disk, position, (long double)get_short(disk, position));
        }
    }
    else if (oldtype == 3)
    {
        if (newtype == 1)
        {
            set_byte(&disk, position, (byte)get_int(disk, position));
        }
        else if (newtype == 2)
        {
            set_short(&disk, position, (short)get_int(disk, position));
        }
        else if (newtype == 4)
        {
            set_long(&disk, position, (long)get_int(disk, position));
        }
        else if (newtype == 5)
        {
            set_float(&disk, position, (float)get_int(disk, position));
        }
        else if (newtype == 6)
        {
            set_double(&disk, position, (double)get_int(disk, position));
        }
        else if (newtype == 7)
        {
            set_long_double(&disk, position, (long double)get_int(disk, position));
        }
    }
    else if (oldtype == 4)
    {
        if (newtype == 1)
        {
            set_byte(&disk, position, (byte)get_long(disk, position));
        }
        else if (newtype == 2)
        {
            set_short(&disk, position, (short)get_long(disk, position));
        }
        else if (newtype == 3)
        {
            set_int(&disk, position, (int)get_long(disk, position));
        }
        else if (newtype == 5)
        {
            set_float(&disk, position, (float)get_long(disk, position));
        }
        else if (newtype == 6)
        {
            set_double(&disk, position, (double)get_long(disk, position));
        }
        else if (newtype == 7)
        {
            set_long_double(&disk, position, (long double)get_long(disk, position));
        }
    }
    else if (oldtype == 5)
    {
        if (newtype == 1)
        {
            set_byte(&disk, position, (byte)get_float(disk, position));
        }
        else if (newtype == 2)
        {
            set_short(&disk, position, (short)get_float(disk, position));
        }
        else if (newtype == 3)
        {
            set_int(&disk, position, (int)get_float(disk, position));
        }
        else if (newtype == 4)
        {
            set_long(&disk, position, (long)get_float(disk, position));
        }
        else if (newtype == 6)
        {
            set_double(&disk, position, (double)get_float(disk, position));
        }
        else if (newtype == 7)
        {
            set_long_double(&disk, position, (long double)get_float(disk, position));
        }
    }
    else if (oldtype == 6)
    {
        if (newtype == 1)
        {
            set_byte(&disk, position, (byte)get_double(disk, position));
        }
        else if (newtype == 2)
        {
            set_short(&disk, position, (short)get_double(disk, position));
        }
        else if (newtype == 3)
        {
            set_int(&disk, position, (int)get_double(disk, position));
        }
        else if (newtype == 4)
        {
            set_long(&disk, position, (long)get_double(disk, position));
        }
        else if (newtype == 5)
        {
            set_float(&disk, position, (float)get_double(disk, position));
        }
        else if (newtype == 7)
        {
            set_long_double(&disk, position, (long double)get_double(disk, position));
        }
    }
    else if (oldtype == 7)
    {
        if (newtype == 1)
        {
            set_byte(&disk, position, (byte)get_long_double(disk, position));
        }
        else if (newtype == 2)
        {
            set_short(&disk, position, (short)get_long_double(disk, position));
        }
        else if (newtype == 3)
        {
            set_int(&disk, position, (int)get_long_double(disk, position));
        }
        else if (newtype == 4)
        {
            set_long(&disk, position, (long)get_long_double(disk, position));
        }
        else if (newtype == 5)
        {
            set_float(&disk, position, (float)get_long_double(disk, position));
        }
        else if (newtype == 6)
        {
            set_double(&disk, position, (double)get_long_double(disk, position));
        }
    }
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

// fb iiii iiii ?
Disk caller_if(Disk disk, int index)
{
    int size = get_ptr(disk, index + 1 + MEMSIZE);
    if(get_byte(disk, get_ptr(disk, index+1)) == 1)
    {
        _goto(&disk, index + 1 + MEMSIZE*2);
        return disk;
    }
    _goto(&disk, index + 1 + MEMSIZE*2 + size);
    return disk;
}


Disk caller_equal(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_ptr(disk, index + 1 + MEMSIZE*2); i++)
    {
        if (disk[get_ptr(disk, index + 1) + i] != disk[get_ptr(disk, index + 1 + MEMSIZE) + i])
        {
            _true = 0;
            break;
        }
    }
    disk[get_ptr(disk, index + 1 + MEMSIZE*3)] = _true;
    _goto(&disk, index + 1 + MEMSIZE*4);
    return disk;
}

Disk caller_not_equal(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_ptr(disk, index + 1 + MEMSIZE*2); i++)
    {
        if (disk[get_ptr(disk, index + 1) + i] == disk[get_ptr(disk, index + 1 + MEMSIZE) + i])
        {
            _true = 0;
            break;
        }
    }
    disk[get_ptr(disk, index + 1 + MEMSIZE*3)] = _true;
    _goto(&disk, index + 1 + MEMSIZE*4);
    return disk;
}

Disk caller_greater(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_ptr(disk, index + 1 + MEMSIZE*2); i++)
    {
        if (disk[get_ptr(disk, index + 1) + i] <= disk[get_ptr(disk, index + 1 + MEMSIZE) + i] && disk[get_ptr(disk, index + 1) + i] != 0)
        {
            _true = 0;
            break;
        }
    }
    disk[get_ptr(disk, index + 1 + MEMSIZE*3)] = _true;
    _goto(&disk, index + 1 + MEMSIZE*4);
    return disk;
}

Disk caller_less(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_ptr(disk, index + 1 + MEMSIZE*2); i++)
    {
        if (disk[get_ptr(disk, index + 1) + i] >= disk[get_ptr(disk, index + 1 + MEMSIZE) + i] && disk[get_ptr(disk, index + 1) + i] != 0)
        {
            _true = 0;
            break;
        }
    }
    disk[get_ptr(disk, index + 1 + MEMSIZE*3)] = _true;
    _goto(&disk, index + 1 + MEMSIZE*4);
    return disk;
}

Disk caller_less_or_equal(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_ptr(disk, index + 1 + MEMSIZE*2); i++)
    {
        if (disk[get_ptr(disk, index + 1) + i] > disk[get_ptr(disk, index + 1 + MEMSIZE) + i])
        {
            _true = 0;
            break;
        }
        {
            _true = 0;
            break;
        }
    }
    disk[get_ptr(disk, index + 1 + MEMSIZE*3)] = _true;
    _goto(&disk, index + 1 + MEMSIZE*4);
    return disk;
}

Disk caller_greater_or_equal(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_ptr(disk, index + 1 + MEMSIZE*2); i++)
    {
        if (disk[get_ptr(disk, index + 1) + i] < disk[get_ptr(disk, index + 1 + MEMSIZE) + i])
        {
            _true = 0;
            break;
        }
        {
            _true = 0;
            break;
        }
    }
    disk[get_ptr(disk, index + 1 + MEMSIZE*3)] = _true;
    _goto(&disk, index + 1 + MEMSIZE*4);
    return disk;
}

Disk caller_and(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_ptr(disk, index + 1 + MEMSIZE); i++)
    {
        if (disk[get_ptr(disk, index + 1) + i] == 0)
        {
            _true = 0;
            break;
        }
    }
    disk[get_ptr(disk, index + 1 + MEMSIZE*2)] = _true;
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_or(Disk disk, int index)
{
    byte _true = 0;
    for (int i = 0; i < get_ptr(disk, index + 1 + MEMSIZE); i++)
    {
        if (disk[get_ptr(disk, index + 1) + i] != 0)
        {
            _true = 1;
            break;
        }
    }
    disk[get_ptr(disk, index + 1 + MEMSIZE*2)] = _true;
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_print(Disk disk, int index)
{
    for (int i = 0; i < get_ptr(disk, index + 1 + MEMSIZE); i++)
    {
        printf("%c", disk[get_ptr(disk, index + 1) + i]);
    }
    printf("\n");
    //set disk index to next instruction
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_scan(Disk disk, int index)
{
    int size = get_ptr(disk, index + 1 + MEMSIZE);
    int _index = get_ptr(disk, index + 1);
    char *str = (char *)malloc(size * sizeof(char));
    scanf("%s", str);
    _set(&disk, _index, size, str);
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_goto(Disk disk, int index)
{
    set_int(&disk, 4, get_ptr(disk, index + 1));

    return disk;
}

Disk caller_add(Disk disk, int index)
{
    int _type = get_byte(disk, index + 1 + MEMSIZE*2);
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    switch (_type)
    {
    case 1:// byte
        disk[_index] += disk[_pointer];
        break;
    case 2:// short
        set_short(&disk, _index, get_short(disk, _index) + get_short(disk, _pointer));
        break;
    case 3:// int
        set_int(&disk, _index, get_int(disk, _index) + get_int(disk, _pointer));
        break;
    case 4:// long
        set_long(&disk, _index, get_long(disk, _index) + get_long(disk, _pointer));
        break;
    case 5:// float
        set_float(&disk, _index, get_float(disk, _index) + get_float(disk, _pointer));
        break;
    case 6:// double
        set_double(&disk, _index, get_double(disk, _index) + get_double(disk, _pointer));
        break;
    case 7:// long double
        set_long_double(&disk, _index, get_long_double(disk, _index) + get_long_double(disk, _pointer));
        break;
    default:
        break;
    }
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_sub(Disk disk, int index)
{
    int _type = get_byte(disk, index + 1 + MEMSIZE*2);
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    switch (_type)
    {
    case 1:// byte
        disk[_index] -= disk[_pointer];
        break;
    case 2:// short
        set_short(&disk, _index, get_short(disk, _index) - get_short(disk, _pointer));
        break;
    case 3:// int
        set_int(&disk, _index, get_int(disk, _index) - get_int(disk, _pointer));
        break;
    case 4:// long
        set_long(&disk, _index, get_long(disk, _index) - get_long(disk, _pointer));
        break;
    case 5:// float
        set_float(&disk, _index, get_float(disk, _index) - get_float(disk, _pointer));
        break;
    case 6:// double
        set_double(&disk, _index, get_double(disk, _index) - get_double(disk, _pointer));
        break;
    case 7:// long double
        set_long_double(&disk, _index, get_long_double(disk, _index) - get_long_double(disk, _pointer));
        break;
    default:
        break;
    }
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_mul(Disk disk, int index)
{
    int _type = get_byte(disk, index + 1 + MEMSIZE*2);
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    switch (_type)
    {
    case 1:// byte
        disk[_index] *= disk[_pointer];
        break;
    case 2:// short
        set_short(&disk, _index, get_short(disk, _index) * get_short(disk, _pointer));
        break;
    case 3:// int
        set_int(&disk, _index, get_int(disk, _index) * get_int(disk, _pointer));
        break;
    case 4:// long
        set_long(&disk, _index, get_long(disk, _index) * get_long(disk, _pointer));
        break;
    case 5:// float
        set_float(&disk, _index, get_float(disk, _index) * get_float(disk, _pointer));
        break;
    case 6:// double
        set_double(&disk, _index, get_double(disk, _index) * get_double(disk, _pointer));
        break;
    case 7:// long double
        set_long_double(&disk, _index, get_long_double(disk, _index) * get_long_double(disk, _pointer));
        break;
    default:
        break;
    }
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_div(Disk disk, int index)
{
    int _type = get_byte(disk, index + 1 + MEMSIZE*2);
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    switch (_type)
    {
    case 1:// byte
        disk[_index] /= disk[_pointer];
        break;
    case 2:// short
        set_short(&disk, _index, get_short(disk, _index) / get_short(disk, _pointer));
        break;
    case 3:// int
        set_int(&disk, _index, get_int(disk, _index) / get_int(disk, _pointer));
        break;
    case 4:// long
        set_long(&disk, _index, get_long(disk, _index) / get_long(disk, _pointer));
        break;
    case 5:// float
        set_float(&disk, _index, get_float(disk, _index) / get_float(disk, _pointer));
        break;
    case 6:// double
        set_double(&disk, _index, get_double(disk, _index) / get_double(disk, _pointer));
        break;
    case 7:// long double
        set_long_double(&disk, _index, get_long_double(disk, _index) / get_long_double(disk, _pointer));
        break;
    default:
        break;
    }
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_increment(Disk disk, int index)
{
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    int _index = get_ptr(disk, index + 1);
    switch (_type)
    {
    case 1:// byte
        disk[_index]++;
        break;
    case 2:// short
        set_short(&disk, _index, get_short(disk, _index) + 1);
        break;
    case 3:// int
        set_int(&disk, _index, get_int(disk, _index) +1);
        break;
    case 4:// long
        set_long(&disk, _index, get_long(disk, _index) + 1);
        break;
    case 5:// float
        set_float(&disk, _index, get_float(disk, _index) + 1);
        break;
    case 6:// double
        set_double(&disk, _index, get_double(disk, _index) + 1);
        break;
    case 7:// long double
        set_long_double(&disk, _index, get_long_double(disk, _index) + 1);
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_decrement(Disk disk, int index)
{
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    int _index = get_ptr(disk, index + 1);
    switch (_type)
    {
    case 1:// byte
        disk[_index]--;
        break;
    case 2:// short
        set_short(&disk, _index, get_short(disk, _index) - 1);
        break;
    case 3:// int
        set_int(&disk, _index, get_int(disk, _index) - 1);
        break;
    case 4:// long
        set_long(&disk, _index, get_long(disk, _index) - 1);
        break;
    case 5:// float
        set_float(&disk, _index, get_float(disk, _index) - 1);
        break;
    case 6:// double
        set_double(&disk, _index, get_double(disk, _index) - 1);
        break;
    case 7:// long double
        set_long_double(&disk, _index, get_long_double(disk, _index) - 1);
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_mod(Disk disk, int index)
{
    int _type = get_byte(disk, index + 1 + MEMSIZE*2);
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    switch (_type)
    {
    case 1:// byte
        disk[_index] %= disk[_pointer];
        break;
    case 2:// short
        set_short(&disk, _index, get_short(disk, _index) % get_short(disk, _pointer));
        break;
    case 3:// int
        set_int(&disk, _index, get_int(disk, _index) % get_int(disk, _pointer));
        break;
    case 4:// long
        set_long(&disk, _index, get_long(disk, _index) % get_long(disk, _pointer));
        break;
    case 5:// float
        set_float(&disk, _index, fmod(get_float(disk, _index), get_float(disk, _pointer)));
        break;
    case 6:// double
        set_double(&disk, _index, fmod(get_double(disk, _index), get_double(disk, _pointer)));
        break;
    case 7:// long double
        set_long_double(&disk, _index, fmod(get_long_double(disk, _index), get_long_double(disk, _pointer)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_pow(Disk disk, int index)
{
    int _type = get_byte(disk, index + 1 + MEMSIZE*2);
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    switch (_type)
    {
    case 1:// byte
        disk[_index] = pow(disk[_index], disk[_pointer]);
        break;
    case 2:// short
        set_short(&disk, _index, pow(get_short(disk, _index), get_short(disk, _pointer)));
        break;
    case 3:// int
        set_int(&disk, _index, pow(get_int(disk, _index), get_int(disk, _pointer)));
        break;
    case 4:// long
        set_long(&disk, _index, pow(get_long(disk, _index), get_long(disk, _pointer)));
        break;
    case 5:// float
        set_float(&disk, _index, pow(get_float(disk, _index), get_float(disk, _pointer)));
        break;
    case 6:// double
        set_double(&disk, _index, pow(get_double(disk, _index), get_double(disk, _pointer)));
        break;
    case 7:// long double
        set_long_double(&disk, _index, pow(get_long_double(disk, _index), get_long_double(disk, _pointer)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 1 + MEMSIZE*3);
    return disk;
}

Disk caller_sqrt(Disk disk, int index)
{
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    int _index = get_ptr(disk, index + 1);
    switch (_type)
    {
    case 1:// byte
        disk[_index] = sqrt(disk[_index]);
        break;
    case 2:// short
        set_short(&disk, _index, sqrt(get_short(disk, _index)));
        break;
    case 3:// int
        set_int(&disk, _index, sqrt(get_int(disk, _index)));
        break;
    case 4:// long
        set_long(&disk, _index, sqrt(get_long(disk, _index)));
        break;
    case 5:// float
        set_float(&disk, _index, sqrt(get_float(disk, _index)));
        break;
    case 6:// double
        set_double(&disk, _index, sqrt(get_double(disk, _index)));
        break;
    case 7:// long double
        set_long_double(&disk, _index, sqrt(get_long_double(disk, _index)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_abs(Disk disk, int index)
{
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    int _index = get_ptr(disk, index + 1);
    switch (_type)
    {
    case 1:// byte
        disk[_index] = abs(disk[_index]);
        break;
    case 2:// short
        set_short(&disk, _index, abs(get_short(disk, _index)));
        break;
    case 3:// int
        set_int(&disk, _index, abs(get_int(disk, _index)));
        break;
    case 4:// long
        set_long(&disk, _index, abs(get_long(disk, _index)));
        break;
    case 5:// float
        set_float(&disk, _index, abs(get_float(disk, _index)));
        break;
    case 6:// double
        set_double(&disk, _index, abs(get_double(disk, _index)));
        break;
    case 7:// long double
        set_long_double(&disk, _index, abs(get_long_double(disk, _index)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_min(Disk disk, int index)
{
    int _position1 = get_ptr(disk, index + 1);
    int _position2 = get_ptr(disk, index + 1 + MEMSIZE);
    int _type = get_byte(disk, index + 1 + MEMSIZE*2);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position1] = (disk[_position1] < disk[_position2]) ? disk[_position1] : disk[_position2];
        break;
    case 2:// short
        short s1 = get_short(disk, _position1);
        short s2 = get_short(disk, _position2);
        set_short(&disk, _position1, (s1 < s2) ? s1 : s2);
        break;
    case 3:// int
        int i1 = get_int(disk, _position1);
        int i2 = get_int(disk, _position2);
        set_int(&disk, _position1, (i1 < i2) ? i1 : i2);
        break;
    case 4:// long
        long l1 = get_long(disk, _position1);
        long l2 = get_long(disk, _position2);
        set_long(&disk, _position1, (l1 < l2) ? l1 : l2);
        break;
    case 5:// float
        float f1 = get_float(disk, _position1);
        float f2 = get_float(disk, _position2);
        set_float(&disk, _position1, (f1 < f2) ? f1 : f2);
        break;
    case 6:// double
        double d1 = get_double(disk, _position1);
        double d2 = get_double(disk, _position2);
        set_double(&disk, _position1, (d1 < d2) ? d1 : d2);
        break;
    case 7:// long double
        long double d3 = get_long_double(disk, _position1);
        long double d4 = get_long_double(disk, _position2);
        set_long_double(&disk, _position1, (d3 < d4) ? d3 : d4);
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE*2);
    return disk;
}

Disk caller_max(Disk disk, int index)
{
    int _position1 = get_ptr(disk, index + 1);
    int _position2 = get_ptr(disk, index + 1 + MEMSIZE);
    int _type = get_byte(disk, index + 1 + MEMSIZE*2);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position1] = (disk[_position1] > disk[_position2]) ? disk[_position1] : disk[_position2];
        break;
    case 2:// short
        short s1 = get_short(disk, _position1);
        short s2 = get_short(disk, _position2);
        set_short(&disk, _position1, (s1 > s2) ? s1 : s2);
        break;
    case 3:// int
        int i1 = get_int(disk, _position1);
        int i2 = get_int(disk, _position2);
        set_int(&disk, _position1, (i1 > i2) ? i1 : i2);
        break;
    case 4:// long
        long l1 = get_long(disk, _position1);
        long l2 = get_long(disk, _position2);
        set_long(&disk, _position1, (l1 > l2) ? l1 : l2);
        break;
    case 5:// float
        float f1 = get_float(disk, _position1);
        float f2 = get_float(disk, _position2);
        set_float(&disk, _position1, (f1 > f2) ? f1 : f2);
        break;
    case 6:// double
        double d1 = get_double(disk, _position1);
        double d2 = get_double(disk, _position2);
        set_double(&disk, _position1, (d1 > d2) ? d1 : d2);
        break;
    case 7:// long double
        long double d3 = get_long_double(disk, _position1);
        long double d4 = get_long_double(disk, _position2);
        set_long_double(&disk, _position1, (d3 > d4) ? d3 : d4);
        break;
    default:
        break;
    }
    _goto(&disk, index + 10);
    return disk;
}

Disk caller_random(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = rand() % 256;
        break;
    case 2:// short
        set_short(&disk, _position, rand() % 65536);
        break;
    case 3:// int
        set_int(&disk, _position, rand());
        break;
    case 4:// long
        set_long(&disk, _position, rand());
        break;
    case 5:// float
        set_float(&disk, _position, (float)rand() / (float)RAND_MAX);
        break;
    case 6:// double
        set_double(&disk, _position, (double)rand() / (double)RAND_MAX);
        break;
    case 7:// long double
        set_long_double(&disk, _position, (long double)rand() / (long double)RAND_MAX);
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_round(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, round(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, round(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, round(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_floor(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, floor(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, floor(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, floor(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_ceil(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, ceil(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, ceil(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, ceil(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_trunc(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, trunc(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, trunc(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, trunc(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_sin(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, sin(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, sin(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, sin(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_cos(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, cos(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, cos(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, cos(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_tan(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, tan(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, tan(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, tan(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_asin(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, asin(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, asin(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, asin(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_acos(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, acos(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, acos(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, acos(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_atan(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, atan(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, atan(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, atan(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_atan2(Disk disk, int index)
{
    int _position1 = get_ptr(disk, index + 1);
    int _position2 = get_ptr(disk, index + 1 + MEMSIZE);
    int _type = get_byte(disk, index + 1 + MEMSIZE*2);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position1, atan2(get_float(disk, _position1), get_float(disk, _position2)));
        break;
    case 6:// double
        set_double(&disk, _position1, atan2(get_double(disk, _position1), get_double(disk, _position2)));
        break;
    case 7:// long double
        set_long_double(&disk, _position1, atan2(get_long_double(disk, _position1), get_long_double(disk, _position2)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE*2);
    return disk;
}

Disk caller_sinh(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, sinh(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, sinh(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, sinh(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_cosh(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, cosh(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, cosh(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, cosh(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_tanh(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, tanh(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, tanh(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, tanh(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_exp(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, exp(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, exp(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, exp(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

// may not work
Disk caller_frexp(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _exp = get_ptr(disk, index + 1 + MEMSIZE);
    int _type = get_byte(disk, index + 1 + MEMSIZE*2);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, frexp(get_float(disk, _position), &_exp));
        break;
    case 6:// double
        set_double(&disk, _position, frexp(get_double(disk, _position), &_exp));
        break;
    case 7:// long double
        set_long_double(&disk, _position, frexp(get_long_double(disk, _position), &_exp));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE*2);
    return disk;
}

Disk caller_ldexp(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _exp = get_ptr(disk, index + 1 + MEMSIZE);
    int _type = get_byte(disk, index + 1 + MEMSIZE*2);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, ldexp(get_float(disk, _position), _exp));
        break;
    case 6:// double
        set_double(&disk, _position, ldexp(get_double(disk, _position), _exp));
        break;
    case 7:// long double
        set_long_double(&disk, _position, ldexp(get_long_double(disk, _position), _exp));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE*2);
    return disk;
}

Disk caller_log(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _type = get_byte(disk, index + 1 + MEMSIZE);
    
    switch (_type)
    {
    case 5:// float
        set_float(&disk, _position, log(get_float(disk, _position)));
        break;
    case 6:// double
        set_double(&disk, _position, log(get_double(disk, _position)));
        break;
    case 7:// long double
        set_long_double(&disk, _position, log(get_long_double(disk, _position)));
        break;
    default:
        break;
    }
    _goto(&disk, index + 2 + MEMSIZE);
    return disk;
}

Disk caller_load(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _size = get_ptr(disk, index + 1 + MEMSIZE);
    int _filename_size = get_ptr(disk, index + 1 + MEMSIZE*2);
    char _filename[_filename_size];
    for (int i = 0; i < _filename_size; i++)
    {
        _filename[i] = disk[index + 1 + MEMSIZE*3 + i];
    }
    Disk _data = disk_read(_filename);
    _set(&disk, _position, _size, _data);
    _goto(&disk, index + 1 + MEMSIZE*3 + _filename_size);
    return disk;
}

Disk caller_save(Disk disk, int index)
{
    int _position = get_ptr(disk, index + 1);
    int _size = get_ptr(disk, index + 1 + MEMSIZE);
    int _filename_size = get_ptr(disk, index + 1 + MEMSIZE*2);
    Disk _filename = (Disk)malloc(_filename_size);
    Disk _data = (Disk)malloc(_size);
    for (int i = 0; i < _filename_size; i++)
    {
        _filename[i] = disk[index + 1 + MEMSIZE*3 + i];
    }
    for (int i = 0; i < _size; i++)
    {
        _data[i] = disk[_position + i];
    }
    disk_write(_filename, _data);
    free(_filename);
    free(_data);
    _goto(&disk, index + 1 + MEMSIZE*3 + _filename_size);
    return disk;
}

// functions
Disk (*functions[])(Disk, int) = 
{
    caller_state,
    caller_set,
    caller_insert,
    caller_delete,
    caller_move,
    caller_swap,
    caller_shift,
    caller_randomize,
    caller_copy,
    caller_fill,
    caller_reverse,
    caller_sort,
    caller_find,
    caller_convert,
    caller_if,
    caller_equal,
    caller_not_equal,
    caller_greater,
    caller_less,
    caller_less_or_equal,
    caller_greater_or_equal,
    caller_and,
    caller_or,
    caller_print,
    caller_scan,
    caller_goto,
    caller_add,
    caller_sub,
    caller_mul,
    caller_div,
    caller_increment,
    caller_decrement,
    caller_mod,
    caller_pow,
    caller_sqrt,
    caller_abs,
    caller_min,
    caller_max,
    caller_random,
    caller_round,
    caller_floor,
    caller_ceil,
    caller_trunc,
    caller_sin,
    caller_cos,
    caller_tan,
    caller_asin,
    caller_acos,
    caller_atan,
    caller_atan2,
    caller_sinh,
    caller_cosh,
    caller_tanh,
    caller_exp,
    caller_frexp,
    caller_ldexp,
    caller_log,
    NULL,
    NULL,
    caller_load,
    caller_save,
};






// runner
// runner
// runner






// runs the byte code
Disk _run(Disk disk)
{
    int i = get_ptr(disk, 4); 
    while (disk[0] != 0)
    {
        i = get_ptr(disk, 4);
        disk = functions[disk[i]](disk, i);
    }
    return disk;
}