opt = -c -Wall -Wextra -std=c11

main.exe: p_nanodom.o p_texte_enrichi.o p_lecteur_grammaire.o
	gcc -o main.exe p_lecteur_grammaire.o p_nanodom.o p_texte_enrichi.o

p_lecteur_grammaire.o: p_lecteur_grammaire.c
	gcc ${opt} p_lecteur_grammaire.c

p_texte_enrichi.o: p_texte_enrichi.c
	gcc ${opt} p_texte_enrichi.c

p_nanodom.o: p_nanodom.c
	gcc ${opt} p_nanodom.c
