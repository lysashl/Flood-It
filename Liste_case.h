#ifndef __Liste_CASE__
#define __Liste_CASE__

/* Liste simplement chainee de case reperee par les coordonnees entiere i,j */
typedef struct elnt_liste{
  int i,j;
  struct elnt_liste *suiv;
} Elnt_liste;

typedef Elnt_liste* ListeCase;

/* Initialise une liste vide */
void init_liste(ListeCase *L);

/* Ajoute un element en tete de liste */
int ajoute_en_tete(ListeCase *L, int i, int j);

/* teste si une liste est vide */
int test_liste_vide(ListeCase *L);

/* Supprime l element de tete et retourne les valeurs en tete */
/* Attention: il faut que la liste soit non vide */
void enleve_en_tete(ListeCase *L, int *i, int *j);

/* Detruit tous les elements de la liste */
void detruit_liste(ListeCase *L);

#endif
