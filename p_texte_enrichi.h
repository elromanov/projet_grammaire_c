#ifndef P_TEXTE_ENRICHI
#define P_TEXTE_ENRICHI

typedef enum e_mode{NORMAL, MAJUSCULE, MINUSCULE} t_mode;

void changer_mode(t_mode mode);

t_mode mode();

void ouvrir_bloc();

void fermer_bloc();

void indenter();

void desindenter();

int est_en_fin_de_ligne();

int est_au_debut_de_ligne();

void entamer_ligne();

void terminer_ligne();

void pucer();

void ecrire_mot(const char* mot);

int peut_ecrire(int nb_caracteres);

// ------------

void s_changer_mode(t_mode mode);

t_mode s_mode();

void s_ouvrir_bloc();

void s_fermer_bloc();

void s_indenter();

void s_desindenter();

int s_est_en_fin_de_ligne();

int s_est_au_debut_de_ligne();

void s_entamer_ligne();

void s_terminer_ligne();

void s_pucer();

void s_ecrire_mot(const char* mot);

int s_peut_ecrire(int nb_caracteres);

void charger_fichier(FILE* file);

#endif