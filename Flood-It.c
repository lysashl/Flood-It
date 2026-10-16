#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "API_Grille.h"
#include "API_Gene_instance.h"
#include "Liste_case.h"
#include "S_Zsg.h"


/* les prototypes des fonctions */
void trouve_zone_rec(int **M, int dim, int i, int j, int *taille, ListeCase *L);

int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff);

int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff);

void compte_couleurs_bordure(S_Zsg *Z, int *compteurs);

int sequence_max_bordure(int **M, Grille *G, int dim, int nbcl, int aff);

void compte_couleurs_bordure_zone(int **M, S_Zsg *Z, int *compteurs);

int sequence_max_bordure_zone(int **M, Grille *G, int dim, int nbcl, int aff);

int sequence_maximale_futur(int **M, Grille *G, int dim, int nbcl, int aff);



/*                          MAIN                             */


int main(int argc, char **argv)
{
    int dim, nbcl, nivdif, graine, exo, aff;
    Grille *G;
    int i, j;
    int **M;
    int nb_coups;

    clock_t 
      temps_initial,/* Temps initial en micro-secondes */
      temps_final; /* Temps final en micro-secondes */
    float 
      temps_cpu; /* Temps total en secondes */

    if (argc != 7) {
        printf("usage : %s <dimension> <nb_de_couleurs> <niveau_difficute> <graine> <exo:0-1-2> <aff 0/1>\n", argv[0]);
        return 1;
    }

    dim = atoi(argv[1]);
    nbcl = atoi(argv[2]);
    nivdif = atoi(argv[3]);
    graine = atoi(argv[4]);
    exo = atoi(argv[5]);
    aff = atoi(argv[6]);

    /* Allocation puis generation de l'instance */
    M = (int **)malloc(dim * sizeof(int *));
    for (i = 0; i < dim; i++)
        M[i] = (int *)malloc(dim * sizeof(int));

    /* Génération instance */
    Gene_instance_genere_matrice(dim, nbcl, nivdif, graine, M);

    
    if (aff == 1) { /* Affichage de la grille */
        Grille_init(dim, nbcl, 500, &G);
        Grille_ouvre_fenetre(G);

        for (i = 0; i < dim; i++)
            for (j = 0; j < dim; j++)
                Grille_attribue_couleur_case(G, i, j, M[i][j]);

        Grille_redessine_Grille(G);
        Grille_attente_touche();
    }

    temps_initial = clock();

    if (exo == 0){
        nb_coups = sequence_aleatoire_rec(M, G, dim, nbcl, aff);
    }

    if (exo == 1){
        nb_coups = sequence_aleatoire_rec(M, G, dim, nbcl, aff);
    }

    if (exo == 2){
        nb_coups = sequence_aleatoire_rapide(M, G, dim, nbcl, aff);
    }
    
    if (exo == 3){
      /* Exercice 3 : Stratégie max-bordure */
      nb_coups = sequence_max_bordure(M, aff==1 ? G : NULL, dim, nbcl, aff);
    }
    
    if (exo == 4){
      nb_coups = sequence_max_bordure_zone(M, aff==1 ? G : NULL, dim, nbcl, aff);
    }
    
    if (exo == 5){
      nb_coups = sequence_maximale_futur(M, aff==1 ? G : NULL, dim , nbcl, aff);
    }
      

    temps_final = clock();

    temps_cpu= (temps_final - temps_initial) * 1e-6;

    printf("Coups = %d\n", nb_coups);
    printf("Temps CPU = %f sec\n", temps_cpu);

    /* Desallocation de la matrice */
    for (i = 0; i < dim; i++)
        free(M[i]);
    free(M);

    if (aff == 1) {
        Grille_ferme_fenetre();
        Grille_free(&G);
    }

    return 0;
}


/*                EXERCICE 1 : trouve_zone_rec               */


