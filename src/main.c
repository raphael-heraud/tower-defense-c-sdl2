#include <SDL2/SDL.h>
#include "maSDL.h"    //biblioth�que avec des fonctions d'affichage utilisant la SDL
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "towerdefense.h"
#include "listeChainee.h"

/*--------- Main ---------------------*/
int main()
{
    // Initialisation de l'al�atoire pour les g�n�rations
    srand(time(NULL));

    SDL_Window *pWindow;
    SDL_Init(SDL_INIT_VIDEO);

    pWindow = SDL_CreateWindow(
        "Appuyez sur ECHAP pour quitter, S/C ET D/V les gerer les sauvegardes",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        LARGEURJEU*40,
        HAUTEURJEU*40,
        SDL_WINDOW_SHOWN
    );

    SDL_Surface* pWinSurf = SDL_GetWindowSurface(pWindow);
    SDL_Surface* pSpriteTourSol = SDL_LoadBMP("assets/tour_sol.bmp");
    SDL_Surface* pSpriteTourAir = SDL_LoadBMP("assets/tour_air.bmp");
    SDL_Surface* pSpriteTourRoi = SDL_LoadBMP("assets/tour_roi.bmp");
    SDL_Surface* pSpriteArcher = SDL_LoadBMP("assets/archer.bmp");
    SDL_Surface* pSpriteChevalier = SDL_LoadBMP("assets/chevalier.bmp");
    SDL_Surface* pSpriteDragon = SDL_LoadBMP("assets/dragon.bmp");
    SDL_Surface* pSpriteGargouille = SDL_LoadBMP("assets/gargouille.bmp");
    SDL_Surface* pSpriteEau = SDL_LoadBMP("assets/eau.bmp");
    SDL_Surface* pSpriteHerbe = SDL_LoadBMP("assets/herbe.bmp");
    SDL_Surface* pSpritePont = SDL_LoadBMP("assets/pont.bmp");
    SDL_Surface* pSpriteTerre = SDL_LoadBMP("assets/terre.bmp");

    SDL_Surface* TabSprite[11]={pSpriteTourSol,pSpriteTourAir,pSpriteTourRoi,pSpriteArcher,pSpriteChevalier,pSpriteDragon,pSpriteGargouille,pSpriteEau,pSpriteHerbe,pSpritePont,pSpriteTerre};

    int** tabParcours=initChemin();

    if ( pSpriteTourSol )
    {
        TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
        initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);
        affichePlateauConsole(jeu,LARGEURJEU,HAUTEURJEU);

        prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
        maj_fenetre(pWindow);

        //A COMMENTER quand vous en aurez assez de cliquer sur ces popups ^^
        message("Welcome in TowerDefend","Bienvenue dans TowerDefend !");
        message("Commandes importantes","ECHAP->quitter, S/C ET D/V pour g�rer les sauvegardes. Bon jeu !");

        /**********************************************************************/
        //variables
        TListePlayer listeHorde = initHorde();
        TListePlayer listeRoi = initRoi();

        // Ajout de la Tour du Roi initiale � la fin du chemin
        Tunite* roiInitial = creeTourRoi(tabParcours[NBCOORDPARCOURS-1][X], tabParcours[NBCOORDPARCOURS-1][Y]);
        // Dans towerdefend.c, vous avez ajout� ajouterUnite(&listeRoi, roiInitial)
        listeRoi = AjoutEnTete(listeRoi, roiInitial);

        /**********************************************************************/
        // boucle principale du jeu
        int cont = 1;
        int cpt = 0; //Compteur pour que les tours apparaissent � un ratio plus faible
        while ( cont != 0 ){
                SDL_PumpEvents(); //do events
                efface_fenetre(pWinSurf);

                /***********************************************************************/

                // On vide le plateau pour le reconstruire
                initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);

                positionnePlayerOnPlateau(listeHorde, jeu);
                positionnePlayerOnPlateau(listeRoi, jeu);

                // Cr�ation des unit�s
                if (cpt % 2 == 0){
                    creationUniteAleatoireHorde(&listeHorde, tabParcours);
                }
                if (cpt % 15 == 0){ //pour que les tours n'arrivent pas trop vite
                    creationUniteAleatoireRoi(&listeRoi, tabParcours, jeu);
                }

                //Bug pour la fonction dessinerAttaque, on a �t� oblig� de mettre la fonction ici
                prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);

                //Il faut que les unit�s puissent attaquer

                TListePlayer current = listeRoi;
                while (current != NULL) {
                    current->pdata->peutAttaquer = 1;
                    current = current->suiv;
                }

                current = listeHorde;
                while (current != NULL) {
                    current->pdata->peutAttaquer = 1;
                    current = current->suiv;
                }

                //Phase de combat

                TListePlayer ptrRoi = listeRoi;
                while (ptrRoi != NULL) {
                    combat(pWinSurf, ptrRoi->pdata, &listeHorde, jeu);
                    ptrRoi = ptrRoi->suiv;
                }

                TListePlayer ptrHorde = listeHorde;
                while (ptrHorde != NULL) {
                    combat(pWinSurf, ptrHorde->pdata, &listeRoi, jeu);
                    ptrHorde = ptrHorde->suiv;
                }

                // D�placement horde

                ptrHorde = listeHorde;
                while(!listeVide(ptrHorde)){
                    avancerUnite(ptrHorde->pdata, jeu, tabParcours);
                    ptrHorde = ptrHorde->suiv;
                }

                //Fin de la partie
                if(tourRoiDetruite(listeRoi)) {
                    cont = 0;
                    message("Merci d'avoir jou� !","La tour du Roi est detruite ! Fin de la partie.");
                }

                cpt++;
                /***********************************************************************/

                //affichage du jeu � chaque tour (avec les nouvelles positions)
                maj_fenetre(pWindow);
                SDL_Delay(500);  // R�gler le d�lai


                //LECTURE DE CERTAINES TOUCHES POUR LANCER LES RESTAURATIONS ET SAUVEGARDES
                const Uint8* pKeyStates = SDL_GetKeyboardState(NULL);
                if ( pKeyStates[SDL_SCANCODE_V] ){
                        // CHARGER S�QUENTIEL (TEXTE)
                        chargerPartieSequentiel(&listeRoi, &listeHorde, tabParcours, jeu);
                        printf("Chargement sequentiel...\n");
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_C] ){
                        // CHARGER BINAIRE
                        chargerPartieBinaire(&listeRoi, &listeHorde, tabParcours, jeu);
                        printf("Chargement binaire...\n");
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_D] ){
                        // SAUVEGARDER S�QUENTIEL (TEXTE)
                        sauvegarderPartieSequentiel(listeRoi, listeHorde, tabParcours);
                        printf("Sauvegarde sequentielle effectuee !\n");
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_S] ){
                        // SAUVEGARDER BINAIRE
                        sauvegarderPartieBinaire(listeRoi, listeHorde, tabParcours);
                        printf("Sauvegarde binaire effectuee !\n");
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_ESCAPE] ){
                        cont = 0;  //sortie de la boucle
                }

        }
        //fin boucle du jeu

        SDL_FreeSurface(pSpriteTourSol);
        SDL_FreeSurface(pSpriteTourAir);
        SDL_FreeSurface(pSpriteTourRoi);
        SDL_FreeSurface(pSpriteArcher);
        SDL_FreeSurface(pSpriteChevalier);
        SDL_FreeSurface(pSpriteDragon);
        SDL_FreeSurface(pSpriteGargouille);
        SDL_FreeSurface(pSpriteEau);
        SDL_FreeSurface(pSpriteHerbe);
        SDL_FreeSurface(pSpritePont);
        SDL_FreeSurface(pWinSurf);
    }
    else
    {
        fprintf(stdout,"�chec de chargement du sprite (%s)\n",SDL_GetError());
    }

    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    freeChemin(tabParcours);
    return 0;
}
