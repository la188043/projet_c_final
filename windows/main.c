#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fonctions.h"

int main()
{
    // Fichiers
    FILE *fdatC, *fdatSpe;
    fdatC = fopen("consultations.dat", "r");
    fdatSpe = fopen("specialites.dat", "r");

    // Variables
    Medecin *firstM, *currentM, *nextM, *lastM, *interM;
    Patient *firstP, *currentP, *nextP, *lastP, *interP;

    int cpM = 0, cpP = 0, exitMenu = 0, i, j, med = 0;
    int n = 1;
    int cpSpe = 0, c, redemarre = 1;
    char **tabSpe = NULL, **tabNomemclature = NULL;
    char choixSave = 'n';

    // On compte les specialites
    if (fdatSpe != NULL)
    {
        while ((c = fgetc(fdatSpe)) != EOF)
        {
            if (c == '\n')
                cpSpe++;
        }

        // On revient au debut du fichier
        rewind(fdatSpe);

        // Initialisation dynamique de la taille du tableau
        tabSpe = (char **)malloc(cpSpe * sizeof(char *));
        tabNomemclature = (char **)malloc(cpSpe * sizeof(char *));
        // Initialisation de la taille des chaines contenues dans les tableaux
        for (i = 0; i < cpSpe; i++)
        {
            tabSpe[i] = (char *)malloc(21 * sizeof(char));
            tabNomemclature[i] = (char *)malloc(7 * sizeof(char));
        }

        // Lecture
        for (i = 0; i < cpSpe; i++)
        {
            fscanf(fdatSpe, "%20s%6s", tabSpe[i], tabNomemclature[i]);
        }

        fclose(fdatSpe);
    }
    else
    {
        fclose(fdatSpe);
        printf("Erreur lecture des specialites\n");
        system("pause");
        return 0;
    }
    


    // Lecture Medecins
    if (lectureMedecins(&firstM, &currentM, &interM, &lastM, &cpM))
    {
        printf("Erreur lecture des medecins\n");
        system("pause");
        return 0;
    }
    // Lecture des consultations
    if (fdatC != NULL)
    {
        for (currentM = firstM; currentM != NULL; currentM = currentM->next)
        {
            for (i = 1; i <= 6; i++)
            {
                for (j = 1; j <= 16; j++)
                {
                    fscanf(fdatC, "%*2d");
                    fgets(currentM->cons[i][j].nomPat, 21, fdatC);
                    currentM->cons[i][j].lettrePrenPat = fgetc(fdatC);
                }
            }
        }
    }
    else
    {
        fclose(fdatC);
        printf("Erreur lecture des consultations\n");
        system("pause");
        return 0;       
    }
    

    // Lecture patients
    if (lecturePatients(&firstP, &currentP, &interP, &lastP, &cpP))
    {
        printf("Erreur lecture des patients\n");
        system("pause");
        return 0;
    }

    // Menu interactif
    system("cls");
    while (1)
    {
        switch (menuPrincipal())
        {
        case 1:
            exitMenu = 0;
            while (exitMenu != 1)
            {
                switch (menuMed())
                {
                case 1:
                    afficherListeMed(firstM, cpM);
                    system("pause");
                    system("cls");
                    break;
                case 2:
                    ajouterMed(&currentM, &firstM, &lastM, &cpM, tabSpe, tabNomemclature, cpSpe);
                    break;
                case 3:
                    supprimerMed(&firstM, &cpM);
                    break;
                case 4:
                    rechercherMed(firstM, &currentM, cpSpe, tabSpe);
                    system("pause");
                    system("cls");
                    break;
                case 5:
                    afficherHoraire(firstM, currentM);
                    system("pause");
                    system("cls");
                    break;
                case 6:
                    if (redemarre)
                        ajouterSpecialite(cpSpe, &redemarre, tabSpe, tabNomemclature);
                    else
                    {
                        system("cls");
                        printf("Un redemarrage est necessaire pour l'utilisation de cette fonctionnalite\n");
                    }

                    break;
                case 7:
                    exitMenu = 1;
                    break;
                default:
                    printf("Mauvais numero selectionne !\n");
                    break;
                }
            }

            break;
        case 2:
            exitMenu = 0;
            while (exitMenu != 1)
            {
                switch (menuPat())
                {
                case 1:
                    afficherListePat(firstP, cpP);
                    system("pause");
                    system("cls");
                    break;
                case 2:
                    ajouterPat(&currentP, &firstP, &lastP, &cpP);
                    break;
                case 3:
                    supprimerPat(&firstP, &cpP);
                    break;
                case 4:
                    rechercherPat(firstP, &currentP);
                    system("pause");
                    system("cls");
                    break;
                case 5:
                    ajouterCons(firstP, &currentP, firstM, &currentM, cpSpe, tabSpe);
                    break;
                case 6:
                    supprimerCons(firstM, &currentM, cpSpe, tabSpe);
                    break;
                case 7:
                    exitMenu = 1;
                    break;
                default:
                    printf("Mauvais numero selectionne !\n");
                    break;
                }
            }

            break;
        case 3:
            reinitialisationRDV(firstM);
            break;
        case 4:
            sauvegarde(firstM, firstP);
            break;
        case 5:
            printf("Voulez-vous sauvegarder les changements effectues ? [o/n] : ");
            choixSave = getc(stdin);
            printf("\n");

            if (choixSave == 'o' || choixSave == 'O')
                sauvegarde(firstM, firstP);

            return 0;

        default:
            printf("Mauvais numero selectionne !\n");
            break;
        }
    }

    return 0;
}
