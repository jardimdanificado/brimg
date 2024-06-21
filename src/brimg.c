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

Disk caller_randomize(Disk disk, int index)
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

//find (position, size, result, patternsize, pattern)
Disk caller_find(Disk disk, int index)
{
    int disksize = strlen(disk);
    int position = get_int(disk, index + 1);
    int size = get_int(disk, index + 5);
    int result = get_int(disk, index + 9);
    int patternsize = get_int(disk, index + 13);
    byte *pattern = get_bytes(disk, index + 17, patternsize);
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
    _goto(&disk, index + 17 + patternsize);
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

Disk caller_scan(Disk disk, int index)
{
    int size = get_int(disk, index + 5);
    int _index = get_int(disk, index + 1);
    char *str = (char *)malloc(size * sizeof(char));
    scanf("%s", str);
    _set(&disk, _index, size, str);
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

Disk caller_mod(Disk disk, int index)
{
    int _position1 = get_int(disk, index + 1);
    int _position2 = get_int(disk, index + 5);
    int _type = get_byte(disk, index + 9);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position1] %= disk[_position2];
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position1);
        short s2 = get_short(disk, _position2);
        u.s = s1 % s2;
        set_short(&disk, _position1, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position1);
        int i2 = get_int(disk, _position2);
        u2.i = i1 % i2;
        set_int(&disk, _position1, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position1);
        long l2 = get_long(disk, _position2);
        u3.l = l1 % l2;
        set_long(&disk, _position1, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position1);
        float f2 = get_float(disk, _position2);
        u4.f = f1 - (int)(f1 / f2) * f2;
        set_float(&disk, _position1, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position1);
        double d2 = get_double(disk, _position2);
        u5.d = d1 - (int)(d1 / d2) * d2;
        set_double(&disk, _position1, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position1);
        long double d4 = get_long_double(disk, _position2);
        u6.d = d3 - (int)(d3 / d4) * d4;
        set_long_double(&disk, _position1, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 10);
    return disk;
}

Disk caller_pow(Disk disk, int index)
{
    int _position1 = get_int(disk, index + 1);
    int _position2 = get_int(disk, index + 5);
    int _type = get_byte(disk, index + 9);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position1] = pow(disk[_position1], disk[_position2]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position1);
        short s2 = get_short(disk, _position2);
        u.s = pow(s1, s2);
        set_short(&disk, _position1, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position1);
        int i2 = get_int(disk, _position2);
        u2.i = pow(i1, i2);
        set_int(&disk, _position1, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position1);
        long l2 = get_long(disk, _position2);
        u3.l = pow(l1, l2);
        set_long(&disk, _position1, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position1);
        float f2 = get_float(disk, _position2);
        u4.f = pow(f1, f2);
        set_float(&disk, _position1, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position1);
        double d2 = get_double(disk, _position2);
        u5.d = pow(d1, d2);
        set_double(&disk, _position1, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position1);
        long double d4 = get_long_double(disk, _position2);
        u6.d = pow(d3, d4);
        set_long_double(&disk, _position1, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 10);
    return disk;
}