void trouve_zone_rec(int **M, int dim, int i, int j, int *taille, ListeCase *L)
{
    int couleur = M[i][j];

    /* Marquer la case comme visitée en utilisant -2 */
    M[i][j] = -2;

    /* Ajouter à la liste */
    ajoute_en_tete(L, i, j);
    (*taille)++;

    /* haut */
    if (i > 0 && M[i - 1][j] == couleur)
        trouve_zone_rec(M, dim, i - 1, j, taille, L);

    /* bas */
    if (i < dim - 1 && M[i + 1][j] == couleur)
        trouve_zone_rec(M, dim, i + 1, j, taille, L);

    /* gauche */
    if (j > 0 && M[i][j - 1] == couleur)
        trouve_zone_rec(M, dim, i, j - 1, taille, L);

    /* droite */
    if (j < dim - 1 && M[i][j + 1] == couleur)
        trouve_zone_rec(M, dim, i, j + 1, taille, L);
}


/*       sequence_aleatoire_rec        */


int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff)
{
    int nb_coups = 0;
    int taille_totale = dim * dim;
    int taille_zone;
    int couleur_actuelle, nouvelle_couleur;
    ListeCase L;
    ListeCase temp;

    srand(time(NULL));

    while (1) {

        /* Trouver la zone Zsg */
        taille_zone = 0;
        init_liste(&L);

        couleur_actuelle = M[0][0];

        trouve_zone_rec(M, dim, 0, 0, &taille_zone, &L);

        /* Restaurer les couleurs (on avait mis -2) */
        temp = L;
        while (temp != NULL) {
            M[temp->i][temp->j] = couleur_actuelle;
            temp = temp->suiv;
        }

        /* Victoire */
        if (taille_zone == taille_totale) {
            detruit_liste(&L);
            break;
        }

        /* Choisir nouvelle couleur ≠ couleur_actuelle */
        do {
            nouvelle_couleur = rand() % nbcl;
        } while (nouvelle_couleur == couleur_actuelle);

        /* Coloration */
        temp = L;
        while (temp != NULL) {

            M[temp->i][temp->j] = nouvelle_couleur;

            if (aff == 1)
                Grille_attribue_couleur_case(G, temp->i, temp->j, nouvelle_couleur);

            temp = temp->suiv;
        }

        if (aff == 1)
            Grille_redessine_Grille(G);

        detruit_liste(&L);

        nb_coups++;
    }

    return nb_coups;
}


/*       EXERCICE 2 : sequence_aleatoire_rapide              */

int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff)
{
    S_Zsg Z;
    int nb_coups = 0;
    int taille_totale = dim * dim;
    int taille_zone = 0;

    int couleur_actuelle, nouvelle_couleur;
    ListeCase temp;

    srand(time(NULL));

    /* Initialiser la structure ZSG */
    init_Zsg(&Z, dim, nbcl);

    /* Initialiser Zsg avec case (0,0) */
    couleur_actuelle = M[0][0];
    ajoute_Bordure(&Z, 0, 0, couleur_actuelle);
    taille_zone += agrandit_Zsg(M, &Z, couleur_actuelle, 0, 0);

    while (taille_zone < taille_totale) {

        /* Choisir nouvelle couleur */
        do {
            nouvelle_couleur = rand() % nbcl;
        } while (nouvelle_couleur == couleur_actuelle);

        /* Colorier toute la Zsg */
        temp = Z.Lzsg;
        while (temp != NULL) {

            M[temp->i][temp->j] = nouvelle_couleur;

            if (aff == 1)
                Grille_attribue_couleur_case(G, temp->i, temp->j, nouvelle_couleur);

            temp = temp->suiv;
        }

        if (aff == 1)
            Grille_redessine_Grille(G);

        /* Agrandir avec B[nouvelle_couleur] */
        temp = Z.B[nouvelle_couleur];
        while (temp != NULL) {

            int i = temp->i;
            int j = temp->j;

            if (Z.App[i][j] == nouvelle_couleur)
                taille_zone += agrandit_Zsg(M, &Z, nouvelle_couleur, i, j);

            temp = temp->suiv;
        }

        detruit_liste(&(Z.B[nouvelle_couleur]));

        couleur_actuelle = nouvelle_couleur;
        nb_coups++;
    }

    free_Zsg(&Z);
    return nb_coups;
}


/*          EXERCICE 3 :    max-bordure          */

