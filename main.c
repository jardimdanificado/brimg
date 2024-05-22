#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256

#define byte char

typedef struct {
    void *data;
    char type;
} Generic;

typedef struct {
    void **data;
    size_t size;
} List;

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

#define List() (List){NULL, 0}


typedef struct {
    int x;
    int y;
} Vec2;

typedef struct {
    char r;
    char g;
    char b;
} RGB;
#define RGB(r, g, b) (RGB){r, g, b}

#define Disk List

#define sum(a, b) (a + b)

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

int readnext()
{
    
}

int main() 
{
    //RGB *disk0 = (RGB *)malloc(sizeof(RGB)*disk0size);

    // Alocando memória dinamicamente para a matriz
    void ****memory = newMemory();

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
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            free(memory[i][j]);
        }
        free(memory[i]);
    }
    free(memory);
    

    return 0;
}
