#include "brimg.h"

// StandardFunctions
// StandardFunctions
// StandardFunctions

void _set(Disk *disk, int index, uint8_t data)
{
    (*disk)[index] = data;
}

void _insert(Disk *disk, int index, char *str)
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
        uint8_t temp = (*disk)[index1 + i];
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
            uint8_t temp = (*disk)[index];
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
            uint8_t temp = (*disk)[index + size - 1];
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

void _fill(Disk *disk, int index, int size, uint8_t data)
{
    for (int i = 0; i < size; i++)
    {
        (*disk)[index + i] = data;
    }
}

// DiskManagement functions
// DiskManagement functions
// DiskManagement functions

char *diskread(char *filename)
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

    char *buffer = (char *)malloc(size + 1);
    fread(buffer, size, 1, file);
    fclose(file);

    buffer[size] = '\0';
    return buffer;
}

void diskwrite(char *filename, char *data)
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

uint8_t* get_bytes(Disk disk, int index, int size)
{
    uint8_t *bytes = (uint8_t *)malloc(size * sizeof(uint8_t));
    for (int i = 0; i < size; i++)
    {
        bytes[i] = disk[index + i];
    }
    return bytes;
}


uint8_t get_byte(Disk disk, int index)
{
    return disk[index];
}

char* get_string(Disk disk, int index, int size)
{
    char *str = (char *)malloc(size * sizeof(char));
    for (int i = 0; i < size; i++)
    {
        str[i] = disk[index + i];
    }
    return str;
}

Number get_number(uint8_t *bytes)
{
    Number u;

    if (bigendian) {
        u.b[0] = bytes[0];
        u.b[1] = bytes[1];
        u.b[2] = bytes[2];
        u.b[3] = bytes[3];
    } else {
        u.b[3] = bytes[0];
        u.b[2] = bytes[1];
        u.b[1] = bytes[2];
        u.b[0] = bytes[3];
    }

    return u;
}

int get_int(Disk disk, int index)
{
    Number u = get_number(get_bytes(disk, index, 4));
    return u.i;
}

float get_float(Disk disk, int index)
{
    Number u = get_number(get_bytes(disk, index, 4));
    return u.f;
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
    
    TCCState *s = new_state(diskread(inpath));

    add_symbols(s, "get_number", get_number);
    add_symbols(s, "get_bytes", get_bytes);
    add_symbols(s, "get_byte", get_byte);
    add_symbols(s, "get_string", get_string);
    add_symbols(s, "get_int", get_int);
    add_symbols(s, "get_float", get_float);
    add_symbols(s, "_set", _set);
    add_symbols(s, "_insert", _insert);
    add_symbols(s, "_remove", _remove);
    add_symbols(s, "_move", _move);
    add_symbols(s, "_swap", _swap);
    add_symbols(s, "_shift", _shift);
    add_symbols(s, "_random", _random);
    add_symbols(s, "_copy", _copy);
    add_symbols(s, "_fill", _fill);
    add_symbols(s, "diskread", diskread);
    add_symbols(s, "diskwrite", diskwrite);

    if (tcc_relocate(s) < 0)
        exit(1);

    void (*func)(int) = (void (*)(int))get_symbol(s, "main");

    func(32);
    tcc_delete(s);
    return 0;
}