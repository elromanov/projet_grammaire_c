#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "p_texte_enrichi.h"

// #define LONGUEUR_LIGNE 80

int mode_ecriture = NORMAL;
int nb_boites = 0;
int nb_indentations = 0;

int longueur_ligne = 80;
int nb_char_ligne = 0;

int is_pucer = 0;

void changer_mode(t_mode mode){
    mode_ecriture = mode;
}

t_mode mode(){
    return mode_ecriture;
}

void ouvrir_bloc(){
    entamer_ligne();
    printf("+");
    int nb_chars = longueur_ligne - 2 - (2*nb_boites);
    for(int i = 0; i < nb_chars; i++){
        printf("-");
        if(nb_boites > 0){
            nb_char_ligne ++;
        }
    }
    if(nb_boites > 0){
        nb_char_ligne+=2;
        printf("+");
        terminer_ligne();
    }else{
        printf("+\n");
    }
    nb_boites += 1;
}

void fermer_bloc(){
    nb_boites = nb_boites - 1;
    entamer_ligne();
    printf("+");
    int nb_chars = longueur_ligne - 2 - (2*nb_boites);
    for(int i = 0; i < nb_chars; i++){
        printf("-");
        if(nb_boites > 0){
            nb_char_ligne ++;
        }
    }
    if(nb_boites > 0){
        nb_char_ligne+=2;
        printf("+");
        terminer_ligne();
    }else{
        printf("+\n");
    }
}

void indenter(){
    nb_indentations++;
}

void desindenter(){
    if(nb_indentations >= 1){
        nb_indentations = nb_indentations - 1;
    }
}

int est_en_fin_de_ligne(); // ?

int est_au_debut_de_ligne(); // ?

void entamer_ligne(){
    nb_char_ligne = 0;
    for(int i = 0; i < nb_boites; i++){
        printf("|");
        nb_char_ligne++;
    }
}

void terminer_ligne(){
    int espaces_a_mettre = longueur_ligne - nb_char_ligne - nb_boites;
    for(int n = 0; n < espaces_a_mettre; n++){
        printf(" ");
    }

    for(int i = 0; i < nb_boites; i++){
        if(i != nb_boites - 1){
            printf("|");
        } else {
            printf("|\n");
        }
    }
    nb_char_ligne = 0;
}

void pucer(){
    if(is_pucer == 1){
        is_pucer = 0;
    } else {
        is_pucer = 1;
    }
}

void ecrire_mot(const char* mot){

    for(int i = 0; i < nb_indentations * 4; i++){
        printf(" ");
        nb_char_ligne++;
    }
    if(is_pucer == 1){
        printf("* ");   
        nb_char_ligne+=2;
    }

    // char mon_mot[(int)sizeof(mot)] = mot;

    // mode_ecriture = mode();
    // if(mode_ecriture == MAJUSCULE){
    //     for(int i = 0; i < (int)sizeof(mon_mot); i++){
    //         if(mon_mot[i] <= 'z' && mon_mot[i] >= 'a'){
    //             mon_mot[i] = mon_mot[i] - 32;
    //         }
    //     }
    // }

    // if(mode_ecriture == MINUSCULE){
    //     for(int i = 0; i < (int)sizeof(mon_mot); i++){
    //         if(mon_mot[i] <= 'Z' && mon_mot[i] >= 'A'){
    //             mon_mot[i] = mon_mot[i] + 32;
    //         }
    //     }
    // }
    
    nb_char_ligne += strlen(mot) + 1;
    printf("%s ", mot);
}

int peut_ecrire(int nb_caracteres); // ?