/* Q 3.1 : Compte le nombre de cases de chaque couleur dans la bordure */
void compte_couleurs_bordure(S_Zsg *Z, int *compteurs) {
    int c;
    ListeCase temp;
    
    /* Initialiser les compteurs à 0 */
    for (c = 0; c < Z->nbcl; c++) {
        compteurs[c] = 0;
    }
    
    /* Parcourir chaque liste de bordure et compter */
    for (c = 0; c < Z->nbcl; c++) {
        temp = Z->B[c];
        while (temp != NULL) {
            compteurs[c]++;
            temp = temp->suiv;
        }
    }
}

/* Q 3.2 : Implémentation de max-bordure */
int sequence_max_bordure(int **M, Grille *G, int dim, int nbcl, int aff) {
    S_Zsg Z;
    int nb_coups = 0;
    int taille_totale = dim * dim;
    int taille_zone = 0;
    int couleur_actuelle, nouvelle_couleur;
    ListeCase temp, copie;
    int i, j, c;
    int *compteurs;
    int bordure_vide;
    int max_count;
    
    /* Allouer le tableau de compteurs */
    compteurs = (int *)malloc(sizeof(int) * nbcl);
    
    /* Initialiser la structure S_Zsg */
    init_Zsg(&Z, dim, nbcl);
    
    /* Initialiser Lzsg et B avec la zone contenant (0,0) */
    couleur_actuelle = M[0][0];
    taille_zone = agrandit_Zsg(M, &Z, couleur_actuelle, 0, 0);
    
    while (taille_zone < taille_totale) {
        /* Compter les couleurs dans la bordure */
        compte_couleurs_bordure(&Z, compteurs);
        
        /* Vérifier si la bordure est vide pour toutes les couleurs */
        bordure_vide = 1;
        for (i = 0; i < nbcl; i++) {
            if (i != couleur_actuelle && compteurs[i] > 0) {
                bordure_vide = 0;
                break;
            }
        }
        
        /* Si bordure vide, le jeu est terminé */
        if (bordure_vide) {
            break;
        }
        
        /* Trouver la couleur avec le maximum de cases dans la bordure */
        nouvelle_couleur = -1;
        max_count = 0;
        for (c = 0; c < nbcl; c++) {
            if (c != couleur_actuelle && compteurs[c] > max_count) {
                max_count = compteurs[c];
                nouvelle_couleur = c;
            }
        }
        
        /* Si aucune couleur trouvée, sortir */
        if (nouvelle_couleur == -1) {
            break;
        }
        
        /* Colorier toute la zone Zsg avec la nouvelle couleur */
        temp = Z.Lzsg;
        while (temp != NULL) {
            M[temp->i][temp->j] = nouvelle_couleur;
            
            if (aff == 1) {
                Grille_attribue_couleur_case(G, temp->i, temp->j, nouvelle_couleur);
            }
            
            temp = temp->suiv;
        }
        
        if (aff == 1) {
            Grille_redessine_Grille(G);
        }
        
        /* Sauvegarder la liste B[nouvelle_couleur] avant de la modifier */
        copie = Z.B[nouvelle_couleur];
        init_liste(&(Z.B[nouvelle_couleur]));
        
        /* Parcourir la copie et agrandir pour chaque case */
        while (copie != NULL) {
            i = copie->i;
            j = copie->j;
            
            /* Agrandir si la case est toujours marquée correctement */
            if (Z.App[i][j] == nouvelle_couleur) {
                taille_zone += agrandit_Zsg(M, &Z, nouvelle_couleur, i, j);
            }
            
            copie = copie->suiv;
        }
        
        couleur_actuelle = nouvelle_couleur;
        nb_coups++;
    }
    
    free(compteurs);
    free_Zsg(&Z);
    return nb_coups;
}

/*         EXERCICE 4 : Stratégie max-bordure-zone         */


