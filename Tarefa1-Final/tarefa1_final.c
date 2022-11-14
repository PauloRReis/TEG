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

typedef struct Aresta{
    int vertice1, vertice2;
    struct Aresta *prox;
}Aresta;
typedef struct ListaArestas{
    Aresta *primeira;
    int quantidade;
}ListaArestas;

Flor *getFlores();
ListaArestas* resolveEnormaliza(Flor* flores);
double** criaMatriz();
void matrizConfusao(ListaArestas *listaArestas, Flor *flores);
int **matrizAdj(ListaArestas *lista);
void arquivoGrafo(ListaArestas *lista);

int main(int argc, char const *argv[]){

    Flor* flores;
    ListaArestas *listaArestas;

    //consome o arquivo .csv retornando uma lista das flores
    flores = getFlores();
    //recebe a lista de flores, resolve a distacia euclidiana, normaliza e forma uma lista com as distancias euclidianas menores que 0,3
    listaArestas = resolveEnormaliza(flores);
    //constroi o arquivo do grafo .csv
    arquivoGrafo(listaArestas);
    //constroi a matriz de adjacencia da lista de arestas, calcula a matriz de confusão e a acuracia
    matrizConfusao(listaArestas, flores);

    return 0;
}

Flor *getFlores(){

    Flor *flores = malloc(150 * sizeof(Flor));
    FILE *arq = fopen("IrisDataset.csv", "rt");
    char *result;
    char *token;
    char string[255];
    char* lixo = (char*) malloc(sizeof(char) * 100);
	fgets(lixo, 100, arq);
	free(lixo);

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
    
    return flores;
}

ListaArestas* resolveEnormaliza(Flor* flores){

    double** matrizEuclidiana = criaMatriz();
    int i,j;
    double max = 0;
    double min = 1;

    for(i = 0; i < 150 ; i++){
        for(j = 0; j < 150; j++){
            if(i != j){
                double euclidiana = sqrt(pow(flores[i].sepal_lenght - flores[j].sepal_lenght, 2) + pow(flores[i].sepal_widht - flores[j].sepal_widht,2) + pow(flores[i].petal_lenght - flores[j].petal_lenght,2) + pow(flores[i].petal_widht - flores[j].petal_widht , 2));
                matrizEuclidiana[i][j] = euclidiana;

                if(euclidiana > max){
                    max = euclidiana;
                }
                if(euclidiana < min){
                    min = euclidiana;
                }
            }
        }
    }

    for(i = 0; i < 150; i++){
        for(j= 0 ; j < 150; j++){
            if(i != j){
                matrizEuclidiana[i][j] = (matrizEuclidiana[i][j] - min)/(max - min);
            }
        }
    }

    ListaArestas *lista = malloc(sizeof(ListaArestas));
    Aresta *aresta;
    lista->quantidade = 0;
    for(i = 0; i < 150; i++){
        for ( j = 0; j < 150; j++){
            if(matrizEuclidiana[i][j] <= 0.3 && i != j && i < j){
                lista->quantidade++;

                if(lista->quantidade == 1){
                    lista->primeira = malloc(sizeof(Aresta));
                    aresta = lista->primeira;
                }else{
                    aresta->prox = malloc(sizeof(Aresta));
                    aresta = aresta->prox;
                }

                aresta->vertice1 = i;
                aresta->vertice2 = j;
            }
        } 
    }

    return lista;
}

double** criaMatriz(){

    double **euclidiana;
    int i,j;
    euclidiana = (double **)malloc(150 * sizeof(double*));
    
    for(i = 0; i < 150; i++){
        euclidiana[i] = (double *)malloc(150*sizeof(double));
    }

    for(i = 0; i < 150; i++){
		for(j = 0; j < 150; j++){
			euclidiana[i][j] = 0;
        }
	}

    return euclidiana;
}

void arquivoGrafo(ListaArestas *lista){

    FILE *arquivo = fopen("GrafoArestas.csv", "wt");
    Aresta *aux = lista->primeira;

    fprintf(arquivo, "%d\n", lista->quantidade);
    int k = 0;
    while(k < lista->quantidade){
        fprintf(arquivo, "%d, %d\n", aux->vertice1, aux->vertice2);
        aux = aux->prox;
        k++;
    }

    fclose(arquivo);
}

int **matrizAdj(ListaArestas *lista){

    int **mat;
    int i, j;
    Aresta *aux = lista->primeira;
    mat = (int **)malloc(150 * sizeof(int *));

    for(i = 0; i < 150; i++){
        mat[i] = (int*)malloc(150*sizeof(int));
    }
    
    for(i = 0; i < 150; i++){
		for(j = 0; j < 150; j++){
			mat[i][j] = 0;
        }
	}

    for(i = 0 ; i < lista->quantidade ; i++){
        int a = aux->vertice1;
        int b = aux->vertice2;
        mat[a][b] = 1;
        aux = aux->prox;
    }

    return mat;
}

void matrizConfusao(ListaArestas *listaArestas, Flor *flores){

    int tp = 0;
    int tn = 0;
    int fp = 0;
    int fn = 0;
    int **mat = matrizAdj(listaArestas);
    int i, j;
    
    for(i = 0 ; i < 50; i++){
        for(int j = 0; j < 50; j++){
            if(mat[i][j] == 1){
                tp++;
            }
        }
    }

    for(i = 0 ; i < 50; i++){
        for(j = 50; j < 150; j++){
            if(mat[i][j] == 1){
                fp++;
            }
        }
    }

    for(i = 50 ; i < 150; i++){
        for(j = 50; j < 150; j++){
            if(mat[i][j] == 1){
                tn++;
            }
        }
    }

    for(i = 50 ; i < 150; i++){
        for(j = 0; j < 50; j++){
            if(mat[i][j] == 1){
                fn++;
            }
        }
    }

    double acuracia = (float)(tp+tn) / (tp+fp+tn+fn);

    printf("Acuracia: %f\n", acuracia*100);
    printf("\n           Valores preditos\n");
    printf("Valores reais| %d | %d  |\n", tp, fp);
    printf("Valores reais| %d  | %d |\n", fn, tn);

}