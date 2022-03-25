#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "p_lecteur_grammaire.h"
#include "p_nanodom.h"
#include "p_texte_enrichi.h"

char currentWord[TAILLE_MAX_TEXTE];
char chevronActuel[TAILLE_MAX_TEXTE];
char myChar;

int premier_noeud_existe = 0;
int iteratorMot = 0;
int iterorChevron = 0;
int chevronTrouve = 0;
int nbChevrons = 0;
int balise_fermante = 0;

p_noeud premier_noeud;
p_noeud noeud_courant;
FILE* file;
FILE* mon_fichier;

void affichage_aux(p_noeud noeud){
    if(noeud->les_parentes[PERE] == NULL){
        if(noeud->les_parentes[GRAND_FRERE] == NULL){
            // debugger_noeud(noeud);
            afficher_elabore(noeud);
        } else {
            affichage_aux(noeud->les_parentes[GRAND_FRERE]);
        }
    } else {
        affichage_aux(noeud->les_parentes[PERE]);
    }
}

void affichage(){
    if(premier_noeud->les_parentes[GRAND_FRERE] == NULL){
        if(premier_noeud->les_parentes[PERE] == NULL){
            afficher_elabore(premier_noeud);
        } else {
            affichage_aux(premier_noeud->les_parentes[PERE]);
        }
    } else {
        affichage_aux(premier_noeud->les_parentes[GRAND_FRERE]);
    }
}

void inserer_dernier(p_noeud noeud, p_noeud orphelin, int interateur){
    printf("%d", interateur);
    debugger_noeud(noeud);
    if(interateur == 0){
        if(noeud->les_parentes[PREMIER_FILS] == NULL){
            inserer_aine(noeud, orphelin);
        } else {
            inserer_cadet(noeud, orphelin);
        }
    } else {
        inserer_dernier(noeud->les_parentes[DERNIER_FILS], orphelin, interateur-1);
    }
}

void faire_noeud(t_token typeNoeud, char* contenu){
    p_noeud noeud = NULL;
    creer_noeud(&noeud, typeNoeud, contenu, NULL, NULL, NULL, NULL, NULL);
    if(premier_noeud_existe == 0){
        premier_noeud = noeud;
        premier_noeud_existe = 1;
    } else {
        if(nbChevrons == 1){
            inserer_apres(premier_noeud, noeud);
            premier_noeud = noeud;
        } else {
            p_noeud noeud_temp = premier_noeud;
            for(int i = 0; i < nbChevrons - 2; i++){
                noeud_temp = noeud_temp->les_parentes[DERNIER_FILS];
            }
            inserer_cadet(noeud_temp, noeud);
        }
    }
    noeud_courant = noeud;
}

void faire_noeud_mot(char* contenu){
    char* content = malloc(sizeof(char)*(strlen(contenu)+1));
    strcpy(content,contenu);
    p_noeud nouveau_noeud = NULL;
    creer_noeud(&nouveau_noeud, MOT,content, NULL, NULL, NULL, NULL, NULL);
    inserer_cadet(noeud_courant, nouveau_noeud);
}

void lire_char(){
    myChar = fgetc(file);
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
        balise_fermante = 0;
        supprimer_espaces_mot();
        if(strlen(currentWord) > 0){
            faire_noeud_mot(currentWord);
        }
        vider_mot();
        iteratorMot = 0;
        chevronTrouve = 1;
        nbChevrons++;
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
        } else if(strcmp(chevronActuel, "annexe") == 0){
            faire_noeud(ANNEXE, NULL);
        } else if(strcmp(chevronActuel, "section") == 0){
            faire_noeud(SECTION, NULL);   
        } else if(strcmp(chevronActuel, "titre") == 0){
            faire_noeud(TITRE, NULL);
        } else if(strcmp(chevronActuel, "liste") == 0){
            faire_noeud(LISTE, NULL);
        } else if(strcmp(chevronActuel, "item") == 0){
            faire_noeud(ITEM, NULL);
        } else if(strcmp(chevronActuel, "important") == 0){
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
            balise_fermante = 1;
            nbChevrons -= 2;
        }
    } else {
        iterorChevron = 0;
        chevronTrouve = 0;
        if(balise_fermante == 0){
            faire_noeud_chevron();
        }
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

void sauvegarde(p_noeud noeud){
    if(noeud->les_parentes[PERE] == NULL){
        if(noeud->les_parentes[GRAND_FRERE] == NULL){
            sauvegarder_elabore(noeud);
        } else {
            sauvegarde(noeud->les_parentes[GRAND_FRERE]);
        }
    } else {
        sauvegarde(noeud->les_parentes[PERE]);
    }
}

void commencer_lecture(){
    while(myChar != EOF){
        consommerChar();
    }
    mon_fichier = fopen("save.txt","w");
    affichage();
    charger_fichier(mon_fichier);
    sauvegarde(premier_noeud);
    fclose(mon_fichier);
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