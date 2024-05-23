#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256

#define byte unsigned char
byte bigendian;

typedef struct {
    int x;
    int y;
} Vec2;

typedef struct {
    byte r;
    byte g;
    byte b;
} RGB;
#define RGB(r, g, b) (RGB){r, g, b}

typedef struct {
    void *data;
    byte type;
} Generic;

typedef struct {
    void **data;
    size_t size;
} List;
#define List() (List){NULL, 0}

void ****memory;
List *disks;

// Define the push function
void *push(List *list, void *data) 
{
    // Reallocate the data array to hold one more element
    list->data = realloc(list->data, (list->size + 1) * sizeof(void *));
    if (list->data == NULL) {
        // Handle allocation failure if needed
        perror("Failed to reallocate memory");
        exit(EXIT_FAILURE);
    }
    // Add the new data element to the list
    list->data[list->size] = data;
    // Increment the size of the list
    list->size++;
    return data;  // Return the added data (or something else as needed)
}

void *insert(List *list, size_t index, void *data) 
{
    if (index > list->size) {
        // Handle out-of-bounds index if needed
        return NULL;
    }
    // Reallocate the data array to hold one more element
    list->data = realloc(list->data, (list->size + 1) * sizeof(void *));
    if (list->data == NULL) {
        // Handle allocation failure if needed
        perror("Failed to reallocate memory");
        exit(EXIT_FAILURE);
    }
    // Shift the data elements after the inserted element to the right
    for (size_t i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }
    // Insert the new data element into the list
    list->data[index] = data;
    // Increment the size of the list
    list->size++;
    return data;  // Return the inserted data (or something else as needed)
}

void *unpush(List *list, size_t index) 
{
    if (index >= list->size) {
        // Handle out-of-bounds index if needed
        return NULL;
    }
    void *data = list->data[index];
    // Shift the data elements after the removed element to the left
    for (size_t i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    // Reallocate the data array to hold one less element
    list->data = realloc(list->data, (list->size - 1) * sizeof(void *));
    if (list->data == NULL && list->size > 1) {
        // Handle reallocation failure if needed
        perror("Failed to reallocate memory");
        exit(EXIT_FAILURE);
    }
    // Decrement the size of the list
    list->size--;
    return data;  // Return the removed data (or something else as needed)
}

void *get(List *list, size_t index) 
{
    if (index >= list->size) {
        // Handle out-of-bounds index if needed
        return NULL;
    }
    return list->data[index];
}

void ****newMemory()
{
    void ****memory = (void ****)malloc(SIZE * sizeof(void ***));
    for (short i = 0; i < SIZE; i++) {
        memory[i] = (void ***)malloc(SIZE * sizeof(void **));
        for (short j = 0; j < SIZE; j++) {
            memory[i][j] = (void **)malloc(SIZE * sizeof(void *));
            for (short k = 0; k < SIZE; k++) {
                memory[i][j][k] = (NULL); // Inicializando os ponteiros como NULL
            }
        }
    }
    memory[0][0][0] = NULL; // Liberando o ponteiro
    memory[0][0][1] = "brimg"; // Atribuindo uma função ao ponteiro

    memory[0][0][5] = 0; 
    memory[0][0][6] = 0;
    //memory[0][0][10] = sum;

    return memory;
}

RGB gettoken(int disk, int index)
{
    RGB token;
    
    if (disk < disks->size) {
        List disk0 = *(List *)disks->data[disk];
        if (index < disk0.size) {
            token = *(RGB *)disk0.data[index];
        }
    }

    return token;
}

RGB currenttoken()
{
    return gettoken((int) memory[0][0][5], (int) memory[0][0][6]);
}

RGB nexttoken()
{
    return gettoken(memory[0][0][5], memory[0][0][6]+1);
}

int Int()
{
    int value = 0;
    RGB token = nexttoken();
    memory[0][0][6]+=2;
    if (bigendian) 
    {
        value = (token.r << 16) | (token.g << 8) | token.b;
    } 
    else 
    {
        value = (token.b << 16) | (token.g << 8) | token.r;
    }
    return value;
}

byte *String()
{
    RGB token = nexttoken();
    byte *str = "";
    while (token.r != 0 && token.g != 0 && token.b != 0) 
    {
        str += token.r;
        str += token.g;
        str += token.b;
        memory[0][0][6]+=1;
        token = nexttoken();
    }
    
    memory[0][0][6]+=2;
    str[0] = token.r;
    str[1] = token.g;
    str[2] = token.b;
    return str;
}

void *parseToken(int disk, int index)
{
    RGB token = gettoken(disk, index);
    if (token.r == 'I') 
    {
        return Int();
    }
    return NULL;
}

int setmemory(byte r, byte g, byte b)
{
    RGB token = currenttoken();
    memory[r][g][b] = &token;
    return 0;
}

int main() 
{
    int n = 1;

    bigendian = !(*(char *)&n == 1);
    //RGB *disk0 = (RGB *)malloc(sizeof(RGB)*disk0size);

    // Alocando memória dinamicamente para a matriz
    memory = newMemory();

    // Exemplo de uso: atribuindo alguns ponteiros aos elementos da matriz
    //float value = 1.1;
    //array[0][0][0] = &value;
    //array[0][3][0] = &sum; // Atribuindo uma função ao ponteiro

    // Exemplo de acesso aos elementos da matriz usando a função array[0][3][0]
    //printf("2 + 2 =: %d\n", ((int (*)(int, int))array[0][3][0])(2, 2));

    /*List *lista = &List();
    char *str = "Hello, World!";
    char *str2 = "Hello, World! 2";
    push(lista, &str);
    insert(lista, 0, &str2);*/
    //unpush(lista, 0);
    //printf("Lista: %s\n", *(char **)lista->data[0]);

    

    memory[0][0][0] = NULL; // Liberando o ponteiro
    char* str3;
    scanf("%s", str3);
    // Liberando a memória alocada
    for (int i = 0; i < SIZE; i++) 
    {
        for (int j = 0; j < SIZE; j++) 
        {
            free(memory[i][j]);
        }
        free(memory[i]);
    }
    free(memory);
    

    return 0;
}