Disk caller_sqrt(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = sqrt(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = sqrt(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = sqrt(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = sqrt(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = sqrt(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = sqrt(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = sqrt(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_abs(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = abs(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = abs(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = abs(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = abs(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = abs(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = abs(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = abs(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_min(Disk disk, int index)
{
    int _position1 = get_int(disk, index + 1);
    int _position2 = get_int(disk, index + 5);
    int _type = get_byte(disk, index + 9);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position1] = (disk[_position1] < disk[_position2]) ? disk[_position1] : disk[_position2];
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position1);
        short s2 = get_short(disk, _position2);
        u.s = (s1 < s2) ? s1 : s2;
        set_short(&disk, _position1, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position1);
        int i2 = get_int(disk, _position2);
        u2.i = (i1 < i2) ? i1 : i2;
        set_int(&disk, _position1, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position1);
        long l2 = get_long(disk, _position2);
        u3.l = (l1 < l2) ? l1 : l2;
        set_long(&disk, _position1, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position1);
        float f2 = get_float(disk, _position2);
        u4.f = (f1 < f2) ? f1 : f2;
        set_float(&disk, _position1, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position1);
        double d2 = get_double(disk, _position2);
        u5.d = (d1 < d2) ? d1 : d2;
        set_double(&disk, _position1, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position1);
        long double d4 = get_long_double(disk, _position2);
        u6.d = (d3 < d4) ? d3 : d4;
        set_long_double(&disk, _position1, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 10);
    return disk;
}

Disk caller_max(Disk disk, int index)
{
    int _position1 = get_int(disk, index + 1);
    int _position2 = get_int(disk, index + 5);
    int _type = get_byte(disk, index + 9);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position1] = (disk[_position1] > disk[_position2]) ? disk[_position1] : disk[_position2];
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position1);
        short s2 = get_short(disk, _position2);
        u.s = (s1 > s2) ? s1 : s2;
        set_short(&disk, _position1, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position1);
        int i2 = get_int(disk, _position2);
        u2.i = (i1 > i2) ? i1 : i2;
        set_int(&disk, _position1, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position1);
        long l2 = get_long(disk, _position2);
        u3.l = (l1 > l2) ? l1 : l2;
        set_long(&disk, _position1, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position1);
        float f2 = get_float(disk, _position2);
        u4.f = (f1 > f2) ? f1 : f2;
        set_float(&disk, _position1, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position1);
        double d2 = get_double(disk, _position2);
        u5.d = (d1 > d2) ? d1 : d2;
        set_double(&disk, _position1, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position1);
        long double d4 = get_long_double(disk, _position2);
        u6.d = (d3 > d4) ? d3 : d4;
        set_long_double(&disk, _position1, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 10);
    return disk;
}

Disk caller_random(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int minrangeposition = get_int(disk, index + 5);
    int maxrangeposition = get_int(disk, index + 9);
    int _type = get_byte(disk, index + 13);

    switch (_type)
    {
    case 1:// byte
        byte _min = disk[minrangeposition];
        byte _max = disk[maxrangeposition];
        disk[_position] = _min + rand() % (_max - _min + 1);
        break;
    case 2:// short
        short _min2 = get_short(disk, minrangeposition);
        short _max2 = get_short(disk, maxrangeposition);
        union {
            short s;
            byte b[2];
        } u;
        u.s = _min2 + rand() % (_max2 - _min2 + 1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        int _min3 = get_int(disk, minrangeposition);
        int _max3 = get_int(disk, maxrangeposition);
        union {
            int i;
            byte b[4];
        } u2;
        u2.i = _min3 + rand() % (_max3 - _min3 + 1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        long _min4 = get_long(disk, minrangeposition);
        long _max4 = get_long(disk, maxrangeposition);
        union {
            long l;
            byte b[8];
        } u3;
        u3.l = _min4 + rand() % (_max4 - _min4 + 1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        float _min5 = get_float(disk, minrangeposition);
        float _max5 = get_float(disk, maxrangeposition);
        union {
            float f;
            byte b[4];
        } u4;
        u4.f = _min5 + fmod(rand(), (_max5 - _min5 + 1));
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        double _min6 = get_double(disk, minrangeposition);
        double _max6 = get_double(disk, maxrangeposition);
        union {
            double d;
            byte b[8];
        } u5;
        u5.d = _min6 + fmod(rand(), (_max6 - _min6 + 1));
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        long double _min7 = get_long_double(disk, minrangeposition);
        long double _max7 = get_long_double(disk, maxrangeposition);
        union {
            long double d;
            byte b[10];
        } u6;
        u6.d = _min7 + fmod(rand(), (_max7 - _min7 + 1));
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 14);
    return disk;
}

Disk caller_round(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = round(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = round(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = round(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = round(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = round(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = round(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = round(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_floor(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = floor(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = floor(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = floor(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = floor(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = floor(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = floor(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = floor(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_ceil(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = ceil(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = ceil(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = ceil(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = ceil(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = ceil(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = ceil(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = ceil(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_trunc(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = trunc(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = trunc(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = trunc(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = trunc(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = trunc(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = trunc(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = trunc(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_sin(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = sin(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = sin(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = sin(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = sin(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = sin(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = sin(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = sin(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_cos(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = cos(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = cos(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = cos(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = cos(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = cos(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = cos(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = cos(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_tan(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = tan(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = tan(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = tan(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = tan(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = tan(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = tan(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = tan(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_asin(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = asin(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = asin(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = asin(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = asin(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = asin(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = asin(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = asin(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_acos(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = acos(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = acos(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = acos(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = acos(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = acos(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = acos(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = acos(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_atan(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = atan(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = atan(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = atan(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = atan(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = atan(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = atan(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = atan(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_atan2(Disk disk, int index)
{
    int _position1 = get_int(disk, index + 1);
    int _position2 = get_int(disk, index + 5);
    int _type = get_byte(disk, index + 9);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position1] = atan2(disk[_position1], disk[_position2]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position1);
        short s2 = get_short(disk, _position2);
        u.s = atan2(s1, s2);
        set_short(&disk, _position1, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position1);
        int i2 = get_int(disk, _position2);
        u2.i = atan2(i1, i2);
        set_int(&disk, _position1, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position1);
        long l2 = get_long(disk, _position2);
        u3.l = atan2(l1, l2);
        set_long(&disk, _position1, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position1);
        float f2 = get_float(disk, _position2);
        u4.f = atan2(f1, f2);
        set_float(&disk, _position1, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position1);
        double d2 = get_double(disk, _position2);
        u5.d = atan2(d1, d2);
        set_double(&disk, _position1, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position1);
        long double d4 = get_long_double(disk, _position2);
        u6.d = atan2(d3, d4);
        set_long_double(&disk, _position1, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 10);
    return disk;
}

Disk caller_sinh(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = sinh(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = sinh(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = sinh(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = sinh(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = sinh(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = sinh(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = sinh(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_cosh(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = cosh(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = cosh(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = cosh(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = cosh(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = cosh(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = cosh(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = cosh(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_tanh(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = tanh(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = tanh(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = tanh(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = tanh(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = tanh(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = tanh(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = tanh(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_exp(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = exp(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = exp(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = exp(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = exp(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = exp(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = exp(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = exp(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_frexp(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _exp = get_int(disk, index + 5);
    int _type = get_byte(disk, index + 9);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = frexp(disk[_position], &_exp);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        short s2 = _exp;
        u.s = frexp(s1, (int*)&s2);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        int i2 = _exp;
        u2.i = frexp(i1, &i2);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        long l2 = _exp;
        u3.l = frexp(l1, (int*)&l2);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        float f2 = _exp;
        u4.f = frexp(f1, (int*)&f2);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        double d2 = _exp;
        u5.d = frexp(d1, (int*)&d2);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        long double d4 = _exp;
        u6.d = frexp(d3, (int*)&d4);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    set_int(&disk, index + 5, _exp);
    _goto(&disk, index + 10);
    return disk;
}

Disk caller_ldexp(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _exp = get_int(disk, index + 5);
    int _type = get_byte(disk, index + 9);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = ldexp(disk[_position], _exp);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = ldexp(s1, _exp);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = ldexp(i1, _exp);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = ldexp(l1, _exp);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = ldexp(f1, _exp);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = ldexp(d1, _exp);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = ldexp(d3, _exp);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 10);
    return disk;
}

Disk caller_log(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = log(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = log(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = log(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = log(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = log(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = log(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = log(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_log2(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = log2(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = log2(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = log2(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = log2(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = log2(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = log2(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = log2(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_log10(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _type = get_byte(disk, index + 5);
    
    switch (_type)
    {
    case 1:// byte
        disk[_position] = log10(disk[_position]);
        break;
    case 2:// short
        union {
            short s;
            byte b[2];
        } u;
        short s1 = get_short(disk, _position);
        u.s = log10(s1);
        set_short(&disk, _position, u.s);
        break;
    case 3:// int
        union {
            int i;
            byte b[4];
        } u2;
        int i1 = get_int(disk, _position);
        u2.i = log10(i1);
        set_int(&disk, _position, u2.i);
        break;
    case 4:// long
        union {
            long l;
            byte b[8];
        } u3;
        long l1 = get_long(disk, _position);
        u3.l = log10(l1);
        set_long(&disk, _position, u3.l);
        break;
    case 5:// float
        union {
            float f;
            byte b[4];
        } u4;
        float f1 = get_float(disk, _position);
        u4.f = log10(f1);
        set_float(&disk, _position, u4.f);
        break;
    case 6:// double
        union {
            double d;
            byte b[8];
        } u5;
        double d1 = get_double(disk, _position);
        u5.d = log10(d1);
        set_double(&disk, _position, u5.d);
        break;
    case 7:// long double
        union {
            long double d;
            byte b[10];
        } u6;
        long double d3 = get_long_double(disk, _position);
        u6.d = log10(d3);
        set_long_double(&disk, _position, u6.d);
        break;
    default:
        break;
    }
    _goto(&disk, index + 6);
    return disk;
}

Disk caller_load(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _size = get_int(disk, index + 5);
    int _filename_size = get_int(disk, index + 9);
    char _filename[_filename_size];
    for (int i = 0; i < _filename_size; i++)
    {
        _filename[i] = disk[index + 13 + i];
    }
    Disk _data = disk_read(_filename);
    _set(&disk, _position, _size, _data);
    _goto(&disk, index + 13 + _filename_size);
    return disk;
}

Disk caller_save(Disk disk, int index)
{
    int _position = get_int(disk, index + 1);
    int _size = get_int(disk, index + 5);
    int _filename_size = get_int(disk, index + 9);
    Disk _filename = (Disk)malloc(_filename_size);
    Disk _data = (Disk)malloc(_size);
    for (int i = 0; i < _filename_size; i++)
    {
        _filename[i] = disk[index + 13 + i];
    }
    for (int i = 0; i < _size; i++)
    {
        _data[i] = disk[_position + i];
    }
    disk_write(_filename, _data);
    free(_filename);
    free(_data);
    _goto(&disk, index + 13 + _filename_size);
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
    caller_if,
    NULL,
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
    caller_log2,
    caller_log10,
    caller_load,
    caller_save,
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
        disk = functions[disk[i]](disk, i);
    }
    return disk;
}