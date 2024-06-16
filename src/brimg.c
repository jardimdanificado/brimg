#include "brimg.h"






// tcc
// tcc
// tcc






void handle_error(void *opaque, const char *msg) {
    fprintf(opaque, "%s\n", msg);
}


TCCState* create_tcc_state() 
{
    TCCState *s = tcc_new();
    if (!s) {
        fprintf(stderr, "Could not create tcc state\n");
        exit(1);
    }
    tcc_set_error_func(s, stderr, handle_error);
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
    return s;
}


void compile_code(TCCState *s, const char *code) {
    if (tcc_compile_string(s, code) == -1)
        exit(1);
}


void add_symbols(TCCState *s, const char *name, void *addr) {
    tcc_add_symbol(s, name, addr);
}


void* get_symbol(TCCState *s, const char *name) {
    void *sym = tcc_get_symbol(s, name);
    if (!sym) {
        fprintf(stderr, "Could not find symbol %s\n", name);
        exit(1);
    }
    return sym;
}


TCCState *new_state(const char *code) {
    TCCState *s = create_tcc_state();
    compile_code(s, code);
    
    return s;
}













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
        return;
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
        return;
    }
    else if (index1 < 0 || index2 < 0 || index1 >= disksize || index2 >= disksize || index2 + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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

void _find(Disk *disk, int index, int size, byte* data, int result)
{
    int disksize = strlen(*disk);
    char *point = strstr(*disk + index, data);
    index = (int*)(point) - (int*)(*disk);
    if (index < 0 || index >= disksize || index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
    }
    else
    {
        int position = (int*)(point) - (int*)(*disk);
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
        return;
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
    //set disk index to next instruction
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
    //set disk index to next instruction
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
    
    //set disk index to next instruction
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
    //set disk index to next instruction
    _goto(&disk, index + 9);
    return disk;
}

Disk caller_move(Disk disk, int index)
{
    //_move(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9));
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
        return;
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
    //set disk index to next instruction
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_swap(Disk disk, int index)
{
    //_swap(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9));
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
        return;
    }
    else if (index1 < 0 || index2 < 0 || index1 >= disksize || index2 >= disksize || index2 + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        byte temp = disk[index1 + i];
        disk[index1 + i] = disk[index2 + i];
        disk[index2 + i] = temp;
    }
    //set disk index to next instruction
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_shift(Disk disk, int index)
{
    //_shift(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9));
    int size = get_int(disk, index + 5);
    int _shift = get_int(disk, index + 9);
    int _index = get_int(disk, index + 1);
    int disksize = strlen(disk);
    // slide items based of _shift - to the left, + to the right, 0 change nothing, 0123456789 -> 0781234569 with shift 3 in a interval from 1 to 8
    if (_index < 0 || _index >= disksize || _index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        return;
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
    
    //set disk index to next instruction
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_random(Disk disk, int index)
{
    int _index = get_int(disk, index + 1);
    //_random(&disk, get_int(disk, index + 1), get_int(disk, index + 5));
    srand(time(NULL));
    int size = get_int(disk, _index + 5);
    for (int i = 0; i < size; i++)
    {
        disk[_index + i] = rand() % 256;
    }
    //set disk index to next instruction
    _goto(&disk, _index + 9);
    return disk;
}

Disk caller_copy(Disk disk, int index)
{
    //_copy(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9));
    int disksize = strlen(disk);
    int size = get_int(disk, index + 9);
    int _index = get_int(disk, index + 1);
    int destiny = get_int(disk, index + 5);
    if (_index < 0 || destiny < 0 || _index >= disksize || destiny >= disksize || _index + size > disksize || destiny + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        disk[destiny + i] = disk[_index + i];
    }
    //set disk index to next instruction
    _goto(&disk, _index + 13);
    return disk;
}

Disk caller_fill(Disk disk, int index)
{
    //_fill(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_byte(disk, index + 9));
    int size = get_int(disk, index + 5);
    byte data = get_byte(disk, index + 9);
    int _index = get_int(disk, index + 1);
    for (int i = 0; i < size; i++)
    {
        disk[_index + i] = data;
    }
    //set disk index to next instruction
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_reverse(Disk disk, int index)
{
    //_reverse(&disk, get_int(disk, index + 1), get_int(disk, index + 5));
    int disksize = strlen(disk);
    int size = get_int(disk, index + 5);
    int _index = get_int(disk, index + 1);

    if (_index < 0 || _index >= disksize || _index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        return;
    }

    for (int i = 0; i < size / 2; i++)
    {
        byte temp = disk[_index + i];
        disk[_index + i] = disk[_index + size - 1 - i];
        disk[_index + size - 1 - i] = temp;
    }
    //set disk index to next instruction
    _goto(&disk, index + 9);
    return disk;
}

Disk caller_sort(Disk disk, int index)
{
    //_sort(&disk, get_int(disk, index + 1), get_int(disk, index + 5));
    int disksize = strlen(disk);
    int size = get_int(disk, index + 5);
    int _index = get_int(disk, index + 1);

    if (_index < 0 || _index >= disksize || _index + size > disksize)
    {
        printf("Error: Index out of bounds\n");
        return;
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
    //set disk index to next instruction
    _goto(&disk, index + 9);
    return disk;
}

Disk caller_find(Disk disk, int index)
{
    
}

// fbiiii?
Disk caller_if(Disk disk, int index)
{
    int size = get_int(disk, index + 2);
    if(get_byte(disk, index+1) == 0)
    {
        byte *data = get_bytes(disk, index + 6, size);
        _run(data);
    }
    _goto(&disk, index + 6 + size);
    return disk;
}

// f b iiii iiii ? ?
Disk caller_ifelse(Disk disk, int index)
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
}

Disk caller_equal(Disk disk, int index)
{
    //_equal(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9), get_int(disk, index + 13));
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
    //set disk index to next instruction
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_not_equal(Disk disk, int index)
{
    //_not_equal(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9), get_int(disk, index + 13));
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
    //set disk index to next instruction
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_greater(Disk disk, int index)
{
    //_greater(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9), get_int(disk, index + 13));
    byte _true = 1;
    for (int i = 0; i < get_int(disk, index + 9); i++)
    {
        if (disk[get_int(disk, index + 1) + i] <= disk[get_int(disk, index + 5) + i])
        {
            _true = 0;
            break;
        }
    }
    disk[get_int(disk, index + 13)] = _true;
    //set disk index to next instruction
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_less(Disk disk, int index)
{
    //_less(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9), get_int(disk, index + 13));
    byte _true = 1;
    for (int i = 0; i < get_int(disk, index + 9); i++)
    {
        if (disk[get_int(disk, index + 1) + i] >= disk[get_int(disk, index + 5) + i])
        {
            _true = 0;
            break;
        }
    }
    disk[get_int(disk, index + 13)] = _true;
    //set disk index to next instruction
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_less_or_equal(Disk disk, int index)
{
    //_less_or_equal(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9), get_int(disk, index + 13));
    byte _true = 1;
    for (int i = 0; i < get_int(disk, index + 9); i++)
    {
        if (disk[get_int(disk, index + 1) + i] > disk[get_int(disk, index + 5) + i])
        {
            _true = 0;
            break;
        }
    }
    disk[get_int(disk, index + 13)] = _true;
    //set disk index to next instruction
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_greater_or_equal(Disk disk, int index)
{
    //_greater_or_equal(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9), get_int(disk, index + 13));
    byte _true = 1;
    for (int i = 0; i < get_int(disk, index + 9); i++)
    {
        if (disk[get_int(disk, index + 1) + i] < disk[get_int(disk, index + 5) + i])
        {
            _true = 0;
            break;
        }
    }
    disk[get_int(disk, index + 13)] = _true;
    //set disk index to next instruction
    _goto(&disk, index + 17);
    return disk;
}

Disk caller_and(Disk disk, int index)
{
    //_and(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9));
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
    //set disk index to next instruction
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_or(Disk disk, int index)
{
    //_or(&disk, get_int(disk, index + 1), get_int(disk, index + 5), get_int(disk, index + 9));
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
    //set disk index to next instruction
    _goto(&disk, index + 13);
    return disk;
}

Disk caller_print(Disk disk, int index)
{
    //_print(disk, get_int(disk, index + 1), get_int(disk, index + 5));
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
    //_goto(&disk, get_int(disk, index + 1));
    set_int(disk, 4, get_int(disk, index + 1));

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
    caller_ifelse,
    caller_equal,
    caller_not_equal,
    caller_greater,
    caller_less,
    caller_less_or_equal,
    caller_greater_or_equal,
    caller_and,
    caller_or,
    caller_print,
    caller_goto
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
        printf("disk0: %d\n", disk[0]);
        i = get_int(disk, 4);
        disk = functions[disk[i]](disk, i);
    }
    return disk;
}

// main
// main
// main
int main(int argc, char *argv[]) 
{

    char *inpath = argv[1];
    char *outpath = argv[2];
    int n = 1;

    bigendian = !(*(char *)&n == 1);
    printf("bigendian: %d\n", bigendian);
    
    TCCState *s = new_state(disk_read(inpath));

    add_symbols(s, "get_bytes", get_bytes);
    add_symbols(s, "get_byte", get_byte);
    add_symbols(s, "get_string", get_string);
    add_symbols(s, "get_int", get_int);
    add_symbols(s, "get_float", get_float);
    add_symbols(s, "get_short", get_short);
    add_symbols(s, "get_double", get_double);
    add_symbols(s, "get_long", get_long);
    add_symbols(s, "get_long_double", get_long_double);

    add_symbols(s, "set_byte", set_byte);
    add_symbols(s, "set_string", set_string);
    add_symbols(s, "set_int", set_int);
    add_symbols(s, "set_float", set_float);
    add_symbols(s, "set_short", set_short);
    add_symbols(s, "set_double", set_double);
    add_symbols(s, "set_long", set_long);
    add_symbols(s, "set_long_double", set_long_double);

    add_symbols(s, "_state", _state);
    add_symbols(s, "_set", _set);
    add_symbols(s, "_insert", _insert);
    add_symbols(s, "_delete", _delete);
    add_symbols(s, "_move", _move);
    add_symbols(s, "_swap", _swap);
    add_symbols(s, "_shift", _shift);
    add_symbols(s, "_random", _random);
    add_symbols(s, "_copy", _copy);
    add_symbols(s, "_fill", _fill);
    add_symbols(s, "_reverse", _reverse);
    add_symbols(s, "_sort", _sort);
    add_symbols(s, "_find", _find);
    add_symbols(s, "_equal", _equal);
    add_symbols(s, "_not_equal", _not_equal);
    add_symbols(s, "_greater", _greater);
    add_symbols(s, "_less", _less);
    add_symbols(s, "_less_or_equal", _less_or_equal);
    add_symbols(s, "_greater_or_equal", _greater_or_equal);
    add_symbols(s, "_and", _and);
    add_symbols(s, "_or", _or);
    add_symbols(s, "_print", _print);
    add_symbols(s, "_goto", _goto);
    
    add_symbols(s, "_run", _run);

    add_symbols(s, "disk_read", disk_read);
    add_symbols(s, "disk_write", disk_write);


    if (tcc_relocate(s) < 0)
        exit(1);

    void (*func)(int) = (void (*)(int))get_symbol(s, "main");

    func(32);
    tcc_delete(s);
    return 0;
}