#include "brimg.h"



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
//find (position, size, patternsize, pattern, result):
void _find(Disk *disk, int index, int size, byte* data, int result)
{
    int disksize = strlen(*disk);
    int position = get_int(*disk, index + 1);
    int patternsize = get_int(*disk, index + 9);
    byte *pattern = get_bytes(*disk, index + 13, patternsize);
    char *point = strstr(*disk + position, pattern);
    position = (int*)(point) - (int*)(*disk);
    if (position < 0 || position >= disksize || position + size > disksize)
    {
        printf("Error: Index out of bounds\n");
    }
    else
    {
        set_int(disk, result, position);
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
    printf("set\n");
    int size = get_int(disk, index + 5);
    byte *data = get_bytes(disk, index + 9, size);
    int _index = get_int(disk, index + 1);
    for (int i = 0; i < size; i++)
    {
        disk[_index + i] = data[i];
    }
    _goto(&disk, index + 9 + size);
    return disk;
}

Disk caller_insert(Disk disk, int index)
{
    int size = get_int(disk, index + 5);
    byte *str = get_bytes(disk, index + 9, size);
    
    int disksize = strlen(disk);
    int _index = get_int(disk, index + 1);
    disk = (Disk)realloc(disk, (disksize + size) * sizeof(Disk));
    
    for (int i = disksize - 1; i >= _index; i--)
    {
        disk[i + size] = disk[i];
    }
    for (int i = 0; i < size; i++)
    {
        disk[_index + i] = str[i];
    }
    
    _goto(&disk, index + 9 + size);
    return disk;
}

Disk caller_delete(Disk disk, int index)
{
    int disksize = strlen(disk);
    int size = get_int(disk, index + 5);
    int _index = get_int(disk, index + 1);
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
    int size = get_int(disk, index + 9);
    int origin = get_int(disk, index + 1);
    int destiny = get_int(disk, index + 5);

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
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_swap(Disk disk, int index)
{
    int disksize = strlen(disk);
    int index1 = get_int(disk, index + 1);
    int index2 = get_int(disk, index + 5);
    int size = get_int(disk, index + 9);
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
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_shift(Disk disk, int index)
{
    int size = get_int(disk, index + 5);
    int _shift = get_int(disk, index + 9);
    int _index = get_int(disk, index + 1);
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
    
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_random(Disk disk, int index)
{
    int _index = get_int(disk, index + 1);
    srand(time(NULL));
    int size = get_int(disk, _index + 5);
    for (int i = 0; i < size; i++)
    {
        disk[_index + i] = rand() % 256;
    }
    _goto(&disk, _index + 9);
    return disk;
}

Disk caller_copy(Disk disk, int index)
{
    int disksize = strlen(disk);
    int size = get_int(disk, index + 9);
    int _index = get_int(disk, index + 1);
    int destiny = get_int(disk, index + 5);
    if (_index < 0 || destiny < 0 || _index >= disksize || destiny >= disksize || _index + size > disksize || destiny + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        exit(1);
    }
    for (int i = 0; i < size; i++)
    {
        disk[destiny + i] = disk[_index + i];
    }
    _goto(&disk, _index + 13);
    return disk;
}

Disk caller_fill(Disk disk, int index)
{
    int size = get_int(disk, index + 5);
    byte data = get_byte(disk, index + 9);
    int _index = get_int(disk, index + 1);
    for (int i = 0; i < size; i++)
    {
        disk[_index + i] = data;
    }
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_reverse(Disk disk, int index)
{
    int disksize = strlen(disk);
    int size = get_int(disk, index + 5);
    int _index = get_int(disk, index + 1);

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
    _goto(&disk, index + 9);
    return disk;
}

Disk caller_sort(Disk disk, int index)
{
    int disksize = strlen(disk);
    int size = get_int(disk, index + 5);
    int _index = get_int(disk, index + 1);

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
    _goto(&disk, index + 9);
    return disk;
}

//find (position, size, patternsize, pattern, result)
Disk caller_find(Disk disk, int index)
{
    int disksize = strlen(disk);
    int position = get_int(disk, index + 1);
    int size = get_int(disk, index + 5);
    int patternsize = get_int(disk, index + 9);
    byte *pattern = get_bytes(disk, index + 13, patternsize);
    int result = get_int(disk, index + 13 + patternsize);
    char *point = strstr(disk + position, pattern);
    position = (int*)(point) - (int*)(disk);
    if (position < 0 || position >= disksize || position + size > disksize)
    {
        printf("Error: Index out of bounds\n");
    }
    else
    {
        set_int(&disk, result, position);
    }
    _goto(&disk, index + 13 + patternsize + 4);
    return disk;
}

// fbiiii?
Disk caller_if(Disk disk, int index)
{
    int size = get_int(disk, index + 5);
    if(get_byte(disk, get_int(disk, index+1)) == 1)
    {
        _goto(&disk, index + 9);
        return disk;
    }
    _goto(&disk, index + 9 + size);
    return disk;
}

// f b iiii iiii ? ?
/*Disk caller_ifelse(Disk disk, int index)
{
    int sizeif = get_int(disk, index + 2);
    int sizeelse = get_int(disk, index + 6);
    if(get_byte(disk, index+1) == 0)
    {
        byte *data = get_bytes(disk, index + 10, sizeif);
        _run(data);
    }
    else
    {
        byte *data = get_bytes(disk, index + 10 + sizeif, sizeelse);
        _run(data);
    }
    return disk;
}*/

Disk caller_equal(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_int(disk, index + 9); i++)
    {
        if (disk[get_int(disk, index + 1) + i] != disk[get_int(disk, index + 5) + i])
        {
            _true = 0;
            break;
        }
    }
    disk[get_int(disk, index + 13)] = _true;
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_not_equal(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_int(disk, index + 9); i++)
    {
        if (disk[get_int(disk, index + 1) + i] == disk[get_int(disk, index + 5) + i])
        {
            _true = 0;
            break;
        }
    }
    disk[get_int(disk, index + 13)] = _true;
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_greater(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_int(disk, index + 9); i++)
    {
        if (disk[get_int(disk, index + 1) + i] <= disk[get_int(disk, index + 5) + i] && disk[get_int(disk, index + 1) + i] != 0)
        {
            _true = 0;
            break;
        }
    }
    disk[get_int(disk, index + 13)] = _true;
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_less(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_int(disk, index + 9); i++)
    {
        //printf("%d %d\n", disk[get_int(disk, index + 1) + i], disk[get_int(disk, index + 5) + i]);
        if (disk[get_int(disk, index + 1) + i] >= disk[get_int(disk, index + 5) + i] && disk[get_int(disk, index + 1) + i] != 0)
        {
            _true = 0;
            break;
        }
    }
    disk[get_int(disk, index + 13)] = _true;
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_less_or_equal(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_int(disk, index + 9); i++)
    {
        if (disk[get_int(disk, index + 1) + i] > disk[get_int(disk, index + 5) + i])
        {
            _true = 0;
            break;
        }
        {
            _true = 0;
            break;
        }
    }
    disk[get_int(disk, index + 13)] = _true;
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_greater_or_equal(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_int(disk, index + 9); i++)
    {
        if (disk[get_int(disk, index + 1) + i] < disk[get_int(disk, index + 5) + i])
        {
            _true = 0;
            break;
        }
        {
            _true = 0;
            break;
        }
    }
    disk[get_int(disk, index + 13)] = _true;
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_and(Disk disk, int index)
{
    byte _true = 1;
    for (int i = 0; i < get_int(disk, index + 5); i++)
    {
        if (disk[get_int(disk, index + 1) + i] == 0)
        {
            _true = 0;
            break;
        }
    }
    disk[get_int(disk, index + 9)] = _true;
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_or(Disk disk, int index)
{
    byte _true = 0;
    for (int i = 0; i < get_int(disk, index + 5); i++)
    {
        if (disk[get_int(disk, index + 1) + i] != 0)
        {
            _true = 1;
            break;
        }
    }
    disk[get_int(disk, index + 9)] = _true;
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_print(Disk disk, int index)
{
    for (int i = 0; i < get_int(disk, index + 5); i++)
    {
        printf("%c", disk[get_int(disk, index + 1) + i]);
    }
    printf("\n");
    //set disk index to next instruction
    _goto(&disk, index + 9);
    return disk;
}

Disk caller_goto(Disk disk, int index)
{
    set_int(&disk, 4, get_int(disk, index + 1));

    return disk;
}

Disk caller_add(Disk disk, int index)
{
    int _type = get_byte(disk, index + 9);
    int _index = get_int(disk, index + 1);
    int _pointer = get_int(disk, index + 5);
    switch (_type)
    {
    case 1:// byte
        disk[_index] += disk[_pointer];
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _index);
        short s2 = get_short(disk, _pointer);
        u.s = s1 + s2;
        set_short(&disk, _index, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _index);
        int i2 = get_int(disk, _pointer);
        u2.i = i1 + i2;
        set_int(&disk, _index, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _index);
        long l2 = get_long(disk, _pointer);
        u3.l = l1 + l2;
        set_long(&disk, _index, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _index);
        float f2 = get_float(disk, _pointer);
        u4.f = f1 + f2;
        set_float(&disk, _index, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _index);
        double d2 = get_double(disk, _pointer);
        u5.d = d1 + d2;
        set_double(&disk, _index, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _index);
        long double d4 = get_long_double(disk, _pointer);
        u6.d = d3 + d4;
        set_long_double(&disk, _index, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_sub(Disk disk, int index)
{
    int _type = get_byte(disk, index + 9);
    int _index = get_int(disk, index + 1);
    int _pointer = get_int(disk, index + 5);
    switch (_type)
    {
    case 1:// byte
        disk[_index] -= disk[_pointer];
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _index);
        short s2 = get_short(disk, _pointer);
        u.s = s1 - s2;
        set_short(&disk, _index, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _index);
        int i2 = get_int(disk, _pointer);
        u2.i = i1 - i2;
        set_int(&disk, _index, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _index);
        long l2 = get_long(disk, _pointer);
        u3.l = l1 - l2;
        set_long(&disk, _index, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _index);
        float f2 = get_float(disk, _pointer);
        u4.f = f1 - f2;
        set_float(&disk, _index, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _index);
        double d2 = get_double(disk, _pointer);
        u5.d = d1 - d2;
        set_double(&disk, _index, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _index);
        long double d4 = get_long_double(disk, _pointer);
        u6.d = d3 - d4;
        set_long_double(&disk, _index, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_mul(Disk disk, int index)
{
    int _type = get_byte(disk, index + 9);
    int _index = get_int(disk, index + 1);
    int _pointer = get_int(disk, index + 5);
    switch (_type)
    {
    case 1:// byte
        disk[_index] *= disk[_pointer];
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _index);
        short s2 = get_short(disk, _pointer);
        u.s = s1 * s2;
        set_short(&disk, _index, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _index);
        int i2 = get_int(disk, _pointer);
        u2.i = i1 * i2;
        set_int(&disk, _index, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _index);
        long l2 = get_long(disk, _pointer);
        u3.l = l1 * l2;
        set_long(&disk, _index, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _index);
        float f2 = get_float(disk, _pointer);
        u4.f = f1 * f2;
        set_float(&disk, _index, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _index);
        double d2 = get_double(disk, _pointer);
        u5.d = d1 * d2;
        set_double(&disk, _index, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _index);
        long double d4 = get_long_double(disk, _pointer);
        u6.d = d3 * d4;
        set_long_double(&disk, _index, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_div(Disk disk, int index)
{
    int _type = get_byte(disk, index + 9);
    int _index = get_int(disk, index + 1);
    int _pointer = get_int(disk, index + 5);
    switch (_type)
    {
    case 1:// byte
        disk[_index] /= disk[_pointer];
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _index);
        short s2 = get_short(disk, _pointer);
        u.s = s1 / s2;
        set_short(&disk, _index, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _index);
        int i2 = get_int(disk, _pointer);
        u2.i = i1 / i2;
        set_int(&disk, _index, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _index);
        long l2 = get_long(disk, _pointer);
        u3.l = l1 / l2;
        set_long(&disk, _index, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _index);
        float f2 = get_float(disk, _pointer);
        u4.f = f1 / f2;
        set_float(&disk, _index, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _index);
        double d2 = get_double(disk, _pointer);
        u5.d = d1 / d2;
        set_double(&disk, _index, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _index);
        long double d4 = get_long_double(disk, _pointer);
        u6.d = d3 / d4;
        set_long_double(&disk, _index, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_increment(Disk disk, int index)
{
    int _type = get_byte(disk, index + 5);
    int _index = get_int(disk, index + 1);
    switch (_type)
    {
    case 1:// byte
        disk[_index]++;
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _index);
        u.s = s1 + 1;
        set_short(&disk, _index, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _index);
        u2.i = i1 + 1;
        set_int(&disk, _index, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _index);
        u3.l = l1 + 1;
        set_long(&disk, _index, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _index);
        u4.f = f1 + 1;
        set_float(&disk, _index, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _index);
        u5.d = d1 + 1;
        set_double(&disk, _index, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _index);
        u6.d = d3 + 1;
        set_long_double(&disk, _index, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_decrement(Disk disk, int index)
{
    int _type = get_byte(disk, index + 5);
    int _index = get_int(disk, index + 1);
    switch (_type)
    {
    case 1:// byte
        disk[_index]--;
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _index);
        u.s = s1 - 1;
        set_short(&disk, _index, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _index);
        u2.i = i1 - 1;
        set_int(&disk, _index, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _index);
        u3.l = l1 - 1;
        set_long(&disk, _index, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _index);
        u4.f = f1 - 1;
        set_float(&disk, _index, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _index);
        u5.d = d1 - 1;
        set_double(&disk, _index, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _index);
        u6.d = d3 - 1;
        set_long_double(&disk, _index, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
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
    caller_random,
    caller_copy,
    caller_fill,
    caller_reverse,
    caller_sort,
    caller_find,
    caller_if,
    NULL,//caller_ifelse removed
    caller_equal,
    caller_not_equal,
    caller_greater,
    caller_less,
    caller_less_or_equal,
    caller_greater_or_equal,
    caller_and,
    caller_or,
    caller_print,
    caller_goto,
    NULL,//caller_while removed
    caller_add,
    caller_sub,
    caller_mul,
    caller_div,
    caller_increment,
    caller_decrement,
};






// runner
// runner
// runner






// runs the byte code
Disk _run(Disk disk)
{
    int i = get_int(disk, 4); 
    while (disk[0] != 0)
    {
        i = get_int(disk, 4);
        printf("current index: %d\n", i);
        disk = functions[disk[i]](disk, i);
    }
    return disk;
}