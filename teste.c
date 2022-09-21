#include <stdlib.h>
#include <stdio.h>

typedef struct Flor{
    int vertice;
    double sepal_lenght,sepal_widht, petal_lenght, petal_widht;
    char type[20];
    struct Flor *adj;
}Flor;

typedef struct Vertice{
    Flor *inicio;
}Vertice;

typedef struct Grafo{
    int vertices;
    int arestas;
    Vertice *adj;
}Grafo;

int main(int argc, char const *argv[]){
    
    return 0;
}
