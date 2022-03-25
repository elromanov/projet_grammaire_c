#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_MAX_TEXTE 1024

/**
 * Cette fonction permet d'afficher l'arbre binaire sous la forme de texte enrichi. Elle utilise une fonction creee auparavant dans le fichier p_nanodom.c
*/
void affichage();

/**
 * Cette fonction permet d'inserer un noeud a la derniere position du noeud correspondant à la balise dans laquelle nous sommes. Elle utilise une fonction creee auparavant dans le fichier p_nanodom.c
 * @param noeud :: le noeud dans lequel on veut en inserer un autre
 * @param orphelin :: le noeud a inserer
 * @param iterateur :: un iterateur qui permet de determiner a quelle distance de l'emplacement du nouveau noeud nous sommes.
*/
void inserer_dernier(p_noeud noeud, p_noeud orphelin, int interateur);

/**
 * Cette fonction permet de generer un noeud en utilisant la fonction creer noeud de p_nanodom.c
 * @param typeNoeud :: le type de noeud (token de la balise qui est lue)
 * @param contenu :: le texte du token (la valeur est toujours NULL, sauf pour les mots)
*/
void faire_noeud(t_token typeNoeud, char* contenu);

/**
 * Cette fonction permet de generer un noeud pour un mot.
 * @param contenu :: le mot a ajouter dans l'arbre
*/
void faire_noeud_mot(char* contenu);

/**
 * Cette fonction permet de lire le caractere courrant du fichier donne en entree
*/
void lire_char();

/**
 * Cette fonction permet de reinitialiser la chaine de caractere correspondant au mot courant
*/
void vider_mot();

/**
 * Cette fonction permet de reinitialiser la chaine de caractere correspondant au token de la balise courante
*/
void vider_chevron();

/**
 * Cette fonction permet de supprimer tous les caracteres inutiles (\n, \r, \t, espaces...) qui sont presents apres le mot courant
*/
void supprimer_espaces_mot();

/**
 * Cette fonction permet d'une part d'ajouter le caractere courant a la chaine de caractere correspondant au mot courant, mais aussi a detecter l'ouverture d'une balise, ainsi qu'ordonner la creation d'un noeud pour le mot courrant, et vider ce dernier.
*/
void consommerMot();

/**
 * Cette fonction sert a ordonner la creation d'un noeud avec le token correspondant a la balise lue.
*/
void faire_noeud_chevron();

/**
 * Cette fonction sert a ajouter le caractere courant a la chaine correspondant a la balise ouverte. Elle permet aussi de detecter la fin de
 * cette balise grace au caractere '/', ainsi d'ordonner la creation du noeud.
*/
void consommerChevron();

/**
 * Cette fonction sert a determiner s'il faut appeler la fonction permettant de consommer un chevron ou un mot.
*/
void consommerChar();

/**
 * Cette fonction sert a lancer la sauvegarde correspondant a l'affichage de l'arbre en appelant des fonctions definies dans p_texte_enrichi.c
 * @param noeud :: correspond a un noeud present dans l'arbre a afficher
*/
void sauvegarde(p_noeud noeud);

/**
 * Cette fonction permet d'ordonner la lecture du fichier, ainsi que d'ordonner l'affichage de l'arbre, et sa sauvegarde.
*/
void commencer_lecture();

/**
 * @param argv :: fichier d'entree, necessaire pour la lecture de ce dernier.
*/
int main(int argc, char** argv);