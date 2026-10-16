#include <stdlib.h>
#include "S_Zsg.h"
#include "Liste_case.h"




void init_Zsg(S_Zsg *Z, int dim, int nbcl) {
    int i, j;

    Z->dim = dim;
    Z->nbcl = nbcl;

    /* Initialiser Lzsg */
    init_liste(&Z->Lzsg);

    /* Allouer B[nbcl] */
    Z->B = (ListeCase*) malloc(sizeof(ListeCase) * nbcl);
    for (i = 0; i < nbcl; i++)
        init_liste(&Z->B[i]);

    /* Allouer et initialiser App[][] */
    Z->App = (int**) malloc(sizeof(int*) * dim);
    for (i = 0; i < dim; i++) {
        Z->App[i] = (int*) malloc(sizeof(int) * dim);
        for (j = 0; j < dim; j++)
            Z->App[i][j] = -2;   /* Non visité */
    }
}


/* Ajout en O(1) */

void ajoute_Zsg(S_Zsg *Z, int i, int j) {
    ajoute_en_tete(&Z->Lzsg, i, j);
    Z->App[i][j] = -1;
}

void ajoute_Bordure(S_Zsg *Z, int i, int j, int cl) {
    ajoute_en_tete(&Z->B[cl], i, j);
    Z->App[i][j] = cl;
}

int appartient_Zsg(S_Zsg *Z, int i, int j) {
    return Z->App[i][j] == -1;
}

int appartient_Bordure(S_Zsg *Z, int i, int j, int cl) {
    return Z->App[i][j] == cl;
}




int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l) {
    ListeCase pile;
    int i, j;
    int vi, vj;
    int ajout = 0;

    init_liste(&pile);
    ajoute_en_tete(&pile, k, l);

    while (!test_liste_vide(&pile)) {
        enleve_en_tete(&pile, &i, &j);

        /* Si déjà dans Zsg → ignorer */
        if (Z->App[i][j] == -1)
            continue;

        /* Cas où la case est de la bonne couleur → agrandir Zsg */
        if (M[i][j] == cl) {
            ajoute_Zsg(Z, i, j);
            ajout++;

            /* 4 voisins */
            /* Haut */
            if (i > 0) {
                vi = i - 1; vj = j;
                if (Z->App[vi][vj] == -2) {
                    if (M[vi][vj] == cl)
                        ajoute_en_tete(&pile, vi, vj);
                    else
                        ajoute_Bordure(Z, vi, vj, M[vi][vj]);
                }
            }
            /* Bas */
            if (i < Z->dim - 1) {
                vi = i + 1; vj = j;
                if (Z->App[vi][vj] == -2) {
                    if (M[vi][vj] == cl)
                        ajoute_en_tete(&pile, vi, vj);
                    else
                        ajoute_Bordure(Z, vi, vj, M[vi][vj]);
                }
            }
            /* Gauche */
            if (j > 0) {
                vi = i; vj = j - 1;
                if (Z->App[vi][vj] == -2) {
                    if (M[vi][vj] == cl)
                        ajoute_en_tete(&pile, vi, vj);
                    else
                        ajoute_Bordure(Z, vi, vj, M[vi][vj]);
                }
            }
            /* Droite */
            if (j < Z->dim - 1) {
                vi = i; vj = j + 1;
                if (Z->App[vi][vj] == -2) {
                    if (M[vi][vj] == cl)
                        ajoute_en_tete(&pile, vi, vj);
                    else
                        ajoute_Bordure(Z, vi, vj, M[vi][vj]);
                }
            }
        }
    }

    detruit_liste(&pile);
    return ajout;
}


/* liberation */

void free_Zsg(S_Zsg *Z) {
    int i;

    detruit_liste(&Z->Lzsg);

    for (i = 0; i < Z->nbcl; i++)
        detruit_liste(&Z->B[i]);
    free(Z->B);

    for (i = 0; i < Z->dim; i++)
        free(Z->App[i]);
    free(Z->App);
}




