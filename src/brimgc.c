#include <libtcc.h>
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