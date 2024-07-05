#include "src/brimg.h"

// size of the pointer in bytes
// 1,2 or 4 bytes, 1 is 256bytes, 2 is 64kb, 4 is 4gb
#define MEMSIZE 4

// StandardFunctions C Api
// StandardFunctions C Api
// StandardFunctions C Api







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



// sets 
// sets 
// sets 



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



void set_byte(Disk *disk, int index, byte data)
{
    byte _data[1] = {data};
    _set(disk, index, 1, _data);
}

void set_string(Disk *disk, int index, byte *str, int size)
{
    _set(disk, index, strlen((char*)str), str);
}


typedef union {
    int i;
    byte b[4];
} IntUnion;

typedef union {
    short s;
    byte b[2];
} ShortUnion;

void set_ptr(Disk *disk, int index, int data)
{
    int size = MEMSIZE;
    if (size == 1)
    {
        set_byte(disk, index, data);
    }
    else if (size == 2)
    {
        ShortUnion u;
        u.s = data;
        set_byte(disk, index, u.b[0]);
        set_byte(disk, index + 1, u.b[1]);
    }
    else if (size == 4)
    {
        IntUnion u;
        u.i = data;
        set_byte(disk, index, u.b[0]);
        set_byte(disk, index + 1, u.b[1]);
        set_byte(disk, index + 2, u.b[2]);
        set_byte(disk, index + 3, u.b[3]);
    }
}





void _insert(Disk *disk, int index, byte *str)
{
    int size = strlen((char*)str);
    int disksize = strlen((char*)*disk);
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
    int disksize = strlen((char*)*disk);
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
    int disksize = strlen((char*)*disk);
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
    int disksize = strlen((char*)*disk);
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
    int disksize = strlen((char*)*disk);
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
    srand(time(0));

    for (int i = 0; i < size; i++)
    {
        (*disk)[index + i] = rand() % 256;
    }
}

void _copy(Disk *disk, int index, int destiny, int size)
{
    int disksize = strlen((char*)*disk);
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
    int disksize = strlen((char*)*disk);
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
    int disksize = strlen((char*)*disk);
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
    set_ptr(disk, 4, position);
}







// DiskManagement brimg_functions
// DiskManagement brimg_functions
// DiskManagement brimg_functions







/*byte *disk_read(char *filename)
{
    File *file = fopen(filename, "r");
    if (file == 0)
    {
        printf("Error: File not found\n");
        return 0;
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
    if (file == 0)
    {
        printf("Error: File not found\n");
        exit(1);
    }

    fwrite(data, strlen((char*)data), 1, file);
    fclose(file);
}*/











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
    
    int disksize = strlen((char*)disk);
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
    int disksize = strlen((char*)disk);
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
    int disksize = strlen((char*)disk);
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
    int disksize = strlen((char*)disk);
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
    int disksize = strlen((char*)disk);
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
    srand(time(0));
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
    int disksize = strlen((char*)disk);
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
    int disksize = strlen((char*)disk);
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
    int disksize = strlen((char*)disk);
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
    int disksize = strlen((char*)disk);
    int position = get_ptr(disk, index + 1);
    int size = get_ptr(disk, index + 1 + MEMSIZE);
    int result = get_ptr(disk, index + 1 + MEMSIZE*2);
    int patternsize = get_ptr(disk, index + 1 + MEMSIZE*3);
    byte *pattern = get_bytes(disk, index + 1 + MEMSIZE*4, patternsize);
    char *point = strstr((char*)disk + position, (char*)pattern);
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
*/

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
    printf("less\n");
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
    _set(&disk, _index, size, (byte*)str);
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_goto(Disk disk, int index)
{
    set_ptr(&disk, 4, get_ptr(disk, index + 1));

    return disk;
}

