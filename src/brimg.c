#include "brimg.h"

// StandardFunctions
// StandardFunctions
// StandardFunctions

void _set(Disk *disk, int index, byte *data)
{
    for (int i = 0; i < strlen(data); i++)
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

void _remove(Disk *disk, int index, int size)
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

// replace first occurence of data with replacement
void _replace(Disk *disk, int offsetmin, int offsetmax, byte* data, byte* replacement)
{
    int disksize = strlen(*disk);
    char *point = strstr(*disk + offsetmin, data);
    int index = (int*)(point) - (int*)(*disk);
    if (index < offsetmin || index > offsetmax)
    {
        return;
    }
    int size = strlen(data);
    int diff = strlen(replacement) - size;
    if (diff > 0)
    {
        *disk = (Disk)realloc(*disk, (disksize + diff) * sizeof(Disk));
    }
    else if (diff < 0)
    {
        *disk = (Disk)realloc(*disk, (disksize + diff) * sizeof(Disk));
        disksize = strlen(*disk);
        diff = -diff;
        for (int j = disksize - 1; j >= index + size; j--)
        {
            (*disk)[j + diff] = (*disk)[j];
        }
    }
    for (int j = 0; j < strlen(replacement); j++)
    {
        (*disk)[index + j] = replacement[j];
    }
}

void _replace_all(Disk *disk, int offsetmin, int offsetmax, byte* data, byte* replacement)
{
    int disksize = strlen(*disk);
    int size = strlen(data);
    int diff = strlen(replacement) - size;
    int count = 0;
    for (int i = offsetmin; i < offsetmax; i++)
    {
        char *point = strstr(*disk + i, data);
        int index = (int*)(point) - (int*)(*disk);
        if (index < offsetmin || index > offsetmax)
        {
            break;
        }
        count++;
        if (diff > 0)
        {
            *disk = (Disk)realloc(*disk, (disksize + diff) * sizeof(Disk));
        }
        else if (diff < 0)
        {
            *disk = (Disk)realloc(*disk, (disksize + diff) * sizeof(Disk));
            disksize = strlen(*disk);
            diff = -diff;
            for (int j = disksize - 1; j >= index + size; j--)
            {
                (*disk)[j + diff] = (*disk)[j];
            }
        }
        for (int j = 0; j < strlen(replacement); j++)
        {
            (*disk)[index + j] = replacement[j];
        }
    }
}

// 

void _if(Disk *disk, byte condition, int size, int posi1, int posi2, int goto1, int goto2)
{
    int _true = 0;
    if (condition == 0) // equal ==
    {
        _true = 1;
        for (int i = 0; i < size; i++)
        {
            if ((*disk)[posi1 + i] != (*disk)[posi2 + i])
            {
                _true = 0;
                break;
            }
        } 
    }
    else if (condition == 1) // not equal !=
    {
        _true = 1;
        for (int i = 0; i < size; i++)
        {
            if ((*disk)[posi1 + i] != (*disk)[posi2 + i])
            {
                _true = 1;
                break;
            }
        }
    }
    else if (condition == 2) // greater >
    {
        _true = 1;
        for (int i = 0; i < size; i++)
        {
            if ((*disk)[posi1 + i] <= (*disk)[posi2 + i])
            {
                _true = 0;
                break;
            }
        }
    }
    else if (condition == 3) // less <
    {
        _true = 1;
        for (int i = 0; i < size; i++)
        {
            if ((*disk)[posi1 + i] >= (*disk)[posi2 + i])
            {
                _true = 0;
                break;
            }
        }
    }
    else if (condition == 4) // greater or equal >=
    {
        _true = 1;
        for (int i = 0; i < size; i++)
        {
            if ((*disk)[posi1 + i] < (*disk)[posi2 + i])
            {
                _true = 0;
                break;
            }
        }
    }
    else if (condition == 5) // less or equal <=
    {
        _true = 1;
        for (int i = 0; i < size; i++)
        {
            if ((*disk)[posi1 + i] > (*disk)[posi2 + i])
            {
                _true = 0;
                break;
            }
        }
    }

    union {
        int i;
        byte b[4];
    } u;

    u.i = _true ? goto1 : goto2;

    _set(disk, 4, u.b);
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
    return u.i;
}

float get_float(Disk disk, int index)
{
    union {
        float f;
        byte b[4];
    } u;
    return u.f;
}

// sets 
// sets 
// sets 

void set_byte(Disk *disk, int index, byte data)
{
    _set(disk, index, (byte[1]){data});
}

void set_string(Disk *disk, int index, byte *str, int size)
{
    _set(disk, index, str);
}

void set_int(Disk *disk, int index, int data)
{
    union {
        int i;
        byte b[4];
    } u;
    u.i = data;
    byte *bytes = get_bytes((byte *)&u, 0, 4);
    _set(disk, index, bytes);
}

void set_float(Disk *disk, int index, float data)
{
    union {
        float f;
        byte b[4];
    } u;
    u.f = data;
    byte *bytes = get_bytes((byte *)&u, 0, 4);
    _set(disk, index, bytes);
}

void set_short(Disk *disk, int index, short data)
{
    union {
        short s;
        byte b[2];
    } u;
    u.s = data;
    _set(disk, index, (byte[2]){u.b[0],u.b[1]});
}

void set_double(Disk *disk, int index, double data)
{
    union {
        double d;
        byte b[8];
    } u;
    u.d = data;
    _set(disk, index, (byte[8]){u.b[0],u.b[1],u.b[2],u.b[3],u.b[4],u.b[5],u.b[6],u.b[7]});
}

void set_long(Disk *disk, int index, long data)
{
    union {
        long l;
        byte b[4];
    } u;
    u.l = data;
    _set(disk, index, (byte[4]){u.b[0],u.b[1],u.b[2],u.b[3]});
}

void set_long_double(Disk *disk, int index, long double data)
{
    union {
        long double d;
        byte b[10];
    } u;
    u.d = data;
    _set(disk, index, (byte[10]){u.b[0],u.b[1],u.b[2],u.b[3],u.b[4],u.b[5],u.b[6],u.b[7],u.b[8],u.b[9]});
}

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


    add_symbols(s, "_set", _set);
    add_symbols(s, "_insert", _insert);
    add_symbols(s, "_remove", _remove);
    add_symbols(s, "_move", _move);
    add_symbols(s, "_swap", _swap);
    add_symbols(s, "_shift", _shift);
    add_symbols(s, "_random", _random);
    add_symbols(s, "_copy", _copy);
    add_symbols(s, "_fill", _fill);
    add_symbols(s, "_reverse", _reverse);
    add_symbols(s, "_sort", _sort);
    add_symbols(s, "_replace", _replace);
    add_symbols(s, "_replace_all", _replace_all);

    add_symbols(s, "disk_read", disk_read);
    add_symbols(s, "disk_write", disk_write);


    if (tcc_relocate(s) < 0)
        exit(1);

    void (*func)(int) = (void (*)(int))get_symbol(s, "main");

    func(32);
    tcc_delete(s);
    return 0;
}