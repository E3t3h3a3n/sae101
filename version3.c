/**
* \file version2.c
* \brief le jeu snake
* \author Merrien Ethan
* \version V2
* \date 4/11/2024
*
* Dans cette troisième version,
* le serpent commence à la position 40,20 et progresse vers la droite,
* sa progression peut changer de direction avec les touches q, z, s, d,
* le serpent arrête sa progression lorsque l'utilisateur appuie sur la touche "a"
* Ou lorsqu'il rentre en collision avec lui-même, un pavé ou les bordures.
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
#define TEMPORISATION 200000 //constante pour le temps entre chaque progression
#define DEPART 20 //constante pour la position de départ
#define ARRET 'a' //constante pour la touche d'arret du programme
#define TETE 'o' //constante pour la tete du serpent
#define ANNEAUX 'x' //constante pour les anneaux du serpent
#define HAUT 'z' //constante pour la touche de direction "haut"
#define BAS 's' //constante pour la touche de direction "bas"
#define GAUCHE 'q' //constante pour la touche de direction "gauche"
#define DROITE 'd' //constante pour la touche de direction "droite"
#define BORDURE '#' //constante pour les bordures du plateau de jeu
#define ESPACE ' ' //constante pour le l'intérieure des bordures du plateau

char plateau[MAXTAB_Y][MAXTAB_X];

/**
 * \brief Affiche un caractère à une position donnée.
 * \param x Position en abscisse.
 * \param y Position en ordonnée.
 * \param c Caractère à afficher.
 */
void afficher(int x, int y, char c);

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
 * \param TailleActuelle Nombre de segments actuellement actifs dans le serpent.
 */
void dessinerSerpent(int lesX[], int lesY[], int TailleActuelle);

/**
 * \brief Met à jour la position du serpent dans la direction indiquée.
 * \param lesX Tableau des positions x de chaque segment du serpent.
 * \param lesY Tableau des positions y de chaque segment du serpent.
 * \param direction Direction actuelle du serpent.
 * \param Collision Collision avec un élement du plateau ou le serpent lui-même.
 */
void progresser(int lesX[], int lesY[], char direction, bool *Collision);

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
 * \param TailleActuelle la taille actuelle du serpent
 */
void initPlateau(char plateau[MAXTAB_Y][MAXTAB_X], int lesX[], int lesY[], int TailleActuelle);

/**
 * \brief Affiche les elements de plateau de jeu donner en paramètres.
 *
 * \param plateau Le plateau de jeu
 */
void dessinerPlateau(char plateau[MAXTAB_Y][MAXTAB_X]);

int kbhit();

