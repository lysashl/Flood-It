#ifndef __S_ZSG__
#define __S_ZSG__

#include "Liste_case.h"

typedef struct {
    int dim;           /* dimension de la grille */
    int nbcl;          /* nombre de couleurs */

    ListeCase Lzsg;    /* Liste des cases de la zone Zsg */
    ListeCase *B;      /* Tableau de listes (bordures) */
    int **App;         /* Tableau d'appartenance :
                         -2 : non visité
                         -1 : dans la Zsg
                          c : dans la bordure de couleur c
                       */
} S_Zsg;

/* Initialisation */
void init_Zsg(S_Zsg *Z, int dim, int nbcl);

/* Ajout en O(1) */
void ajoute_Zsg(S_Zsg *Z, int i, int j);
void ajoute_Bordure(S_Zsg *Z, int i, int j, int cl);

/* Tests d'appartenance */
int appartient_Zsg(S_Zsg *Z, int i, int j);
int appartient_Bordure(S_Zsg *Z, int i, int j, int cl);

/* Agrandissement de Zsg */
int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l);

/* Libération */
void free_Zsg(S_Zsg *Z);

#endif