void compte_couleurs_bordure_zone(int **M, S_Zsg *Z, int *compteurs) {
    int c, i, j, v;
    ListeCase temp;
    int **visite_zone;
    int voisins_i[4] = {-1, 1, 0, 0};
    int voisins_j[4] = {0, 0, -1, 1};
    ListeCase file;
    int taille_zone;
    int vi, vj;
    
    /* Initialiser les compteurs à 0 */
    for (c = 0; c < Z->nbcl; c++) {
        compteurs[c] = 0;
    }
    
    /* Allouer tableau pour marquer les zones déjà visitées */
    visite_zone = (int **)malloc(sizeof(int*) * Z->dim);
    for (i = 0; i < Z->dim; i++) {
        visite_zone[i] = (int *)malloc(sizeof(int) * Z->dim);
        for (j = 0; j < Z->dim; j++) {
            visite_zone[i][j] = 0;
        }
    }
    
    /* Pour chaque couleur dans la bordure */
    for (c = 0; c < Z->nbcl; c++) {
        temp = Z->B[c];
        
        /* Parcourir chaque case de la bordure de couleur c */
        while (temp != NULL) {
            i = temp->i;
            j = temp->j;
            
            /* Si cette case n'a pas encore été visitée */
            if (visite_zone[i][j] == 0) {
                /* Explorer toute la zone contenant cette case */
                init_liste(&file);
                ajoute_en_tete(&file, i, j);
                visite_zone[i][j] = 1;
                taille_zone = 0;
                
                /* Parcours en largeur de la zone */
                while (!test_liste_vide(&file)) {
                    int ci, cj;
                    enleve_en_tete(&file, &ci, &cj);
                    taille_zone++;
                    
                    /* Explorer les voisins */
                    for (v = 0; v < 4; v++) {
                        vi = ci + voisins_i[v];
                        vj = cj + voisins_j[v];
                        
                        if (vi >= 0 && vi < Z->dim && vj >= 0 && vj < Z->dim) {
                            /* Si même couleur et pas encore visité */
                            if (M[vi][vj] == c && visite_zone[vi][vj] == 0) {
                                ajoute_en_tete(&file, vi, vj);
                                visite_zone[vi][vj] = 1;
                            }
                        }
                    }
                }
                
                /* Ajouter la taille de cette zone au compteur */
                compteurs[c] += taille_zone;
                detruit_liste(&file);
            }
            
            temp = temp->suiv;
        }
    }
    
    /* Libérer le tableau de visite */
    for (i = 0; i < Z->dim; i++) {
        free(visite_zone[i]);
    }
    free(visite_zone);
}

/* Q 4.2 : Implémentation de max-bordure-zone */
int sequence_max_bordure_zone(int **M, Grille *G, int dim, int nbcl, int aff) {
    S_Zsg Z;
    int nb_coups = 0;
    int taille_totale = dim * dim;
    int taille_zone = 0;
    int couleur_actuelle, nouvelle_couleur;
    ListeCase temp, copie;
    int i, j, c;
    int *compteurs;
    int bordure_vide;
    int max_count;
    
    /* Allouer le tableau de compteurs */
    compteurs = (int *)malloc(sizeof(int) * nbcl);
    
    /* Initialiser la structure S_Zsg */
    init_Zsg(&Z, dim, nbcl);
    
    /* Initialiser Lzsg et B avec la zone contenant (0,0) */
    couleur_actuelle = M[0][0];
    taille_zone = agrandit_Zsg(M, &Z, couleur_actuelle, 0, 0);
    
    while (taille_zone < taille_totale) {
        /* Compter les cases de chaque zone adjacente */
        compte_couleurs_bordure_zone(M, &Z, compteurs);
        
        /* Vérifier si la bordure est vide pour toutes les couleurs */
        bordure_vide = 1;
        for (i = 0; i < nbcl; i++) {
            if (i != couleur_actuelle && compteurs[i] > 0) {
                bordure_vide = 0;
                break;
            }
        }
        
        /* Si bordure vide, le jeu est terminé */
        if (bordure_vide) {
            break;
        }
        
        /* Trouver la couleur qui amène le plus de cases */
        nouvelle_couleur = -1;
        max_count = 0;
        for (c = 0; c < nbcl; c++) {
            if (c != couleur_actuelle && compteurs[c] > max_count) {
                max_count = compteurs[c];
                nouvelle_couleur = c;
            }
        }
        
        /* Si aucune couleur trouvée, sortir */
        if (nouvelle_couleur == -1) {
            break;
        }
        
        /* Colorier toute la zone Zsg avec la nouvelle couleur */
        temp = Z.Lzsg;
        while (temp != NULL) {
            M[temp->i][temp->j] = nouvelle_couleur;
            
            if (aff == 1) {
                Grille_attribue_couleur_case(G, temp->i, temp->j, nouvelle_couleur);
            }
            
            temp = temp->suiv;
        }
        
        if (aff == 1) {
            Grille_redessine_Grille(G);
        }
        
        /* Sauvegarder la liste B[nouvelle_couleur] avant de la modifier */
        copie = Z.B[nouvelle_couleur];
        init_liste(&(Z.B[nouvelle_couleur]));
        
        /* Parcourir la copie et agrandir pour chaque case */
        while (copie != NULL) {
            i = copie->i;
            j = copie->j;
            
            /* Agrandir si la case est toujours marquée correctement */
            if (Z.App[i][j] == nouvelle_couleur) {
                taille_zone += agrandit_Zsg(M, &Z, nouvelle_couleur, i, j);
            }
            
            copie = copie->suiv;
        }
        
        couleur_actuelle = nouvelle_couleur;
        nb_coups++;
    }
    
    free(compteurs);
    free_Zsg(&Z);
    return nb_coups;
}

