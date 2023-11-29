/*
    fichier image.h
    but: contenir les prototypes des fonctions de image.c */

#ifndef IMAGE_H
#define IMAGE_H

void chargerRessources(SDL_Surface **menu1, SDL_Surface **menu2, SDL_Surface **imageDeFond, SDL_Surface **gagner, SDL_Surface **cestPlus, SDL_Surface **cestMoins, SDL_Surface **info, SDL_Surface **tempsEcoule, int *rejouer);
void libererRessources(SDL_Surface **menu1, SDL_Surface **menu2, SDL_Surface **imageDeFond, SDL_Surface **gagner, SDL_Surface **cestPlus, SDL_Surface **cestMoins, SDL_Surface **info, SDL_Surface **tempsEcoule);

int lireNombreImage(int *rejouer);
void chargerImages(SDL_Surface *image[], int *rejouer);

int choisirImageAleatoire(SDL_Surface *ecran, SDL_Surface *image[], SDL_Rect *positionImage, int *nombreImage);
int lireBonPrix(int nombreAleatoire, int *continuer, int *rejouer);

#endif





