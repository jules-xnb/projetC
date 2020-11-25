#define TAILLE_MAX 100

int checkXMLVersion(FILE* f);
char* getElementXML(FILE* xml);
int checkFinElementXML(FILE* xml, int nbElementDTD, char** tabNameElement);
void lireContenuElementXML(FILE* xml);
void lireElementDansXML(FILE* xml, int nbElementDTD, char** tabNameElement, char** tabAttributElement, int profondeur);
void lireXML(char* nomFichierXML, char** tabNameElement, char** tabAttributElement, int nbElementDTD);