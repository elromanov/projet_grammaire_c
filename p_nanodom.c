#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "p_nanodom.h"
#include "p_texte_enrichi.h"

const char* t_token_image(t_token ceci){
    char* image_token[] = {"DOCUMENT", "ANNEXE", "SECTION", "TITRE", "LISTE", "ITEM", "IMPORTANT", "RETOUR_A_LA_LIGNE", "MOT"};
    return image_token[ceci];
}

const char* t_parente_image(t_parente ceci){
    char* image_parente[] = {"PERE", "PREMIER_FILS", "DERNIER_FILS", "GRAND_FRERE", "PETIT_FRERE"};
    return image_parente[ceci];
}

void creer_noeud(
    p_noeud* ceci,
    t_token etiquette,
    const char* contenu,
    p_noeud pere,
    p_noeud premier_fils,
    p_noeud dernier_fils,
    p_noeud grand_frere,
    p_noeud petit_frere){
        (*ceci) = (t_noeud*)malloc(sizeof(t_noeud));

        (*ceci)->l_etiquette = etiquette;
        (*ceci)->le_contenu = contenu;

        (*ceci)->les_parentes[PERE] = pere;
        (*ceci)->les_parentes[PREMIER_FILS] = premier_fils;
        
        (*ceci)->les_parentes[DERNIER_FILS] = dernier_fils;
        (*ceci)->les_parentes[GRAND_FRERE] = grand_frere;
        (*ceci)->les_parentes[PETIT_FRERE] = petit_frere;
}

void detruire_noeud(p_noeud* ceci){
    free(*ceci);
}

void modifier_etiquette_noeud(p_noeud ceci, t_token nouvelle_etiquette){
    ceci->l_etiquette = nouvelle_etiquette;
}

void modifier_contenu_noeud(p_noeud ceci, char* nouveau_contenu){
    ceci->le_contenu = nouveau_contenu;
}

void modifier_parente_noeud(p_noeud ceci, t_parente lien_de_parente, p_noeud nouveau_parent){
    if(ceci != NULL){
        ceci->les_parentes[lien_de_parente] = nouveau_parent;
    }
}

void debugger_noeud(p_noeud ceci){
    printf("Noeud range a l'adresse %p\n", ceci);
    printf("\t- ETIQUETTE : %s\n", t_token_image(ceci->l_etiquette));
    printf("\t- CONTENU : %s\n", ceci->le_contenu);
    printf("\t- PERE : %p\n", ceci->les_parentes[PERE]);
    printf("\t- PREMIER_FILS : %p\n",ceci->les_parentes[PREMIER_FILS]);
    printf("\t- DERNIER_FILS : %p\n",ceci->les_parentes[DERNIER_FILS]);
    printf("\t- GRAND_FRERE : %p\n",ceci->les_parentes[GRAND_FRERE]);
    printf("\t- PETIT_FRERE : %p\n",ceci->les_parentes[PETIT_FRERE]);
}

void inserer_aine(p_noeud ceci, p_noeud orphelin){
    if(ceci->les_parentes[PREMIER_FILS] == NULL){
        modifier_parente_noeud(ceci, PREMIER_FILS, orphelin);
        modifier_parente_noeud(orphelin, PERE, ceci);
        if(ceci->les_parentes[DERNIER_FILS] == NULL){
            modifier_parente_noeud(ceci, DERNIER_FILS, orphelin);
        } else {
            modifier_parente_noeud(orphelin, PETIT_FRERE, ceci->les_parentes[DERNIER_FILS]);
        }
    } else {
        modifier_parente_noeud(orphelin, PERE, ceci);
        modifier_parente_noeud(orphelin, PETIT_FRERE, ceci->les_parentes[PREMIER_FILS]);
        modifier_parente_noeud(ceci->les_parentes[PREMIER_FILS], GRAND_FRERE, orphelin);
        modifier_parente_noeud(ceci, PREMIER_FILS, orphelin);
    }
}

void inserer_cadet(p_noeud ceci, p_noeud orphelin){
    if(ceci->les_parentes[DERNIER_FILS] == NULL){
        modifier_parente_noeud(ceci, DERNIER_FILS, orphelin);
        modifier_parente_noeud(orphelin, PERE, ceci);
        if(ceci->les_parentes[PREMIER_FILS] == NULL){
            modifier_parente_noeud(ceci, PREMIER_FILS, orphelin);
        } else {
            modifier_parente_noeud(orphelin, GRAND_FRERE, ceci->les_parentes[PREMIER_FILS]);
        }
    } else {
        modifier_parente_noeud(orphelin, PERE, ceci);
        modifier_parente_noeud(orphelin, GRAND_FRERE, ceci->les_parentes[DERNIER_FILS]);
        modifier_parente_noeud(ceci->les_parentes[DERNIER_FILS], PETIT_FRERE, orphelin);
        modifier_parente_noeud(ceci, DERNIER_FILS, orphelin);
    }
}

