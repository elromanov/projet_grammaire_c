opt = -c -Wall -Wextra -std=c11

test_nanodom.exe: test_nanodom.o p_nanodom.o p_texte_enrichi.o
	gcc -o test_nanodom.exe p_nanodom.o test_nanodom.o

p_texte_enrichi.o: p_texte_enrichi.c
	gcc ${opt} p_texte_enrichi.c

test_nanodom.o: test_nanodom.c
	gcc ${opt} test_nanodom.c


p_nanodom.o : p_nanodom.h p_nanodom.c
	gcc ${opt} p_nanodom.c

clean :
	rm *.o
	rm *.exe