#include "brimg.h"

int main (int argc, char *argv[])
{
    Disk disk = disk_read(argv[1]);
    _run(disk);
    return 0;
}