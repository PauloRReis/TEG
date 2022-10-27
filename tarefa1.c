//Alunos: Carlos Bunn e Paulo Ricardo

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

//1 - SETOSA
//2 - VERSICOLOR
//3 - VIRGINICA

typedef struct Flor{
    int id;
    double sepal_lenght,sepal_widht, petal_lenght, petal_widht;
    int type;
}Flor;

typedef struct FloresDistEuclidiana{
    int vertice1, vertice2;
    int type1, type2;
    double euclidiana;
}FloresDistEuclidiana;

typedef struct Aresta{
    int vertice1, vertice2;
    int type1,type2;
    struct Aresta *prox;
}Aresta;

typedef struct ListaArestas{
    Aresta *primeira;
    int quantidade;
}ListaArestas;

Flor *getFlores();
FloresDistEuclidiana *resolveDistanciaEuclidiana(Flor *flores, double *max, double *min);
FloresDistEuclidiana *normalizaFlor(FloresDistEuclidiana *euclidianas, double max, double min);
ListaArestas *listaArestasGrafo(FloresDistEuclidiana *euclidianas);
void matrizConfusao(ListaArestas *listaArestas);
void arquivoGrafo(ListaArestas *lista);
void toStringFlores(Flor *flores);

int main(int argc, char const *argv[]){
    double max, min;
    Flor* flores;
    FloresDistEuclidiana *euclidiana;
    FloresDistEuclidiana *ListaNormalizada;
    ListaArestas *lista;

    //consome o arquivo .csv retornando uma lista das flores
    flores = getFlores();
    //toStringFlores(flores);
    //recebe a lista de flores e resolve a distancia euclidiana para todos os pares de vertices
    euclidiana = resolveDistanciaEuclidiana(flores, &max, &min);
    //normaliza a lista de todos os pares de vertices
    ListaNormalizada = normalizaFlor(euclidiana, max, min);
    //forma uma lista com as distancias euclidianas menores que 0,3
    lista = listaArestasGrafo(ListaNormalizada);

    matrizConfusao(lista);
    //gera um arquivo .csv com a lista de arestas
    arquivoGrafo(lista);

    return 0;
}

Flor *getFlores(){

    Flor *flores = malloc(150 * sizeof(Flor));
    FILE *arq = fopen("IrisDataset.csv", "rt");
    char *result;
    char *token;

    char string[255];

    int i = 0;
    while(!feof(arq)){
        result = fgets(string, 150, arq);

        if(result){
            token = strtok(result, ",");
        }

        Flor novo;
        int j = 0;
        while(token != NULL){
            switch(j){
                case 0:
                    novo.sepal_lenght = atof(token);
                    break;
                case 1:
                    novo.sepal_widht = atof(token);
                    break;
                case 2:
                    novo.petal_lenght = atof(token);
                    break;
                case 3:
                    novo.petal_widht = atof(token);
                    break;
                case 4:
                    if(i <= 50){
                        novo.type = 1;
                    }else if(i > 50 && i<=100){
                        novo.type = 2;
                    }else if(i>100 && i<=150){
                        novo.type = 3;
                    }
            }
            token = strtok(NULL, ",");
            j++;
        }
        novo.id = i+1;
        flores[i] = novo;
        i++;
    } 
    fclose(arq);

    return flores;
}

FloresDistEuclidiana *resolveDistanciaEuclidiana(Flor *flores, double *max, double *min){

    FloresDistEuclidiana *floresEuclidianas = malloc((150*150-150)*sizeof(FloresDistEuclidiana));

    int distancia = 0;
    for(int i = 0; i < 150; i++){
        for(int j = 0; j < 150; j++){
            if(i==j){
                continue;
            }
            double euclidiana = sqrt(pow(flores[i].sepal_lenght - flores[j].sepal_lenght, 2) + pow(flores[i].sepal_widht - flores[j].sepal_widht,2) + pow(flores[i].petal_lenght - flores[j].petal_lenght,2) + pow(flores[i].petal_widht - flores[j].petal_widht , 2));

            if(euclidiana > *(max)){
                *max = euclidiana;
            }else if(euclidiana < *(min)){
                *min = euclidiana;
            }
            
            FloresDistEuclidiana novo;
            novo.vertice1 = i+1;
            novo.vertice2 = j+1;
            novo.euclidiana = euclidiana;
            novo.type1 = flores[i].type;
            novo.type2 = flores[j].type;
            floresEuclidianas[distancia] = novo;
            distancia++;
        }
    }

    return floresEuclidianas;
}

FloresDistEuclidiana *normalizaFlor(FloresDistEuclidiana *euclidianas, double max, double min){

    FloresDistEuclidiana *floresEuclidianas = malloc((150*150-150) * sizeof(FloresDistEuclidiana));

    for(int i = 0; i < (150*150-150); i++){
        FloresDistEuclidiana novo;
        novo.vertice1 = euclidianas[i].vertice1;
        novo.vertice2 = euclidianas[i].vertice2;
        novo.euclidiana = (euclidianas[i].euclidiana - min)/(max - min);
        novo.type1 = euclidianas[i].type1;
        novo.type2 = euclidianas[i].type2;
        floresEuclidianas[i] = novo;
    }

    return floresEuclidianas;
}

ListaArestas *listaArestasGrafo(FloresDistEuclidiana *euclidianas){

    ListaArestas *lista = malloc(sizeof(ListaArestas));
    Aresta *aresta;
    lista->quantidade = 0;

    for(int i = 0; i < (150*150-150) ; i++){
        if(euclidianas[i].euclidiana > 0.3){
            continue;
        }
        lista->quantidade++;

        if(lista->quantidade == 1){
            lista->primeira = malloc(sizeof(Aresta));
            aresta = lista->primeira;
        }else{
            aresta->prox = malloc(sizeof(Aresta));
            aresta = aresta->prox;
        }

        aresta->type1 = euclidianas[i].type1;
        aresta->type2 = euclidianas[i].type2;
        aresta->vertice1 = euclidianas[i].vertice1;
        aresta->vertice2 = euclidianas[i].vertice2;
        printf("%d %d %d %d\n", aresta->vertice1, aresta->vertice2, aresta->type1, aresta->type2);
    }

    printf("%d\n", lista->quantidade);

    return lista;
}

void arquivoGrafo(ListaArestas *lista){

    FILE *arquivo = fopen("GrafoArestas.csv", "wt");
    Aresta *aux = lista->primeira;

    fprintf(arquivo, "%d\n", lista->quantidade);
    int i =0;
    while(i < lista->quantidade){
        fprintf(arquivo, "%d, %d\n", aux->vertice1, aux->vertice2);
        aux = aux->prox;
        i++;
    }

    fclose(arquivo);
}

void toStringFlores(Flor *flores){
    for(int i = 0; i < 150; i++){
        printf("id:%d\n%.3f %.3f %.3f %.3f %d\n", flores[i].id, flores[i].sepal_lenght, flores[i].sepal_widht, flores[i].petal_lenght, flores[i].petal_widht, flores[i].type);
    }
}

void matrizConfusao(ListaArestas *listaArestas){

    int **matriz;
    Aresta *lista = listaArestas->primeira;
    
    int tp = 0;
    
    for(int i = 0; i <= listaArestas->quantidade; i++){
        if(lista->type1 == lista->type2){
            tp++;
        }
        if(lista->prox == NULL){
            break;
        }
        lista = lista->prox;
    }
    printf("%d\n", tp);

}