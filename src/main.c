/*
    Jeu "TicTac...Prix"
    réalisé pour le projet ISN Informatique et Science du Numérique en 2013

    fichier main.c
    but: fonction principale du jeu

    contient les fonctions:
       - main
       - appuyerSurTouche
       - afficherErreur
                                        */

#include <stdlib.h>             // On inclut les bibliothèques
#include <stdio.h>              // base pour la programmation en C

#include <SDL/SDL.h>            // SDL
#include <SDL/SDL_image.h>      // Gérer tous les formats d'images: jpg, png, bmp ...
#include <SDL/SDL_ttf.h>        // Permet d'écrire du texte: decompte, prixTape
#include <fmodex/fmod.h>        // Jouer du son: tic tac
#include <math.h>               // Utiliser la fonction ceil() dans jeu.c pour prendre la partie entiere d'un nombre
#include <dirent.h>             // Parcourir les dossier dans image.c pour charger toutes les images du dossier

#ifndef WIN32
    #include <sys/types.h>      // Parcourir les dossier dans image.c pour charger toutes les images du dossier
#endif

#include "main.h"               // On inclut les fichier .h dit "headers" qui contiennent les prototypes des fonctions
#include "menu.h"               // Cela permet d'utiliser les fonctions des autres fichiers dans celui-ci
#include "jeu.h"                // pour rendre le code plus lisible
#include "image.h"


int main(int argc, char *argv[])  // Début de la fonction principale
{

    //************** DECLARATION DES SURFACES ET DES VARIABLES *****************


    int choixMenu = 0, rejouer = 1;
    int nombreImage = 0;

    // Déclaration des surfaces
    SDL_Surface *ecran = NULL;
    SDL_Surface *menu1 = NULL, *menu2 = NULL;
    SDL_Surface *imageDeFond = NULL, *gagner = NULL, *cestPlus = NULL, *cestMoins = NULL, *tempsEcoule = NULL, *info = NULL;
    SDL_Surface *image = NULL;     // On va créer un tableau avec cette variable quand on connaitra
                                   // le nombre d'image dans le dossier


    //************* INITIALISATION DE FMOD POUR JOUER DU SON *******************


    FMOD_SYSTEM *system;
    FMOD_SOUND *ticTac = NULL; // Ce pointeur contiendra le son
    FMOD_RESULT resultat;      // Variable pour tester si le chargement du fichier son a marché

    FMOD_System_Create(&system);
    FMOD_System_Init(system, 2, FMOD_INIT_NORMAL, NULL);
    resultat = FMOD_System_CreateSound(system, "ressources/tic_tac.wav", FMOD_CREATESAMPLE, 0, &ticTac); // Chargement du son
    if (resultat != FMOD_OK) // Si le chargement a échoué
    {
        fprintf(stderr, "Impossible de lire tic_tac.wav\n"); // Message d'erreur dans le fichier stderr.txt    }
    }


    //*************** INITIALISATION DE LA SDL ET DE SDL_TTF POUR ECRIRE DU TEXTE ****************


    SDL_Init(SDL_INIT_VIDEO);                                // Initialisation de la SDL
    TTF_Init();                                              // Initialisation le mode TTF pour écrire dans la SDL

    putenv("SDL_VIDEO_WINDOW_POS=center");                   // Centre la fenêtre à l'ouverture du programme
    ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption("TicTac...Prix", NULL);                  // Définition du titre de la fenêtre

    SDL_Rect positionEcran; // On initialise la position de l'écran à 0
    positionEcran.x = 0;
    positionEcran.y = 0;


    //***************** CHARGEMENT DU DOSSIER 'IMAGES' ET DU DOSSIER 'RESSOURCES'  *************************


    nombreImage = lireNombreImage(&rejouer);           // On lit le nombre d'image dans le dossier 'images'
    image = malloc(nombreImage * sizeof(SDL_Surface)); // On crée un tableau juste assez grand pour contenir toutes
                                                       // les images du dossier

    // On charge les images ressources
    chargerRessources(&menu1, &menu2, &imageDeFond, &gagner, &cestPlus, &cestMoins, &info, &tempsEcoule, &rejouer);
    // On charge l'image du dossier image dans le tableau créé
    chargerImages(image, &rejouer);


    //*************************  FIN DU CHARGMENT ******************************************


    // Si il y a eu une erreur durant les chargements, rejouer = 0

    if(rejouer) // Si le chargement des images s'est bien déroulé
    {
        SDL_BlitSurface(info, NULL, ecran, &positionEcran); // On colle les infos à l'écran
        SDL_Flip(ecran);                                    // On met à jour l'écran
        appuyerSurTouche(&rejouer);                         // Appuyer sur n'importe quelle touche pour continuer
    }
    else
    {
        afficherErreur(ecran); // Sinon on affiche qu'il y a eu une erreur
    }

    while(rejouer)
    {
        choixMenu = gererMenu(ecran, menu1, menu2, &rejouer); // Appel d'une fonction pour gérer le menu

        if(choixMenu == 1)   // On lance le jeu
        {
            jouer(ecran, imageDeFond, image, gagner, cestPlus, cestMoins, tempsEcoule, &nombreImage, &rejouer, system, ticTac);
        }

        if(choixMenu == 2) // On arrête le jeu
        {
            rejouer = 0;
        }
    }

    // ****************** LIBERATION DE LA MEMOIRE UTILISEE PAR LE JEU  **************************


    FMOD_Sound_Release(ticTac); // On ferme le mode son
    FMOD_System_Close(system);
    FMOD_System_Release(system);

    // On libère les images resources
    libererRessources(&menu1, &menu2, &imageDeFond, &gagner, &cestPlus, &cestMoins, &info, &tempsEcoule);
    free(image);               // On libère les images du jeu
    TTF_Quit();                // Fermeture du mode TTF
    SDL_Quit();                // Arrêt de la SDL

    return EXIT_SUCCESS;
}


void appuyerSurTouche(int *rejouer)       // Appuyer sur n'importe quelle touche pour continuer
{
    int continuer = 1;
    SDL_Event event;                      // Variable qui contiendra les évènements du clavier

    while (continuer)                     // On crée une boucle pour tester en permanence s'il y a un évènement
    {
        SDL_PollEvent(&event);            // Chargement des évènements dans event
        switch(event.type)
        {
            case SDL_QUIT:                // Si on clique sur la croix de la fenêtre
                continuer = 0;
                *rejouer = 0;
                break;
            case SDL_KEYDOWN:             // Si une touche est appuyée
                switch(event.key.keysym.sym)
                {
                    default:              // Si on appuie sur n'importe quelle touche
                        continuer = 0;
                        break;
                }
                break;
        }

        SDL_Delay(30); // On met une pause de 30 ms pour empêcher que le jeu monopolise le processeur
    }

}


void afficherErreur(SDL_Surface *ecran)
{
    SDL_Rect positionEcran;
    positionEcran.x = 0;
    positionEcran.y = 0;

    SDL_Surface *erreur = IMG_Load("ressources/erreur.jpg"); // Chargement de l'image erreur.jpg
    SDL_BlitSurface(erreur, NULL, ecran, &positionEcran);    // On colle l'image à l'écran
    SDL_Flip(ecran);                                         // Mise à jour de l'écran

    SDL_Delay(10000);                                        // On attend 10 s
    SDL_FreeSurface(erreur);                                 // On libère la mémoire de l'image d'erreur
}



