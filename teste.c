#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct Flor{
    int id;
    double sepal_lenght,sepal_widht, petal_lenght, petal_widht;
}Flor;

Flor *getFlores();
void toStringFlores(Flor* flores);

int main(int argc, char const *argv[]){

    Flor* flores = malloc(150 * sizeof(Flor*));
    flores = getFlores();
    toStringFlores(flores);

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
            }
            printf("%s\n", token);
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

void toStringFlores(Flor *flores){
    for(int i = 0; i < 150; i++){
        printf("id:%d\n%.3f %.3f %.3f %.3f\n", flores[i].id, flores[i].sepal_lenght, flores[i].sepal_widht, flores[i].petal_lenght, flores[i].petal_widht);
    }
}