Disk caller_add(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    set_ptr(&disk, _index, get_ptr(disk, _index) + get_ptr(disk, _pointer));
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_sub(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    set_ptr(&disk, _index, get_ptr(disk, _index) - get_ptr(disk, _pointer));
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_mul(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    set_ptr(&disk, _index, get_ptr(disk, _index) * get_ptr(disk, _pointer));
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_div(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    set_ptr(&disk, _index, get_ptr(disk, _index) / get_ptr(disk, _pointer));
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_increment(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    set_ptr(&disk, _index, get_ptr(disk, _index) + 1);
    _goto(&disk, index + 2);
    return disk;
}

Disk caller_decrement(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    set_ptr(&disk, _index, get_ptr(disk, _index) - 1);
    _goto(&disk, index + 2);
    return disk;
}

Disk caller_mod(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    set_ptr(&disk, _index, get_ptr(disk, _index) % get_ptr(disk, _pointer));
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_pow(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    set_ptr(&disk, _index, pow(get_ptr(disk, _index), get_ptr(disk, _pointer)));
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_sqrt(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    set_ptr(&disk, _index, sqrt(get_ptr(disk, _index)));
    _goto(&disk, index + 2);
    return disk;
}

Disk caller_abs(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    set_ptr(&disk, _index, abs(get_ptr(disk, _index)));
    _goto(&disk, index + 2);
    return disk;
}

Disk caller_min(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    if (get_ptr(disk, _index) < get_ptr(disk, _pointer))
    {
        set_ptr(&disk, _index, get_ptr(disk, _index));
    }
    else
    {
        set_ptr(&disk, _index, get_ptr(disk, _pointer));
    }
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_max(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    int _pointer = get_ptr(disk, index + 1 + MEMSIZE);
    if (get_ptr(disk, _index) > get_ptr(disk, _pointer))
    {
        set_ptr(&disk, _index, get_ptr(disk, _index));
    }
    else
    {
        set_ptr(&disk, _index, get_ptr(disk, _pointer));
    }
    _goto(&disk, index + 1 + MEMSIZE*2);
    return disk;
}

Disk caller_random(Disk disk, int index)
{
    int _index = get_ptr(disk, index + 1);
    set_ptr(&disk, _index, rand() % 256);
    _goto(&disk, index + 2);
    return disk;
}


/*Disk caller_load(Disk disk, int index)
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
}*/

// brimg_functions
Disk (*brimg_functions[])(Disk, int) = 
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
    0,
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
    0,
    0,
    //caller_load,
    //caller_save,
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
        disk = brimg_functions[disk[i]](disk, i);
    }
    return disk;
}

#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#define BUZZER D3


#define BTN_1 D8
#define BTN_2 D7
#define BTN_3 D4
#define BTN_4 D2
#define BTN_5 D1

#define PROGS 2

#define byte unsigned char

#include "src/font.h"
SSD1306Wire display(0x3c, D5, D6); 

bool redraw = true;
byte cursor_x[PROGS];
byte cursor_y[PROGS];
byte str[PROGS][96];

byte current_prog = 0;
short global_delay = 50;

void setup()
{
    display.init();
    display.setContrast(255);

    display.flipScreenVertically();

    pinMode(BTN_1, INPUT);
    pinMode(BTN_2, INPUT);
    pinMode(BTN_3, INPUT);
    pinMode(BTN_4, INPUT);
    pinMode(BTN_5, INPUT);
    pinMode(BUZZER, OUTPUT);

    display.setFont(Font6x8);
    for (byte i = 0; i < PROGS; i++)
    {
        for (byte j = 0; j < 96; j++)
        {
            
            str[i][j] = 0;
        }

        cursor_x[i] = 0;
        cursor_y[i] = 0;
    }

}

void drawFooter(char* text)
{
    display.setColor(WHITE); // alternate colors
    display.drawLine(0, 51, display.getWidth() - 1, 51);
    display.drawString(0, 53, text);
}

void drawBoard()
{
    char buffer[16];
    for (byte y = 0; y < 6; y++)
    {
        for (byte x = 0; x < 16; x++)
        {
            display.drawStringf((x * 8)+1, y * 8, buffer, "%c", str[current_prog][y * 16 + x]);
        }
    }
    display.setColor(WHITE); // alternate colors
    display.fillRect(cursor_x[current_prog]-1, cursor_y[current_prog]+1, 8, 8);
    display.setColor(BLACK); // alternate colors
    display.drawStringf(cursor_x[current_prog], cursor_y[current_prog], buffer, "%c", str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8]);        
}


bool moveCursor()
{
    if (digitalRead(BTN_1) == HIGH)
    {
        if (digitalRead(BTN_2) == HIGH)
        {
            if (digitalRead(BTN_5) == HIGH)
            {
                if (cursor_y[current_prog] < 40)
                {
                    cursor_y[current_prog] += 8;
                }
                else if (cursor_y[current_prog] == 40)
                {
                    cursor_y[current_prog] = 0;
                }
                redraw = true;
                return true;
            }
            else if (digitalRead(BTN_4) == HIGH)
            {
                if (cursor_y[current_prog] > 0)
                {
                    cursor_y[current_prog] -= 8;
                }
                else if (cursor_y[current_prog] == 0)
                {
                    cursor_y[current_prog] = 40;
                }
                redraw = true;
                return true;
            }
        }
        else if (digitalRead(BTN_5) == HIGH)
        {
            if (cursor_x[current_prog] < 120)
            {
                cursor_x[current_prog] += 8;
            }
            else
            {
                if (cursor_y[current_prog] < 40)
                {
                    cursor_y[current_prog] += 8;
                }
                else if (cursor_y[current_prog] == 40)
                {
                    cursor_y[current_prog] = 0;
                }
                cursor_x[current_prog] = 0;
            }
            redraw = true;
            return true;
        }
        else if (digitalRead(BTN_4) == HIGH)
        {
            if (cursor_x[current_prog] > 0)
            {
                cursor_x[current_prog] -= 8;
            }
            else
            {
                if (cursor_y[current_prog] > 0)
                {
                    cursor_y[current_prog] -= 8;
                }
                else if (cursor_y[current_prog] == 0)
                {
                    cursor_y[current_prog] = 40;
                }
                cursor_x[current_prog] = 120;
            }
            redraw = true;
            return true;
        }
    }
    return false;
}

bool progChange()
{
    if (digitalRead(BTN_3) == HIGH)
    {
        if (digitalRead(BTN_5) == HIGH)
        {
            if (current_prog < PROGS - 1)
            {
                current_prog++;
            }
            else
            {
                current_prog = 0;
            }
            redraw = true;
            return true;
        }
        else if (digitalRead(BTN_4) == HIGH)
        {
            if (current_prog > 0)
            {
                current_prog--;
            }
            else
            {
                current_prog = PROGS - 1;
            }
            redraw = true;
            return true;
        }
    }
    return false;
}

void editProg()
{
    if (redraw)
    {
        display.clear();
        char buffer[16];
        drawBoard();
        sprintf(buffer, "edit %i", str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8]);
        drawFooter(buffer);
        display.display();
        redraw = false;
    }
    
    if (moveCursor()) {}
    else if (progChange()) {}
    else if (digitalRead(BTN_5) == HIGH)
    {
        if (str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8] < 255)
            str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8]++;
        else
            str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8] = 0;
        redraw = true;
    }
    else if (digitalRead(BTN_4) == HIGH)
    {
        if (str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8] > 0)
            str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8]--;
        else
            str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8] = 255;
        redraw = true;
    }
}

