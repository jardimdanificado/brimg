#include "brimg.h"
#include "structs.h"

int main()
{
    Disk disk0 = diskread("test.img");//disk are always little endian, so we need to change the order of the bytes
    int size = strlen(disk0);
    printf("Disk0: %d\n", size);
}