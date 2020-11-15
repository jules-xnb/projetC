#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLE_MAX 100

void enleverEspaces(FILE* f){
    char c = fgetc(f) ;
    while (c == 9 || c == ' '){
        c = fgetc(f) ;
    }
    fseek(f, -1, SEEK_CUR) ;
}

int verifierDoctype(FILE* f){
    enleverEspaces(f);
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    fgets(buffer, 11, f);
    if (strcmp("<!DOCTYPE ",buffer)==0){
        printf("DOCTYPE CORRECT\n");
        free(buffer);
        return 0 ;
    } else {
        free(buffer);
        printf("DOCTYPE INCORRECT\n");
        return 1 ;
    }

}

char* getNomDTD(FILE* f){
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    int i = 0 ;
    char c = fgetc(f) ;
    int save = 0 ;
    while (c != '['){
        if (i == 100){
            printf("NOM TROP LONG OU CARACTERE '[' NON PRESENT");
            exit(1) ;
        }
        if(c == ' '){
            save ++ ;
            if (save > 1 ){
                printf("ERREUR : TROP D'ESPACES DANS LE NOM") ;
                exit(1);
            }
        }
        if (save < 1){
            buffer[i] = c ;
        }

        c = fgetc(f) ;
        i++ ;
    }
    return buffer ;
}

void sauterLigne(FILE* f){
    char c ;
    do {
        c = fgetc(f);
        if(c != ' ' && c != '\n'){
            printf("ERREUR : CARACTERE INNATENDU");
            exit(1);
        }
    } while (c != '\n') ;
}

int finDeDTD(FILE* f){
    char c = fgetc(f);
    if (c == ']'){
        c = fgetc(f);
        if (c == '>'){
            printf("FIN DE DTD");
            return 0 ;
        } else {
            printf("ERREUR : CARACTERE INNATENDU");
            exit(1);
            return 0 ;
        }
    } else {
        fseek(f,-1,SEEK_CUR);
        return 1 ;
    }
}



int detectionElement (FILE* f){
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    enleverEspaces(f);
    fgets(buffer, 11, f) ;
    if (strcmp("<!ELEMENT ", buffer)==0){
        printf("DETECTION D'UN ELEMENT\n") ;
        free(buffer) ;
        return 1 ;
    } else {
        free(buffer) ;
        printf("ELEMENT INCORRECT\n") ;
        return 0 ;
    }

}

void getNameElement(int i, char** tabNameElement, FILE* f){
    int j = 0 ;
    char c = fgetc(f);
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    if (c == ' '){
        printf("ERREUR : TROP D'ESPACES AVANT LE NOM DE L'ELEMENT");
        exit(1);
    }
    while (c != ' '){
        buffer[j] = c ;
        //printf("|%c|",c );
        c = fgetc(f);
        j++ ;
    }
    tabNameElement[i] = buffer ;
    printf("L'élément (%d) est : %s \n", i, tabNameElement[i]);

}

void getAttributElement (int i, char** tabAttributElement, FILE* f){
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    int j = 0 ;
    char c = fgetc(f);
    while (c != ')'){
        if (c == ' '){
            printf("ERREUR DE SYNTAXE DANS L'ATTRIBUT DE L'ELEMENT");
            exit(1);
        }
        buffer[j] = c ;
        c = fgetc(f);
        j++ ;
    }
    tabAttributElement[i] = buffer ;
    printf("L'attribut associé est : %s \n\n", tabAttributElement[i] );

}

int checkParenthese(FILE* f){
    char c = fgetc(f);
    if (c == '('){
            return 1 ;
        } else {
            return 0 ;
        }

}

void getElement(int i, char** tabNameElement, char** tabAttributElement, FILE* f){
    if (detectionElement(f) == 1 ){
        getNameElement(i,tabNameElement,f);
        if (checkParenthese(f) == 1){
            getAttributElement(i, tabAttributElement, f);
            if (fgetc(f) == '>'){
                sauterLigne(f);
            } else {
                printf("ERREUR DE FIN D'ELEMENT");
                exit(1);
            }
        } else {
            printf("ERREUR DE SYNTAXE DANS L'ELEMENT");
            exit(1);
        }

    } else {
        exit(1) ;
    }

}

int main()
{
    FILE* dtd = fopen("dtd1.dtd", "r+");
    if (dtd == NULL ){
        exit(1);
    }

    if(verifierDoctype(dtd)){
        exit(1);
    }
    char* nomDTD = getNomDTD(dtd);
    printf("Nom de la DTD : %s\n\n",nomDTD);

    sauterLigne(dtd);

    char** tabNameElement = malloc(sizeof(char*)*TAILLE_MAX);
    char** tabAttributElement = malloc(sizeof(char*)*TAILLE_MAX);
    int i = 0 ;

    while(finDeDTD(dtd) == 1 ){
        getElement(i, tabNameElement, tabAttributElement, dtd) ;
        i++ ;
    }

    fclose (dtd);
    return 0 ;
}
