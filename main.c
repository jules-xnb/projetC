#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int verifierDoctype(FILE* f){
    char* buffer = malloc(sizeof(char)*100);
    fgets(buffer, 11, f);
    if (strcmp("<!DOCTYPE ",buffer)==0){
        printf("DOCTYPE CORRECT\n");
        free(buffer);
        return 0 ;
    } else {
        printf("%s", buffer);
        free(buffer);
        printf("DOCTYPE INCORRECT\n");
        return 1 ;
    }

}

char* getNomDTD(FILE* f){
    char* buffer = malloc(sizeof(char)*100);
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
    } while (c != '\n') ;
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
    printf("Nom de la DTD : %s\n",nomDTD);

    sauterLigne(dtd);
    char c = fgetc(dtd);
    printf("%c",c);


    fclose (dtd);
    return 0 ;
}