int main()
{
    int i;
    int lesX[TAILLE_SERPENT], lesY[TAILLE_SERPENT];
    char Key = DROITE; // Direction actuelle
    char LastKey = DROITE;
    char NewKey = ' ';
    int TailleActuelle;
    bool Collision = false;

    lesX[0] = DEPART; //coordonnées de départ du sepent
    lesY[0] = DEPART;

    for (i = 1; i < TAILLE_SERPENT; i++) { //initialise les positions du serpent
        lesX[i] = lesX[0] - i;
        lesY[i] = lesY[0];
    }
    
    system("clear");

    initPlateau(plateau, lesX, lesY, TAILLE_SERPENT);
    dessinerPlateau(plateau);

    TailleActuelle = TAILLE_SERPENT;

    disableEcho();
    while (Key != ARRET && Collision == false) {  //Boucle principale 
        for (i = 0; i < TailleActuelle; i++) { //Boucle permettant d'effacer l'ancienne position du serpent
            effacer(lesX[i], lesY[i]);
        }

        progresser(lesX, lesY, Key, &Collision);
        dessinerSerpent(lesX, lesY, TailleActuelle);

        usleep(TEMPORISATION);

        if (kbhit()){ 
            NewKey = getchar();
            if ((NewKey == DROITE && LastKey != GAUCHE) || // Boucle pour empêcher les directions opposées
                (NewKey == GAUCHE && LastKey != DROITE) ||
                (NewKey == HAUT && LastKey != BAS) ||
                (NewKey == BAS && LastKey != HAUT) || 
                (NewKey == ARRET)) {
                Key = NewKey;  // Met à jour la direction uniquement si elle n'est pas opposée
            }
        }
        LastKey = Key;

    }
    enableEcho();
    system("clear");
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


void dessinerSerpent(int lesX[], int lesY[], int TailleActuelle){
    int i;

    if (lesX[0] >= MINTAB && lesX[0] < MAXTAB_X && lesY[0] >= MINTAB && lesY[0] < MAXTAB_Y) { // Affiche la tête si elle est dans les limites
        afficher(lesX[0], lesY[0], TETE);
    }

    for (i = 1; i < TailleActuelle; i++) { // Affiche les anneaux du corps si dans les limites
        if (lesX[i] >= MINTAB && lesX[i] < MAXTAB_X && lesY[i] >= MINTAB && lesY[i] < MAXTAB_Y) {
            afficher(lesX[i], lesY[i], ANNEAUX);
        }
    }
}

void afficher(int x, int y, char c){
    gotoXY(x, y);
    printf("%c", c);
}

void progresser(int lesX[], int lesY[], char Direction, bool *Collision) {
    int TailleActuelle = TAILLE_SERPENT; // Taille fixée du serpent
    int nouvelleTeteX = lesX[0];
    int nouvelleTeteY = lesY[0];

    if (Direction == DROITE) {// Déterminer la nouvelle position de la tête selon la direction
        nouvelleTeteX += 1;
    } else if (Direction == GAUCHE) {
        nouvelleTeteX -= 1;
    } else if (Direction == HAUT) {
        nouvelleTeteY -= 1;
    } else if (Direction == BAS) {
        nouvelleTeteY += 1;
    }

    *Collision = false;
    for (int i = 0; i < TailleActuelle; i++) { // Vérifier les collisions avec le corps du serpent
        if (nouvelleTeteX == lesX[i] && nouvelleTeteY == lesY[i]) {
            *Collision = true;
        }
    }

    if (plateau[nouvelleTeteY][nouvelleTeteX] != ESPACE) {// Vérifier les collisions avec les bordures ou obstacles
        *Collision = true;
    }

    for (int i = TailleActuelle - 1; i > 0; i--) {// Si pas de collision, mettre à jour les positions du serpent
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }

    lesX[0] = nouvelleTeteX; // Mettre à jour la position de la tête
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

void initPlateau(char plateau[MAXTAB_Y][MAXTAB_X], int lesX[], int lesY[], int TailleActuelle) {
    int i, j, k, AleatX, AleatY;
    srand(time(NULL));

    for (i = MINTAB; i < MAXTAB_X; i++){ //boucle pour initialiser la ligne du haut et celle du bas
        plateau[MINTAB][i] = BORDURE;
        plateau[MAXTAB_Y - 1][i] = BORDURE;
    }
    for (j = MINTAB + 1; j < MAXTAB_Y - 1; j++){//boucle pour initialiser les lignes intermédiaires
        plateau[j][MINTAB] = BORDURE;
        for (i = MINTAB + 1; i < MAXTAB_X - 1; i++){
            plateau[j][i] = ESPACE;
        }
        plateau[j][MAXTAB_X - 1] = BORDURE;
    }

    for (k = 0; k < 5; k++) { // créer 5 carrés
        bool positionValide = false;
        while (positionValide == false){
            AleatX = (rand() % (MAXTAB_X - 9)) + 3;// créer des coordonnées aléatoire avec un espace entre les bordures et les carrés
            AleatY = (rand() % (MAXTAB_Y - 9)) + 3;

            positionValide = true; // Boucle pour empecher les carrés d'apparaitre sur les bordures
            for (j = 0; j < 5; j++) {
                for (i = 0; i < 5; i++) {
                    if (plateau[AleatY + j][AleatX + i] != ESPACE) {
                        positionValide = false;
                    }
                }
            }

            for (j = 0; j < TAILLE_SERPENT; j++) { // Boucle pour empecher les carrés d'apparaitre sur le serpent
                for (int y = 0; y < 5; y++) {
                    for (int x = 0; x < 5; x++) {
                        if ((AleatX + x == lesX[j] && AleatY + y == lesY[j])) {
                            positionValide = false;
                        }
                    }
                }
            }
        }

        for (j = 0; j < 5; j++){ // initialiser le carré une fois que les coordonnées sont valide
            for (i = 0; i < 5; i++) {
                plateau[AleatY + j][AleatX + i] = BORDURE;
            }
        }
    }
}


void dessinerPlateau(char plateau[MAXTAB_Y][MAXTAB_X]) {
    int i, j;

    for (i = MINTAB; i < MAXTAB_Y; i++){ //boucle qui parcours chaque element du tableau et l'affiche
        for (j = MINTAB; j < MAXTAB_X; j++) {
            printf("%c", plateau[i][j]);
        }
        printf("\n");
    }
}