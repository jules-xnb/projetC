#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xmlFunctions.h"
#include "verifyDtd.h"

int checkXMLVersion(FILE* f){
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    fgets(buffer, 22, f);
    if (strcmp("<?xml version=\"1.0\"?>", buffer) == 0){
        printf("XML VERSION CORRECT\n\n");
        free(buffer);
        return 1 ;
    } else {
        free(buffer);
        printf("XML VERSION INCORRECT\n");
        return 0 ;
    }

}

char* getElementXML(FILE* xml){
    char* buffer = lectureFinElementDTD(xml);
    if (strcmp(buffer, "")==0){
        printf("ERREUR DANS DECLARATION D'ELEMENT\n");
        exit(1);
    }
    return buffer ;
}

int checkFinElementXML(FILE* xml, int nbElementDTD, char** tabNameElement){
    char c = fgetc(xml);
    if (c == '/'){
        char* buffer = lectureFinElementDTD(xml);
        checkElementInDTD(nbElementDTD,tabNameElement, buffer, 1);
        free(buffer);
        return 1 ;
    } else {
        fseek(xml, -1, SEEK_CUR);
        return 0 ;
    }
}

void lireContenuElementXML(FILE* xml){
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    char c = fgetc(xml);
    int i = 0 ;
    while (c != '<'){
        buffer[i] = c ;
        c = fgetc(xml);
        if (i == TAILLE_MAX){
            printf("ERREUR : '<' NON PRESENT OU NOM D'ELEMENT TROP GRAND\n");
            exit(1);
        }
        i++;
    }
    buffer[i]='\0' ;
    printf("L'ELEMENT CONTIENT %s\n", buffer);
    free(buffer);
}

void lireElementDansXML(FILE* xml, int nbElementDTD, char** tabNameElement, char** tabAttributElement, int profondeur){
    sauterLigne(xml);
    enleverEspaces(xml);
    if (fgetc(xml) == '<'){
        if (checkFinElementXML(xml, nbElementDTD, tabNameElement) == 1 && profondeur == 0){
            return ;
        }
        char* nomElement = getElementXML(xml);

        int indexElement = checkElementInDTD(nbElementDTD, tabNameElement, nomElement, 0);

        if(strcmp(tabAttributElement[indexElement], "#PCDATA" ) == 0){
            lireContenuElementXML(xml);
            if (checkFinElementXML(xml, nbElementDTD, tabNameElement)==1){
                //printf("FIN D'ELEMENT\n");
                lireElementDansXML(xml, nbElementDTD,tabNameElement, tabAttributElement, profondeur--);
            } else {
                printf("ERREUR DE FIN D'ELEMENT\n");
                exit(1);
            }

        } else {
            int n = checkFinElementDTD(indexElement, tabAttributElement);
            if (n == 1){
                free(nomElement);
                lireElementDansXML(xml, nbElementDTD,tabNameElement, tabAttributElement, profondeur++);
            }
        }
    }
}

void lireXML(char* nomFichierXML, char** tabNameElement, char** tabAttributElement, int nbElementDTD){
    FILE* xml = fopen(nomFichierXML, "r+");
    if (xml == NULL ){
        exit(1);
    }
    if (checkXMLVersion(xml) == 1){
        lireElementDansXML(xml, nbElementDTD, tabNameElement, tabAttributElement, 0);
    }
}