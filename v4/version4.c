/**
* \file version4.c
* \brief le jeu snake
* \author Merrien Ethan
* \version V4
* \date 18/11/2024
*
* Dans cette quatrième version,
* le serpent commence à la position 40,20 et progresse vers la droite,
* sa progression peut changer de direction avec les touches q, z, s, d,
* le serpent arrête sa progression lorsque l'utilisateur appuie sur la touche "a"
* Ou lorsqu'il rentre en collision avec lui-même, un pavé ou les bordures.
* Sauf si il sort par les espaces de la bordure
* L'utilisateur peut gagner la partie en mangeant 10 pommes
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAXTAB_X 80 //constante pour la Taille Max d’un tableau
#define MAXTAB_Y 40
#define MINTAB 1 //constante pour la Taille Min d’un tableau
#define TAILLE_SERPENT 10 //constante pour la Taille du serpent
#define DEPARTX 40 //constante pour la position de départ
#define DEPARTY 20
#define ARRET 'a' //constante pour la touche d'arret du programme
#define TETE 'O' //constante pour la tete du serpent
#define ANNEAUX 'X' //constante pour les anneaux du serpent
#define HAUT 'z' //constante pour la touche de direction "haut"
#define BAS 's' //constante pour la touche de direction "bas"
#define GAUCHE 'q' //constante pour la touche de direction "gauche"
#define DROITE 'd' //constante pour la touche de direction "droite"
#define BORDURE '#' //constante pour les bordures du plateau de jeu
#define ESPACE ' ' //constante pour le l'intérieure des bordures du plateau
#define POMME '6' //constante pour le caractère des pommes
#define MAXPOMME 10 //constante pour le nombre de pommes à mangé pour gagner

char plateau[MAXTAB_Y][MAXTAB_X];
int tailleActuelle;

/**
 * \brief Affiche un caractère à une position donnée.
 * \param x Position en abscisse.
 * \param y Position en ordonnée.
 * \param c Caractère à afficher.
 */
void afficher(int x, int y, char c);

void gotoXY(int x, int y);

/**
 * \brief Efface le caractère à une position donnée.
 * \param x Position en abscisse.
 * \param y Position en ordonnée.
 */
void effacer(int x, int y);

/**
 * \brief Dessine le serpent sur le champ de jeu.
 * \param lesX Tableau des positions x de chaque segment du serpent.
 * \param lesY Tableau des positions y de chaque segment du serpent.
 */
void dessinerSerpent(int lesX[], int lesY[]);

/**
 * \brief Met à jour la position du serpent dans la direction indiquée.
 * \param lesX Tableau des positions x de chaque segment du serpent.
 * \param lesY Tableau des positions y de chaque segment du serpent.
 * \param direction Direction actuelle du serpent.
 * \param Collision Collision avec un élement du plateau ou le serpent lui-même.
 */
void progresser(int lesX[], int lesY[], char direction, bool *collision, bool *pomme);

/**
 * \brief Désactive l'affichage des caractères tapés dans le terminal.
 */
void disableEcho();

/**
 * \brief Réactive l'affichage des caractères tapés dans le terminal.
 */
void enableEcho();

/**
 * \brief Initialise les elements de plateau de jeu donner en paramètres.
 *
 * \param plateau Le plateau de jeu
 * \param lesX la position du serpent en abscisses
 * \param lesY la position du serpent en ordonnées
 */
void initPlateau(char plateau[MAXTAB_Y][MAXTAB_X], int lesX[], int lesY[]);

/**
 * \brief Affiche les elements de plateau de jeu donner en paramètres.
 *
 * \param plateau Le plateau de jeu
 */
void dessinerPlateau(char plateau[MAXTAB_Y][MAXTAB_X]);

void ajouterPomme(int lesX[], int lesY[]);

int kbhit();

