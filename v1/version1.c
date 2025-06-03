/**
* \file version1.c
* \brief le jeu snake
* \author Merrien Ethan
* \version V1
* \date 15/10/2024
*
* Dans cette première version, l'utilisateur entre des coordonnées,
* le serpent commence donc son mouvement à partir de ces coordonnées et progresse vers la droite,
* le serpent arrête sa progression lorsque l'utilisateur appuie sur la touche "a".
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

/**
*
* \def MAXTAB
*
* \brief constante pour la Taille Max d’un tableau.
*
*/
#define MAXTAB 40

/**
*
* \def MINTAB
*
* \brief constante pour la Taille Min d’un tableau.
*
*/
#define MINTAB 1

/**
*
* \def TAILLE_SERPENT
*
* \brief constante pour la Taille du serpent.
*
*/
#define TAILLE_SERPENT 10

/**
 * \brief Affiche un caractère à une position donnée.
 * 
 * \param x Position en abscisse.
 * \param y Position en ordonnée.
 * \param c Caractère à afficher.
 */
void afficher(int x, int y, char c);

/**
 * \fn Efface le caractère à la position spécifiée.
 * 
 * \param x Position en abscisse.
 * \param y Position en ordonnée.
 */
void effacer(int x, int y);

/**
 * \brief Dessine le serpent à partir des coordonnées fournies.
 * 
 * \param lesX Tableau des positions x des segments du serpent.
 * \param lesY Tableau des positions y des segments du serpent.
 * \param taille_actuelle Nombre de segments actuels du serpent.
 */
void dessinerSerpent(int lesX[], int lesY[], int taille_actuelle);

/**
 * \brief Fait progresser le serpent en déplaçant chaque segment.
 * 
 * \param lesX Tableau des positions x des segments du serpent.
 * \param lesY Tableau des positions y des segments du serpent.
 * \param taille_actuelle Pointeur sur la taille actuelle du serpent.
 */
void progresser(int lesX[], int lesY[], int *taille_actuelle);

int kbhit();

int main()
{
    int i;
    int lesX[TAILLE_SERPENT], lesY[TAILLE_SERPENT];  /**  déclarations de deux tableaux d'entiers qui peuvent contenir TAILLE_SERPENT éléments*/
    char key = ' ';
    int taille_actuelle;  /** Taille actuelle du serpent */

    printf("x = ");
    scanf("%d", &lesX[0]);
    
    /**
    *
    * Boucle permettant de redemander la valeur de x
    * tant qu'elle n'est pas comprise entre 1 et 40.
    *
    */
    while (lesX[0] < MINTAB || lesX[0] > MAXTAB)
    {
        printf("Veuillez saisir une valeur entre 1 et 40.\n");
        printf("x = ");
        scanf("%d", &lesX[0]);
    }
    printf("y = ");
    scanf("%d", &lesY[0]);
    /**
    *
    * Boucle permettant de redemander la valeur de y 
    * tant qu'elle n'est pas comprise entre 1 et 40.
    *
    */
    while (lesY[0] < MINTAB || lesY[0] > MAXTAB)
    {
        printf("Veuillez saisir une valeur entre 1 et 40.\n");
        printf("y = ");
        scanf("%d", &lesY[0]);
    }

    /**
    *
    * Boucle pour initialiser les positions des segments du corps du serpent.
    *
    */
    for (i = 1; i < TAILLE_SERPENT; i++)
    {
        lesX[i] = lesX[0] - i;
        lesY[i] = lesY[0];
    }

    system("clear");

    /** Initialisation de la taille actuelle du serpent */
    taille_actuelle = lesX[0] < TAILLE_SERPENT ? lesX[0] : TAILLE_SERPENT;

    /**
    *
    * Boucle principale:
    * Elle affiche et fait progresser le serpent tant que l'utilisateur n'appuie pas sur la touche "a".
    *
    */
    while (key != 'a' && key != 'A')
    {
        /**
        *
        * Boucle permettant d'effacer l'ancienne position du serpent
        *
        */
        for (i = 0; i < taille_actuelle; i++) {
            effacer(lesX[i], lesY[i]);
        }
        
        progresser(lesX, lesY, &taille_actuelle);
        dessinerSerpent(lesX, lesY, taille_actuelle);
        
        usleep(200000);
        
        /**
        *
        * Condition vérifiant si l'utilisateur a appuyé sur une touche.
        *
        */
        if (kbhit()) 
        {
            key = getchar(); /** récupère la touche saisie par l'utilisateur. */
        }
    }

    return EXIT_SUCCESS;
}

/**
 * \brief Vérifie si une touche a été pressée.
 * 
 * \return 1 si une touche est pressée, sinon 0.
 */
int kbhit(){
    int unCaractere=0;
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
 
    if(ch != EOF){
        ungetc(ch, stdin);
        unCaractere=1;
    } 
    return unCaractere;
}

/**
 * \brief Déplace le curseur de la console aux coordonnées spécifiées.
 * 
 * \param x Position en abscisse.
 * \param y Position en ordonnée.
 */
void gotoXY(int x, int y)
{ 
    printf("\033[%d;%df", y, x);
}

/**
 * \brief Dessine le serpent en affichant 'o' pour la tête et 'x' pour chaque segment du corps.
 * 
 * \param lesX Tableau des positions x des segments du serpent.
 * \param lesY Tableau des positions y des segments du serpent.
 * \param taille_actuelle Nombre de segments actuels du serpent.
 */
void dessinerSerpent(int lesX[], int lesY[], int taille_actuelle)
{
    int i;
    afficher(lesX[0], lesY[0], 'o');
    for (i = 1; i < taille_actuelle; i++)
    {
        afficher(lesX[i], lesY[i], 'x');
    }
}

/**
 * \brief Affiche un caractère à la position donnée.
 * 
 * \param x Position en abscisse.
 * \param y Position en ordonnée.
 * \param c Caractère à afficher.
 */
void afficher(int x, int y, char c)
{
    gotoXY(x, y);
    printf("%c", c);
}

/**
 * \brief Fait progresser le serpent en déplaçant chaque segment vers l'avant.
 * 
 * \param lesX Tableau des positions x des segments du serpent.
 * \param lesY Tableau des positions y des segments du serpent.
 * \param taille_actuelle Pointeur sur la taille actuelle du serpent.
 */
void progresser(int lesX[], int lesY[], int *taille_actuelle)
{
    for (int i = *taille_actuelle - 1; i > 0; i--)
    {
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }

    lesX[0] += 1;

    if (lesX[0] >= MAXTAB) {
        lesX[0] = 0;
    }

    /** 
     * 
     * Augmenter la taille actuelle si elle est inférieure à la taille maximale du serpent
     * 
     */
    if (*taille_actuelle < TAILLE_SERPENT) {
        (*taille_actuelle)++;
    }
}

/**
 * \brief Efface le caractère à la position donnée.
 * 
 * \param x Position en abscisse.
 * \param y Position en ordonnée.
 */
void effacer(int x, int y)
{
    gotoXY(x, y);
    printf(" ");
}