void cmdRun()
{
    byte cmd[str[current_prog][0]];
    for (byte i = 0; i < str[current_prog][0]; i++)
    {
        cmd[i] = str[current_prog][i + 1];
    }
    if (cmd[1] == 1)
    {
        int index = get_int((Disk)cmd, 2);
        int size = get_int((Disk)cmd, 6);
        for (byte i = 0; i < size; i++)
        {
            str[0][i] = get_byte((Disk)cmd, 10 + i);
        }
    }
}

void cmdProg()
{
    if (redraw)
    {
        display.clear();
        char buffer[16];
        drawBoard();
        sprintf(buffer, "cmd %i", str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8]);
        drawFooter(buffer);
        display.display();
        redraw = false;
    }
    
    if (moveCursor()) {}
    else if (progChange()) {}
    else if (digitalRead(BTN_5) == HIGH)
    {
        if (str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8] < 255)
            str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8]++;
        else
            str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8] = 0;
        redraw = true;
    }
    else if (digitalRead(BTN_4) == HIGH)
    {
        if (str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8] > 0)
            str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8]--;
        else
            str[current_prog][cursor_y[current_prog] / 8 * 16 + cursor_x[current_prog] / 8] = 255;
        redraw = true;
    }

    display.clear();
}

// func pointer array
void (*programs[PROGS])() = 
{
    editProg, 
    cmdProg
};

void loop()
{
    programs[current_prog]();
    delay(global_delay);
}