int main()
{
    int i;
    int lesX[TAILLE_SERPENT + MAXPOMME], lesY[TAILLE_SERPENT + MAXPOMME];
    char cle = DROITE; // Direction actuelle
    char ancienneCle = DROITE;
    char nouvelleCle = ' ';
    int temporisation = 200000;
    int pommeMange = 0;
    bool collision = false;

    lesX[0] = DEPARTX; //coordonnées de départ du sepent
    lesY[0] = DEPARTY;

    for (i = 1; i < TAILLE_SERPENT; i++) { //initialise les positions du serpent
        lesX[i] = lesX[0] - i;
        lesY[i] = lesY[0];
    }
    
    system("clear");

    initPlateau(plateau, lesX, lesY);
    dessinerPlateau(plateau);

    tailleActuelle = TAILLE_SERPENT;

    disableEcho();
    ajouterPomme(lesX, lesY);
    while (cle != ARRET && collision == false && pommeMange < MAXPOMME) {  //Boucle principale 
        bool pomme = false;
        for (i = 0; i < tailleActuelle; i++) { //Boucle permettant d'effacer l'ancienne position du serpent
            effacer(lesX[i], lesY[i]);
        }
        gotoXY(90, 20);
        printf("Pomme mangées: %d", pommeMange);

        progresser(lesX, lesY, cle, &collision, &pomme);

        if (pomme == true){
            temporisation = temporisation - 15000;
            pommeMange++;
            tailleActuelle++;
            ajouterPomme(lesX, lesY);

        }
        
        dessinerSerpent(lesX, lesY);

        usleep(temporisation);

        if (kbhit()){ 
            nouvelleCle = getchar();
            while(nouvelleCle == 'p'){
                usleep(temporisation);
                if (kbhit()){ 
                    nouvelleCle = getchar();
                }
            }
            if ((nouvelleCle == DROITE && ancienneCle != GAUCHE) || // Boucle pour empêcher les directions opposées
                (nouvelleCle == GAUCHE && ancienneCle != DROITE) ||
                (nouvelleCle == HAUT && ancienneCle != BAS) ||
                (nouvelleCle == BAS && ancienneCle != HAUT) || 
                (nouvelleCle == ARRET)) {
                cle = nouvelleCle;  // Met à jour la direction uniquement si elle n'est pas opposée
            }
        }
        ancienneCle = cle;
    }
    enableEcho();
    system("clear");
    if(pommeMange == MAXPOMME){
        printf("YOU WIN !");
    }
    else{
        printf("GAME OVER !");
    }
    return EXIT_SUCCESS;
}


int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}

void gotoXY(int x, int y) { 
    printf("\033[%d;%df", y, x);
}


void dessinerSerpent(int lesX[], int lesY[]){
    int i;

    if (lesX[0] >= MINTAB && lesX[0] < MAXTAB_X && lesY[0] >= MINTAB && lesY[0] < MAXTAB_Y) { // Affiche la tête si elle est dans les limites
        afficher(lesX[0], lesY[0], TETE);
    }

    for (i = 1; i < tailleActuelle; i++) { // Affiche les anneaux du corps si dans les limites
        if (lesX[i] >= MINTAB && lesX[i] < MAXTAB_X && lesY[i] >= MINTAB && lesY[i] < MAXTAB_Y) {
            afficher(lesX[i], lesY[i], ANNEAUX);
        }
    }
}

void afficher(int x, int y, char c){
    gotoXY(x, y);
    printf("%c", c);
}

void progresser(int lesX[], int lesY[], char Direction, bool *collision, bool *pomme) {
    int nouvelleTeteX = lesX[0];
    int nouvelleTeteY = lesY[0];

    // Calculer la nouvelle position selon la direction
    if (Direction == DROITE) {
        nouvelleTeteX += 1;
    } else if (Direction == GAUCHE) {
        nouvelleTeteX -= 1;
    } else if (Direction == HAUT) {
        nouvelleTeteY -= 1;
    } else if (Direction == BAS) {
        nouvelleTeteY += 1;
    }

    if (plateau[nouvelleTeteY][nouvelleTeteX] == BORDURE){
        *collision = true;
    }
    else{
        // Gestion de la sortie par les bordures
        if (nouvelleTeteX >= MAXTAB_X - 1) {  // Sortie par la droite
            nouvelleTeteX = MINTAB;  // Réapparaît à gauche
        } else if (nouvelleTeteX <= 0) {  // Sortie par la gauche
            nouvelleTeteX = MAXTAB_X - 2;  // Réapparaît à droite
        }

        if (nouvelleTeteY >= MAXTAB_Y - 1) {  // Sortie par le bas
            nouvelleTeteY = MINTAB;  // Réapparaît en haut
        } else if (nouvelleTeteY <= 0) {  // Sortie par le haut
            nouvelleTeteY = MAXTAB_Y - 2;  // Réapparaît en bas
        }
    }

    // Vérifier les collisions avec le corps du serpent
    for (int i = 0; i < tailleActuelle; i++) {
        if (nouvelleTeteX == lesX[i] && nouvelleTeteY == lesY[i]) {
            *collision = true;
        }
    }

    // Vérifier si la tête rencontre une pomme
    if (plateau[nouvelleTeteY][nouvelleTeteX] == POMME) {
        *pomme = true;
    }

    // Mettre à jour les positions du corps
    for (int i = tailleActuelle - 1; i > 0; i--) {
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }

    // Mettre à jour la tête
    lesX[0] = nouvelleTeteX;
    lesY[0] = nouvelleTeteY;
}

