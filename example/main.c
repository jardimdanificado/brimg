#include "brimg.h"

// works like speacial headers
#bruter "main.br"

int main()
{
    Disk disk0 = disk_read("disk0.img");
    // bytecode interpreter test
    _run(disk0);
    
    Person p;
    p.age = 244;
    p.weight = 99980.5;
    p.height = 999891.75;
    strcpy(p.name, "joao boladao");
    p.id = 12349956;
    _insert(&disk0, 0, "Hello World!Hello World!");
    set_Person(&disk0, 0, p);
    union {
        Person p;
        byte b[sizeof(Person)];
    } u;
    u.p = p;
    printf("sizeof(Person): %d\n", sizeof(Person));
    for (int i = 0; i < 28; i++)
    {
        printf("%d: %d\n", disk0[i], u.b[i]);
    }
    int size = strlen((Disk)disk0);
    printf("Disk0: %s\n", disk0);
    printf("Size: %d\n", size);
    disk_write("disk1.img", disk0);
}