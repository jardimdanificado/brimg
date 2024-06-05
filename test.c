#include "brimg.h"

int main()
{
    Disk disk0 = txtload("test.txt");//disk are always little endian, so we need to change the order of the bytes
    printf("Disk0: %s\n", disk0);
}