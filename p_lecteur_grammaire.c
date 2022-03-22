#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "p_lecteur_grammaire.h"
#include "p_nanodom.h"

char currentWord[TAILLE_MAX_TEXTE];
char chevronActuel[TAILLE_MAX_TEXTE];
int noeud_trouve = 0;
int iteratorMot = 0;
int iterorChevron = 0;
char myChar;
int chevronTrouve = 0;
int nbChevrons = 0;
p_noeud premier_noeud;
p_noeud noeud_courant;
FILE* file;

void affichage_aux(p_noeud noeud){
    if(noeud->les_parentes[GRAND_FRERE] == NULL){
        afficher_elabore(noeud);
    } else {
        affichage_aux(noeud->les_parentes[GRAND_FRERE]);
    }
}

void affichage(){
    if(premier_noeud->les_parentes[GRAND_FRERE] == NULL){
        afficher_elabore(premier_noeud);
    } else {
        affichage_aux(premier_noeud->les_parentes[GRAND_FRERE]);
    }
}

void faire_noeud(t_token typeNoeud, char* contenu){
    p_noeud noeud;
    creer_noeud(noeud, typeNoeud, NULL, NULL, NULL, NULL, NULL, NULL);

    if(noeud_trouve == 0){
        premier_noeud = noeud;
    } else {
        if(nbChevrons == 0){
            inserer_apres(premier_noeud, premier_noeud);
            premier_noeud = noeud;
        }
        if(nbChevrons == 1){
            if(premier_noeud->les_parentes[PREMIER_FILS] == NULL){
                inserer_aine(premier_noeud, noeud);
            } else if(premier_noeud->les_parentes[DERNIER_FILS] == NULL){
                inserer_cadet(premier_noeud, noeud);
            } else {
                inserer_cadet(premier_noeud, noeud);
            }
        } else {
            inserer_cadet(noeud_courant, noeud);
        }
    }
    noeud_courant = noeud;
}

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
            faire_noeud(MOT, currentWord);
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

void faire_noeud_chevron(){
        if(strcmp(chevronActuel, "document") == 0){
            faire_noeud(DOCUMENT, NULL);
        } else if(strcmp(chevronActuel, "annexe")){
            faire_noeud(ANNEXE, NULL);
        } else if(strcmp(chevronActuel, "section")){
            faire_noeud(SECTION, NULL);   
        } else if(strcmp(chevronActuel, "titre")){
            faire_noeud(TITRE, NULL);
        } else if(strcmp(chevronActuel, "liste")){
            faire_noeud(LISTE, NULL);
        } else if(strcmp(chevronActuel, "item")){
            faire_noeud(ITEM, NULL);
        } else if(strcmp(chevronActuel, "important")){
            faire_noeud(IMPORTANT, NULL);
        } else {
            faire_noeud(RETOUR_A_LA_LIGNE, NULL);
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
        faire_noeud_chevron();
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
    // printf("%d", nbChevrons);
    affichage();
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