void inserer_apres(p_noeud ceci, p_noeud orphelin){
    if(ceci == ceci->les_parentes[DERNIER_FILS]){
        inserer_cadet(ceci->les_parentes[PERE], orphelin);
    } else {
        modifier_parente_noeud(ceci->les_parentes[PETIT_FRERE], GRAND_FRERE, orphelin);
        modifier_parente_noeud(orphelin, PETIT_FRERE, ceci->les_parentes[PETIT_FRERE]);
        modifier_parente_noeud(orphelin, PERE, ceci->les_parentes[PERE]);
        modifier_parente_noeud(orphelin, GRAND_FRERE, ceci);
        modifier_parente_noeud(ceci, PETIT_FRERE, orphelin);
    }
}

void inserer_avant(p_noeud ceci, p_noeud orphelin){
        if(ceci == ceci->les_parentes[PERE]->les_parentes[PREMIER_FILS]){
            inserer_aine(ceci->les_parentes[PERE], orphelin);
        } else {
            modifier_parente_noeud(ceci->les_parentes[GRAND_FRERE], PETIT_FRERE, orphelin);
            modifier_parente_noeud(orphelin, GRAND_FRERE, ceci->les_parentes[GRAND_FRERE]);
            modifier_parente_noeud(orphelin, PERE, ceci->les_parentes[PERE]);
            modifier_parente_noeud(orphelin, PETIT_FRERE, ceci);
            modifier_parente_noeud(ceci, GRAND_FRERE, orphelin);
        }
}

void extraire(p_noeud ceci){
    if(ceci == ceci->les_parentes[PERE]->les_parentes[PREMIER_FILS]){
        modifier_parente_noeud(ceci->les_parentes[PERE], PREMIER_FILS, ceci->les_parentes[PETIT_FRERE]);
        modifier_parente_noeud(ceci, PERE, NULL);
        modifier_parente_noeud(ceci, PETIT_FRERE, NULL);
    } else if(ceci == ceci->les_parentes[PERE]->les_parentes[DERNIER_FILS]){
        modifier_parente_noeud(ceci->les_parentes[PERE], DERNIER_FILS, ceci->les_parentes[GRAND_FRERE]);
        modifier_parente_noeud(ceci, PERE, NULL);
        modifier_parente_noeud(ceci, GRAND_FRERE, NULL);
    } else {
        modifier_parente_noeud(ceci->les_parentes[GRAND_FRERE], PETIT_FRERE, ceci->les_parentes[PETIT_FRERE]);
        modifier_parente_noeud(ceci->les_parentes[PETIT_FRERE], GRAND_FRERE, ceci->les_parentes[GRAND_FRERE]);
        modifier_parente_noeud(ceci, GRAND_FRERE, NULL);
        modifier_parente_noeud(ceci, PETIT_FRERE, NULL);
        modifier_parente_noeud(ceci, PERE, NULL);
    }
}

void afficher_espaces(int nb_espaces){
    for(int i = 0; i < nb_espaces; i++){
        printf(" ");
    }
}

void affichage_enrichi_aux(t_arbre_nanodom ceci, int decalage){
    if(ceci != NULL){
        if(ceci->les_parentes[GRAND_FRERE] != NULL){
            if(ceci->les_parentes[GRAND_FRERE]->l_etiquette == MOT && ceci->l_etiquette == MOT){
                afficher_espaces(1);
            } else {
                afficher_espaces(decalage*4);
            }
        } else {
            afficher_espaces(decalage*4);
        }
        if(ceci->l_etiquette != MOT){
             printf("<%s>\n", t_token_image(ceci->l_etiquette));
        } else {
            printf("%s", ceci->le_contenu);
            if(ceci->les_parentes[PETIT_FRERE] != NULL){
                if(ceci->les_parentes[PETIT_FRERE]->l_etiquette != MOT){
                    printf("\n");
                }
            } else {
                printf("\n");
            }
        }
        if(ceci->les_parentes[PREMIER_FILS]){
            affichage_enrichi_aux(ceci->les_parentes[PREMIER_FILS], decalage+1);
        } else {
            affichage_enrichi_aux(ceci->les_parentes[DERNIER_FILS], decalage+1);
        }
        if(ceci->l_etiquette != MOT){
            afficher_espaces(decalage*4);
            printf("</%s>\n", t_token_image(ceci->l_etiquette));
        }
        affichage_enrichi_aux(ceci->les_parentes[PETIT_FRERE], decalage);
    }
}

void afficher_enrichi(t_arbre_nanodom ceci){
    affichage_enrichi_aux(ceci, 0);
}