void effacer(int x, int y)
{
    gotoXY(x, y);
    printf(" ");
}

void disableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminalchar *plateau[MAXTAB_Y][MAXTAB_X]
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void enableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void initPlateau(char plateau[MAXTAB_Y][MAXTAB_X], int lesX[], int lesY[]) {
    int i, j, k, aleatX, aleatY;
    srand(time(NULL));

    // Initialiser les bords haut et bas
    for (i = 0; i < MAXTAB_X; i++) {
        if (i == MAXTAB_X / 2) {
            plateau[0][i] = ESPACE; // Milieu
            plateau[MAXTAB_Y - 1][i] = ESPACE;
        }
        else {
            plateau[0][i] = BORDURE;
            plateau[MAXTAB_Y - 1][i] = BORDURE;
        }
    }

    // Initialiser les bords gauche et droit, et les espaces intérieurs
    for (j = MINTAB; j < MAXTAB_Y - 1; j++) {
        if (j == MAXTAB_Y / 2) {
            plateau[j][0] = ESPACE;
            plateau[j][MAXTAB_X - 1] = ESPACE;
        }
        else {
            plateau[j][0] = BORDURE;
            plateau[j][MAXTAB_X - 1] = BORDURE;
        }

        for (i = 1; i < MAXTAB_X - 1; i++) {
            plateau[j][i] = ESPACE; // Remplissage intérieur
        }
    }

    // Créer 5 carrés aléatoires
    for (k = 0; k < 5; k++) {
        bool positionValide = false;
        while (!positionValide) {
            aleatX = rand() % (MAXTAB_X - 6) + 1; // Espace entre les bordures
            aleatY = rand() % (MAXTAB_Y - 6) + 1;

            positionValide = true;
            // Vérifier qu'il n'y a pas de collision avec les bords ou autres carrés
            for (j = 0; j < 5 && positionValide; j++) {
                for (i = 0; i < 5 && positionValide; i++) {
                    if (plateau[aleatY + j][aleatX + i] != ESPACE) {
                        positionValide = false;
                    }
                }
            }

            // Vérifier qu'il n'y a pas de collision avec le serpent
            for (j = 0; j < TAILLE_SERPENT && positionValide; j++) {
                for (int y = 0; y < 5 && positionValide; y++) {
                    for (int x = 0; x < 5 && positionValide; x++) {
                        if (aleatX + x == lesX[j] && aleatY + y == lesY[j]) {
                            positionValide = false;
                        }
                    }
                }
            }
        }

        // Initialiser le carré une fois que la position est valide
        for (j = 0; j < 5; j++) {
            for (i = 0; i < 5; i++) {
                plateau[aleatY + j][aleatX + i] = BORDURE;
            }
        }
    }
}

void dessinerPlateau(char plateau[MAXTAB_Y][MAXTAB_X]) {
    int i, j;
    for (i = 0; i < MAXTAB_Y; i++) { 
        for (j = 0; j < MAXTAB_X; j++) {
            afficher(j + 1, i + 1, plateau[i][j]);
        }
    }
}


void ajouterPomme(int lesX[], int lesY[]){
    int aleatX, aleatY;
    srand(time(NULL));
    bool positionValide = false;
    while (positionValide == false){
            aleatX = (rand() % (MAXTAB_X - 2)) + 1;// créer des coordonnées aléatoire avec un espace entre les bordures et les carrés
            aleatY = (rand() % (MAXTAB_Y - 2)) + 1;

            positionValide = true; // Boucle pour empecher les carrés d'apparaitre sur les bordures
            if (plateau[aleatY][aleatX] != ESPACE) {
                positionValide = false;
            }

            for (int j = 0; j < TAILLE_SERPENT; j++) { // Boucle pour empecher les carrés d'apparaitre sur le serpent
                if ((aleatX == lesX[j] && aleatY == lesY[j])) {
                    positionValide = false;
                }
            }
    }
    plateau[aleatY][aleatX] = POMME;
    afficher(aleatX, aleatY, POMME);
}