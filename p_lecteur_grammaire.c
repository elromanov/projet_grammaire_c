#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "p_lecteur_grammaire.h"

char currentWord[TAILLE_MAX_TEXTE];
char chevronActuel[TAILLE_MAX_TEXTE];
int iteratorMot = 0;
int iterorChevron = 0;
char myChar;
int chevronTrouve = 0;
int nbChevrons = 0;
FILE* file;

void lire_char(){
    myChar = fgetc(file);
}

void passer_espaces(){
    while(myChar == ' ' || myChar == '\t' || myChar == '\n' || myChar == '\r'){
        lire_char();
    }
}

void vider_mot(){
    for(int i = 0; i < TAILLE_MAX_TEXTE; i++){
        currentWord[i] = '\0';
    }
}

void vider_chevron(){
    for(int i = 0; i < TAILLE_MAX_TEXTE ; i++){
        chevronActuel[i] = '\0';
    }
}

void supprimer_espaces_mot(){
    int lettreTrouvee = 0;
    for(int i = strlen(currentWord); i > 0; i--){
        if(lettreTrouvee == 0){
            if((currentWord[i] == ' ' || currentWord[i] == '\t' || currentWord[i] == '\n' || currentWord[i] == '\r') && (currentWord[i-1] == ' ' || currentWord[i-1] == '\t' || currentWord[i-1] == '\n' || currentWord[i-1] == '\r')){
                // printf("'%c'\n", currentWord[i]);
                currentWord[i] = '\0';
            } else {
                if(currentWord[i] == ' ' || currentWord[i] == '\t' || currentWord[i] == '\n' || currentWord[i] == '\r'){
                    currentWord[i] = '\0';
                    lettreTrouvee = 1;
                }
            }
        }
    }
}

void consommerMot(){
    if(myChar == '<'){
        nbChevrons++;
        supprimer_espaces_mot();
        if(strlen(currentWord) > 0){
            printf("\"%s\"\n", currentWord);
        }
        vider_mot();
        iteratorMot = 0;
        chevronTrouve = 1;
    } else {
        if(iteratorMot == 0){
            if(myChar != ' ' && myChar != '\n' && myChar != '\t' && myChar != '\r'){
                currentWord[iteratorMot] = myChar;
                iteratorMot++;
            }
        } else {
            currentWord[iteratorMot] = myChar;
            iteratorMot++;
        }
    }
}

void consommerChevron(){
    lire_char();
    if(myChar != '>'){
        chevronActuel[iterorChevron] = myChar;
        iterorChevron++;
        if(myChar == '/'){
            nbChevrons -= 2;
        }
    } else {
        iterorChevron = 0;
        chevronTrouve = 0;
        printf("%s\n", chevronActuel);
        vider_chevron();
    }
}

void consommerChar(){
    if(chevronTrouve == 0){
        lire_char();
        consommerMot();
    } else {
        consommerChevron();
    }
}

void commencer_lecture(){
    while(myChar != EOF){
        consommerChar();
        // test();
    }
    printf("%d", nbChevrons);
}

int main(int argc, char** argv){
    if(argc > 1){
        file = fopen(argv[1], "r");
        if(!file){
            fprintf(stderr, "Fichier: %s introuvable !\n", argv[1]);
        } else {
            commencer_lecture();
        }
    } else {
        fprintf(stderr, "Il manque un fichier\n");
    }
    return 0;
}