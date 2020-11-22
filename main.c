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
        if (i == TAILLE_MAX){
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
            printf("FIN DE DTD\n\n");
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
        if (j < TAILLE_MAX){
            buffer[j] = c ;
            c = fgetc(f);
            j++ ;
        } else {
            printf("ERREUR : NOM ELEMENT TROP LONG");
            exit(1);
        }
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
        if (j < TAILLE_MAX){
            buffer[j] = c ;
            c = fgetc(f);
            j++ ;
        } else {
            printf("ERREUR : ATTRIBUT ELEMENT TROP LONG");
            exit(1);
        }
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
                printf("ERREUR DE FIN D'ELEMENT\n");
                exit(1);
            }
        } else {
            printf("ERREUR DE SYNTAXE DANS L'ELEMENT\n");
            exit(1);
        }

    } else {
        exit(1) ;
    }

}

int lireDTD(char* nomFichierDTD, char** tabNameElement, char** tabAttributElement){
    FILE* dtd = fopen(nomFichierDTD, "r+");
    if (dtd == NULL ){
        exit(1);
    }

    if(verifierDoctype(dtd)){
        exit(1);
    }
    char* nomDTD = getNomDTD(dtd);
    printf("Nom de la DTD : %s\n\n",nomDTD);

    sauterLigne(dtd);

    int i = 0 ;

    while(finDeDTD(dtd) == 1 ){
        getElement(i, tabNameElement, tabAttributElement, dtd) ;
        i++ ;
    }

    fclose (dtd);
    return i ;
}

int checkXMLVersion(FILE* f){
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    fgets(buffer, 22, f);
    if (strcmp("<?xml version=\"1.0\"?>", buffer) == 0){
        printf("XML VERSION CORRECT\n");
        free(buffer);
        return 1 ;
    } else {
        free(buffer);
        printf("XML VERSION INCORRECT\n");
        return 0 ;
    }

}

void checkElementInDTD(int nbElementDTD, char** tabNameElement, char* buffer ){
    int indexSave = -1 ;
    for(int j = 0 ; j < nbElementDTD ; j++){
        if (strcmp(buffer, tabNameElement[j]) == 0){
            printf("DETECTION DE L'ELEMENT %s", tabNameElement[j]);
            indexSave = j ;
            break ;
        }
    }
    if (indexSave == -1 ){
        printf("ERREUR : ELEMENT NON PRESENT DANS LA DTD\n");
        exit(1);
    }
}

char* getElementXML(FILE* xml){
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    char c = fgetc(xml);
    int i = 0 ;
    while (c != '>'){
        buffer[i] = c ;
        c = fgetc(xml);
        if (i == TAILLE_MAX){
            printf("ERREUR : '>' NON PRESENT OU NOM D'ELEMENT TROP GRAND");
            exit(1);
        }
        i++;
    }
    if (strcmp(buffer, "")==0){
        printf("ERREUR DANS DECLARATION D'ELEMENT\n");
        exit(1);
    }
    return buffer ;
}

void lireElementDansXML(FILE* xml, int nbElementDTD, char** tabNameElement){
    if (fgetc(xml) == '<'){
        char* nomElement = getElementXML(xml);
        checkElementInDTD(nbElementDTD, tabNameElement, nomElement);
    } else {
        exit(1);
     }
}


void lireXML(char* nomFichierXML, char** tabNameElement,
             char** tabAttributElement, int nbElementDTD){
    FILE* xml = fopen(nomFichierXML, "r+");
    if (xml == NULL ){
        exit(1);
    }
    if (checkXMLVersion(xml) == 1){
        sauterLigne(xml);
        enleverEspaces(xml);
        lireElementDansXML(xml, nbElementDTD, tabNameElement);


        }
}

int main()
{
    //DTD
    char* nomFichierDTD = "dtd1.dtd" ;
    char** tabNameElement = malloc(sizeof(char*)*TAILLE_MAX);
    char** tabAttributElement = malloc(sizeof(char*)*TAILLE_MAX);

    int nbElementDTD = lireDTD(nomFichierDTD, tabNameElement, tabAttributElement);

    //XML
    char* nomFichierXML = "fichierxml.xml" ;

    lireXML(nomFichierXML, tabNameElement, tabAttributElement, nbElementDTD);




    return 0 ;
}
