#include "brimg.h"

int main (int argc, char *argv[])
{
    Disk disk = disk_read(argv[1]);
    if (disk == NULL) {
        printf("Error: disk_read\n");
        return 1;
    }

    if (disk[2] == VERSION_X && disk[3] == VERSION_Y) 
    {
        printf("brimg v%d.%d\n", disk[2], disk[3]);
    } 
    else 
    {
        printf("Warning: version mismatch, expected brimg v%d.%d bytecode but got brimg v%d.%d bytecode\n", VERSION_X, VERSION_Y, disk[2], disk[3]);
    }
    _run(disk);
    return 0;
}