/*                  Exercice 5                 */




int sequence_maximale_futur(int **M, Grille *G, int dim, int nbcl, int aff) {
    S_Zsg Z;
    int nb_coups = 0;
    int taille_totale = dim * dim;
    int taille_zone = 0;
    int couleur_actuelle, nouvelle_couleur;
    int *compteurs;
    ListeCase temp, copie;
    int i, j, c;
   
    /* Allouer un tableau pour compter combien de cases chaque couleur peut apporter */
    compteurs = (int *)malloc(nbcl * sizeof(int));

    /* Initialiser la structure Zsg */
    init_Zsg(&Z, dim, nbcl);

    /* Définir la zone initiale avec la case (0,0) */
    couleur_actuelle = M[0][0];
    taille_zone = agrandit_Zsg(M, &Z, couleur_actuelle, 0, 0);

    /* Boucle principale : tant que la zone n'occupe pas toute la grille */
    while (taille_zone < taille_totale) {

        /* Compter pour chaque couleur combien de cases elle peut apporter maintenant */
        compte_couleurs_bordure(&Z, compteurs);

        /* Chercher la couleur qui maximise l’expansion future */
        nouvelle_couleur = -1;
        int max_valeur = -1;

        for (c = 0; c < nbcl; c++) {
            if (c != couleur_actuelle) {
                
                int valeur_future = compteurs[c];

                
                if (valeur_future > max_valeur) {
                    max_valeur = valeur_future;
                    nouvelle_couleur = c;
                }
            }
        }

        /* Si aucune couleur n'est trouvée, sortir de la boucle */
        if (nouvelle_couleur == -1) {
            break;
        }

        /* Colorier toute la zone Zsg avec la nouvelle couleur choisie */
        temp = Z.Lzsg;
        while (temp != NULL) {
            M[temp->i][temp->j] = nouvelle_couleur;

            /* Si on veut afficher la grille */
            if (aff == 1) {
                Grille_attribue_couleur_case(G, temp->i, temp->j, nouvelle_couleur);
            }

            temp = temp->suiv;
        }

        /* Redessiner si nécessaire */
        if (aff == 1) {
            Grille_redessine_Grille(G);
        }

        /* Agrandir la zone Zsg en fonction de la nouvelle couleur */
        copie = Z.B[nouvelle_couleur];
        init_liste(&(Z.B[nouvelle_couleur]));
        while (copie != NULL) {
            i = copie->i;
            j = copie->j;

            if (Z.App[i][j] == nouvelle_couleur) {
                taille_zone += agrandit_Zsg(M, &Z, nouvelle_couleur, i, j);
            }

            copie = copie->suiv;
        }

        /* Mettre à jour la couleur actuelle */
        couleur_actuelle = nouvelle_couleur;
        nb_coups++;
    }

    /* Libération mémoire */
    free(compteurs);
    free_Zsg(&Z);

    return nb_coups;
}





