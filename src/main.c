#include "brimg.h"

int main (int argc, char *argv[])
{
    Disk disk = disk_read(argv[1]);
    if (disk == NULL) {
        printf("Error: disk_read\n");
        return 1;
    }

    if (disk[1] == VERSION_X && disk[2] == VERSION_Y && disk[3] == VERSION_Z) {
        printf("brimg v%d.%d.%d\n", disk[1], disk[2], disk[3]);
    } 
    else 
    {
        printf("Error: version mismatch, expected brimg v%d.%d.%d bytecode but got brimg v%d.%d.%d bytecode\n", VERSION_X, VERSION_Y, VERSION_Z, disk[1], disk[2], disk[3]);
        return 1;
    }
    _run(disk);
    return 0;
}