void sauvegarder_enrichi_aux(t_arbre_nanodom ceci, int decalage, FILE* file){
    if(ceci != NULL){
        if(ceci->les_parentes[GRAND_FRERE] != NULL){
            if(ceci->les_parentes[GRAND_FRERE]->l_etiquette == MOT && ceci->l_etiquette == MOT){
                fprintf(file, " ");
            } else {
                for(int i = 0; i < decalage * 4; i++){
                    fprintf(file, " ");
                }
            }
        } else {
            for(int i = 0; i < decalage * 4; i++){
                fprintf(file, " ");
            }
        }
        if(ceci->l_etiquette != MOT){
             fprintf(file ,"<%s>\n", t_token_image(ceci->l_etiquette));
        } else {
            fprintf(file, "%s", ceci->le_contenu);
            if(ceci->les_parentes[PETIT_FRERE] != NULL){
                if(ceci->les_parentes[PETIT_FRERE]->l_etiquette != MOT){
                    fprintf(file, "\n");
                }
            } else {
                fprintf(file, "\n");
            }
        }
        if(ceci->les_parentes[PREMIER_FILS]){
            sauvegarder_enrichi_aux(ceci->les_parentes[PREMIER_FILS], decalage+1, file);
        } else {
            sauvegarder_enrichi_aux(ceci->les_parentes[DERNIER_FILS], decalage+1, file);
        }
        if(ceci->l_etiquette != MOT){
            for(int i = 0; i < decalage * 4; i++){
                fprintf(file, " ");
            }
            fprintf(file, "</%s>\n", t_token_image(ceci->l_etiquette));
        }
        sauvegarder_enrichi_aux(ceci->les_parentes[PETIT_FRERE], decalage, file);
    }
}


void sauvegarder_enrichi(t_arbre_nanodom ceci, FILE* fichier){
    sauvegarder_enrichi_aux(ceci, 0, fichier);
}

void afficher_elabore(t_arbre_nanodom ceci){
    if(ceci != NULL){
        if(ceci->l_etiquette == TITRE || ceci->l_etiquette == ITEM || ceci->l_etiquette == IMPORTANT){
            if(ceci->les_parentes[PREMIER_FILS] != NULL){
                afficher_elabore(ceci->les_parentes[PREMIER_FILS]);
            }
            if(ceci->les_parentes[PETIT_FRERE] != NULL){
                afficher_elabore(ceci->les_parentes[PETIT_FRERE]);
            }
        } else if(ceci->l_etiquette == MOT ){
            if(ceci->les_parentes[GRAND_FRERE] == NULL){
                entamer_ligne();
            } else {
                if(ceci->les_parentes[GRAND_FRERE]->l_etiquette != MOT){
                    entamer_ligne();
                }
            }
            ecrire_mot(ceci->le_contenu);
            if(ceci->les_parentes[PETIT_FRERE] == NULL){
                terminer_ligne();
            } else {
                if(ceci->les_parentes[PETIT_FRERE]->l_etiquette != MOT){
                    terminer_ligne();
                }
                afficher_elabore(ceci->les_parentes[PETIT_FRERE]);
            }
        } else {
            ouvrir_bloc();
            if(ceci->les_parentes[PREMIER_FILS] != NULL){
                afficher_elabore(ceci->les_parentes[PREMIER_FILS]);
            }
            fermer_bloc();

            if(ceci->les_parentes[PETIT_FRERE] != NULL){
                afficher_elabore(ceci->les_parentes[PETIT_FRERE]);
            }
        }
    }
}

void sauvegarder_elabore(t_arbre_nanodom ceci){
    if(ceci != NULL){
        if(ceci->l_etiquette == TITRE || ceci->l_etiquette == ITEM || ceci->l_etiquette == IMPORTANT){
            if(ceci->les_parentes[PREMIER_FILS] != NULL){
                sauvegarder_elabore(ceci->les_parentes[PREMIER_FILS]);
            }
            if(ceci->les_parentes[PETIT_FRERE] != NULL){
                sauvegarder_elabore(ceci->les_parentes[PETIT_FRERE]);
            }
        } else if(ceci->l_etiquette == MOT ){
            if(ceci->les_parentes[GRAND_FRERE] == NULL){
                s_entamer_ligne();
            } else {
                if(ceci->les_parentes[GRAND_FRERE]->l_etiquette != MOT){
                    s_entamer_ligne();
                }
            }
            s_ecrire_mot(ceci->le_contenu);
            if(ceci->les_parentes[PETIT_FRERE] == NULL){
                s_terminer_ligne();
            } else {
                if(ceci->les_parentes[PETIT_FRERE]->l_etiquette != MOT){
                    s_terminer_ligne();
                }
                sauvegarder_elabore(ceci->les_parentes[PETIT_FRERE]);
            }
        } else {
            s_ouvrir_bloc();
            if(ceci->les_parentes[PREMIER_FILS] != NULL){
                sauvegarder_elabore(ceci->les_parentes[PREMIER_FILS]);
            }
            s_fermer_bloc();

            if(ceci->les_parentes[PETIT_FRERE] != NULL){
                sauvegarder_elabore(ceci->les_parentes[PETIT_FRERE]);
            }
        }
    }
}


void detruire_nanodom(t_arbre_nanodom* ceci){
    if(*ceci != NULL){
        if((*ceci)->les_parentes[PREMIER_FILS] != NULL){
            detruire_nanodom(&((*ceci)->les_parentes[PREMIER_FILS]));
        } 
        if((*ceci)->les_parentes[PETIT_FRERE] != NULL){
            detruire_nanodom(&((*ceci)->les_parentes[PETIT_FRERE]));
        }
        printf("DESTRUCTION: ");
        debugger_noeud(*ceci);
        detruire_noeud(ceci);
    }
}