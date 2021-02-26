/*
	Canvas pour algorithmes de jeux à 2 joueurs
	
	joueur 0 : humain
	joueur 1 : ordinateur
			
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Paramètres du jeu
#define LARGEUR_MAX 7 		// nb max de fils pour un noeud (= nb max de coups possibles)

#define TEMPS 10		// temps de calcul pour un coup avec MCTS (en secondes)

// macros
#define AUTRE_JOUEUR(i) (1-(i))
#define min(a, b)       ((a) < (b) ? (a) : (b))
#define max(a, b)       ((a) < (b) ? (b) : (a))
#define LARGEUR_GRILLE 7
#define HAUTEUR_GRILLE 6

// Critères de fin de partie
typedef enum {NON, MATCHNUL, ORDI_GAGNE, HUMAIN_GAGNE } FinDePartie;

// Definition du type Etat (état/position du jeu)
typedef struct EtatSt {

	int joueur; // à qui de jouer ? 

	// TODO: à compléter par la définition de l'état du jeu

	/* Pour un puissance 4*/
	char plateau[HAUTEUR_GRILLE][LARGEUR_GRILLE];	

} Etat;

// Definition du type Coup
typedef struct {
	// TODO: à compléter par la définition d'un coup 

	/* Pour un puissance 4*/
	int colonne;

} Coup;

// Copier un état 
Etat * copieEtat( Etat * src ) {
	Etat * etat = (Etat *)malloc(sizeof(Etat));

	etat->joueur = src->joueur;
	
		
	// TODO: à compléter avec la copie de l'état src dans etat
	
	/* par exemple : */
	int i,j;	
	for (i=0; i< HAUTEUR_GRILLE; i++)
		for ( j=0; j< LARGEUR_GRILLE; j++)
			etat->plateau[i][j] = src->plateau[i][j];
	

	
	return etat;
}

// Etat initial 
Etat * etat_initial( void ) {
	Etat * etat = (Etat *)malloc(sizeof(Etat));
	
	// TODO: à compléter avec la création de l'état initial
	
	/* par exemple : */
	int i,j;	
	for (i=0; i< HAUTEUR_GRILLE; i++)
		for ( j=0; j< LARGEUR_GRILLE; j++)
			etat->plateau[i][j] = ' ';
	
	return etat;
}


void afficheJeu(Etat * etat) {

	// TODO: à compléter

	/* par exemple : */
	int i,j;
	printf("   |");
	for ( j = 0; j < LARGEUR_GRILLE; j++) 
		printf(" %d |", j);
	printf("\n");
	printf("   -----------------------------");
	printf("\n");
	
	for(i=0; i < HAUTEUR_GRILLE; i++) {
		printf("   |");
		for ( j = 0; j < LARGEUR_GRILLE; j++) 
			printf(" %c |", etat->plateau[i][j]);
		printf("\n");
		printf("   -----------------------------");
		printf("\n");
	}
}


// Nouveau coup 
// TODO: adapter la liste de paramètres au jeu
Coup * nouveauCoup( int j ) {
	Coup * coup = (Coup *)malloc(sizeof(Coup));
	
	// TODO: à compléter avec la création d'un nouveau coup
	
	/* par exemple : */
	coup->colonne = j;
	
	return coup;
}

// Demander à l'humain quel coup jouer 
Coup * demanderCoup () {

	// TODO...

	/* par exemple : */
	int j;
	printf(" quelle colonne ? ") ;
	scanf("%d",&j); 
	
	return nouveauCoup(j);
}

// Modifier l'état en jouant un coup 
// retourne 0 si le coup n'est pas possible
int jouerCoup( Etat * etat, Coup * coup ) {

	// TODO: à compléter
	
	/* par exemple : */
	if ( etat->plateau[0][coup->colonne] == 'O' || etat->plateau[0][coup->colonne] == 'X')
		//Cas où la colonne choisie est déjà pleine
		return 0;
	else {
		//etat->plateau[coup->ligne][coup->colonne] = etat->joueur ? 'O' : 'X';
		
		//On part du haut de la colonne choisi ( la colonne  de numéro coup->colonne) et on va augmenter le numéro de la ligne en faisant ligne ++ , 
		//On part donc de la ligne 0 (haut de la grille) et on descend jusqu'a atteindre au max 6
		//une fois qu'on a trouvé le numéro de la ligne on place le coup joué
		int ligne = 0;
		while(etat->plateau[ligne][coup->colonne] == ' '){
			ligne++;
			if(ligne == 6){break;}
		}
		etat->plateau[ligne-1][coup->colonne] = etat->joueur ? 'O' : 'X';

		// à l'autre joueur de jouer
		etat->joueur = AUTRE_JOUEUR(etat->joueur); 	

		return 1;
	}	
}

// Retourne une liste de coups possibles à partir d'un etat 
// (tableau de pointeurs de coups se terminant par NULL)
Coup ** coups_possibles( Etat * etat ) {
	
	Coup ** coups = (Coup **) malloc((1+LARGEUR_MAX) * sizeof(Coup *) );
	
	int k = 0;
	
	// TODO: à compléter
	
	/* par exemple */
	int i;
	for(i=0; i < LARGEUR_GRILLE; i++) {
		/*for (j=0; j < HAUTEUR_GRILLE; j++) {
			if ( etat->plateau[i][j] == ' ' ) {
				coups[k] = nouveauCoup(i,j); 
				k++;
			}
		}*/
		//Au puissance 4 un coup est possible quand la haut de la grille est libre sur uen colonne
		if ( etat->plateau[0][i] == ' ' ) {
				coups[k] = nouveauCoup(i); 
				k++;
		}
	}
	/* fin de l'exemple */
	
	coups[k] = NULL;

	return coups;
}


// Definition du type Noeud 
typedef struct NoeudSt {
		
	int joueur; // joueur qui a joué pour arriver ici
	Coup * coup;   // coup joué par ce joueur pour arriver ici
	
	Etat * etat; // etat du jeu
			
	struct NoeudSt * parent; 
	struct NoeudSt * enfants[LARGEUR_MAX]; // liste d'enfants : chaque enfant correspond à un coup possible
	int nb_enfants;	// nb d'enfants présents dans la liste
	
	// POUR MCTS:
	int nb_victoires;
	int nb_simus;
	
} Noeud;


// Créer un nouveau noeud en jouant un coup à partir d'un parent 
// utiliser nouveauNoeud(NULL, NULL) pour créer la racine
Noeud * nouveauNoeud (Noeud * parent, Coup * coup ) {
	Noeud * noeud = (Noeud *)malloc(sizeof(Noeud));
	
	if ( parent != NULL && coup != NULL ) {
		noeud->etat = copieEtat ( parent->etat );
		jouerCoup ( noeud->etat, coup );
		noeud->coup = coup;			
		noeud->joueur = AUTRE_JOUEUR(parent->joueur);		
	}
	else {
		noeud->etat = NULL;
		noeud->coup = NULL;
		noeud->joueur = 0; 
	}
	noeud->parent = parent; 
	noeud->nb_enfants = 0; 
	
	// POUR MCTS:
	noeud->nb_victoires = 0;
	noeud->nb_simus = 0;	
	

	return noeud; 	
}

// Ajouter un enfant à un parent en jouant un coup
// retourne le pointeur sur l'enfant ajouté
Noeud * ajouterEnfant(Noeud * parent, Coup * coup) {
	Noeud * enfant = nouveauNoeud (parent, coup ) ;
	parent->enfants[parent->nb_enfants] = enfant;
	parent->nb_enfants++;
	return enfant;
}

void freeNoeud ( Noeud * noeud) {
	if ( noeud->etat != NULL)
		free (noeud->etat);
		
	while ( noeud->nb_enfants > 0 ) {
		freeNoeud(noeud->enfants[noeud->nb_enfants-1]);
		noeud->nb_enfants --;
	}
	if ( noeud->coup != NULL)
		free(noeud->coup); 

	free(noeud);
}
	

// Test si l'état est un état terminal 
// et retourne NON, MATCHNUL, ORDI_GAGNE ou HUMAIN_GAGNE
FinDePartie testFin( Etat * etat ) {

	// TODO...
	
	/* Pour puissance 4*/
	int i,j,k,nbCoup = 0;
	for ( i=0;i < HAUTEUR_GRILLE; i++) {
		for(j=0; j < LARGEUR_GRILLE; j++) {
			if ( etat->plateau[i][j] != ' ') {
				nbCoup++;	// nb coups joués
			
				// colonnes
				k=0;
				while ( k < 4 && i+k < HAUTEUR_GRILLE && etat->plateau[i+k][j] == etat->plateau[i][j] ) 
					k++;
				if ( k == 4 ){
					//printf("GAGNE EN VERTICALE\n");
					return etat->plateau[i][j] == 'O'? ORDI_GAGNE : HUMAIN_GAGNE;
				}

				// lignes
				k=0;
				while ( k < 4 && j+k < LARGEUR_GRILLE && etat->plateau[i][j+k] == etat->plateau[i][j] ) 
					k++;
				if ( k == 4 ){
					//printf("GAGNE EN HORIZONTALE\n");
					return etat->plateau[i][j] == 'O'? ORDI_GAGNE : HUMAIN_GAGNE;
				}

				// diagonales
				k=0;
				while ( k < 4 && i+k < HAUTEUR_GRILLE && j+k < LARGEUR_GRILLE && etat->plateau[i+k][j+k] == etat->plateau[i][j] ) 
					k++;
				if ( k == 4){
					//printf("GAGNE EN DIAGONALE DROITE\n");
					return etat->plateau[i][j] == 'O'? ORDI_GAGNE : HUMAIN_GAGNE;
				}

				k=0;
				while ( k < 4 && i+k < HAUTEUR_GRILLE && j-k >= 0 && etat->plateau[i+k][j-k] == etat->plateau[i][j] ) 
					k++;
				if ( k == 4 ){
					//printf("GAGNE EN DIAGONALE GAUCHE\n");
					return etat->plateau[i][j] == 'O'? ORDI_GAGNE : HUMAIN_GAGNE;
				}
			}
		}
	}

	// et sinon tester le match nul	
	if ( nbCoup == LARGEUR_GRILLE*HAUTEUR_GRILLE ) 
		return MATCHNUL;
		
	return NON;
}



// Calcule et joue un coup de l'ordinateur avec MCTS-UCT
// en tempsmax secondes
void ordijoue_mcts(Etat * etat, int tempsmax) {

	clock_t tic, toc;
	tic = clock();
	int temps;

	Coup ** coups;
	Coup * meilleur_coup ;
	
	// Créer l'arbre de recherche

	Noeud * racine = nouveauNoeud(NULL, NULL);
	racine->etat = copieEtat(etat); 
	Noeud * current = racine; 
	Noeud * noeud_depart;
	// créer les premiers noeuds:
	coups = coups_possibles(current->etat); 
	int k = 0;
	Noeud * enfant;
	while ( coups[k] != NULL) {
		enfant = ajouterEnfant(current, coups[k]);
		//printf("Coup %d : %d\n", k, coups[k]->colonne);  //DEBUG
		k++;
	}
	
	

	int iter = 0;
	
	do {

		current = racine; 

		int indexNoeudMaxUCB;
		float maxUCB,currentUCB = 0;
		int noeud_pas_visite = 0;
		
		/* 1. Selectionner le bon noeud*/
		//Tant que on trouve pas un noeud pas developpé on descend dans l'arbre en maximisant la valeur UCB
		while (current && current->nb_enfants != 0 && !noeud_pas_visite){
			//printf("SEG FAULT 0\n");
			for(int i = 0 ; i < current->nb_enfants;i++){
				//Si on a jamais visité ce noeud on le prend
				if(current->enfants[i]->nb_simus == 0){
					
					indexNoeudMaxUCB = i;
					noeud_pas_visite = 1;
					break;
				}else{
					
					//Sinon on calcule la valeur UCB du noeud
					currentUCB = (current->enfants[i]->nb_victoires/current->enfants[i]->nb_simus)+1.4*sqrt(log(current->nb_simus)/current->enfants[i]->nb_simus);
					if(maxUCB < currentUCB){
						maxUCB = currentUCB;
						indexNoeudMaxUCB = i;
					}
					//printf("i = %d et currentNB_CHILD = %d\n",i,current->nb_enfants);		//DEBUG
				
				}	
			}
			current = current->enfants[indexNoeudMaxUCB];
		}
		
		noeud_depart = current;
		
		
		if(current){
			/* 2. Developper le noeud choisi*/
			//printf("STEP 2\n");
			coups = coups_possibles(current->etat); 
			k = 0;
			while ( coups[k] != NULL) {
				enfant = ajouterEnfant(current, coups[k]);
				k++;
			}
			

			/* 3. Marche aléatoire depuis le noeud courant*/
			Coup * coup_aleatoire;
			Noeud * noeud_pour_marche = nouveauNoeud(NULL,NULL);
			noeud_pour_marche->etat = copieEtat(noeud_depart->etat);
			FinDePartie test_fin = NON;
			while(test_fin == NON){
				coups = coups_possibles(noeud_pour_marche->etat);
				k = 0;
				while ( coups[k] != NULL) {
					k++;
				}
				//printf("k = %d\n", k);		//DEBUG
				if(k == 0){break;}
				coup_aleatoire = coups[ rand()%k ];

				noeud_pour_marche = ajouterEnfant(noeud_pour_marche,coup_aleatoire);
				test_fin = testFin(noeud_pour_marche->etat);
				
			}
			freeNoeud(noeud_pour_marche);


			/* 4. Mettre à jour les valeurs de noeuds*/
			while(noeud_depart != racine){
				if(test_fin==2){
					noeud_depart->nb_victoires++;
				}
				noeud_depart->nb_simus++;
				noeud_depart = noeud_depart->parent;
				
			}
		}
		//printf("FIN ALGO\n");			//DEBUG
		//printf("iter %d\n",iter );	//DEBUG
		toc = clock(); 
		temps = (int)( ((double) (toc - tic)) / CLOCKS_PER_SEC );
		iter ++;
		
	} while (temps < tempsmax);
	
	int index_best_WR = 0;
	float best_WR,WR = 0.0f;


	/*Choix du coup à jouer*/
	for(int i =0 ; i < racine->nb_enfants;i++){
		printf("WIN RATE enfant %d = %f \n",i,(float)racine->enfants[i]->nb_victoires/racine->enfants[i]->nb_simus);
		WR = (float)racine->enfants[i]->nb_victoires/racine->enfants[i]->nb_simus;
		if(best_WR < WR){
			index_best_WR = i;
			best_WR = WR;
		}
	}

	printf("ORDINATEUR joue la colonne %d avec %f de winrate\n",index_best_WR,best_WR );
	
	coups = coups_possibles(racine->etat); 
	// Jouer le meilleur premier coup
	jouerCoup(etat, coups[ index_best_WR]); 

	/* fin de l'algorithme  */

	// Penser à libérer la mémoire :
	freeNoeud(racine);
	free (coups);
}

int main(void) {

	Coup * coup;
	FinDePartie fin;
	
	// initialisation
	Etat * etat = etat_initial(); 
	
	// Choisir qui commence : 
	printf("Qui commence (0 : humain, 1 : ordinateur) ? ");
	scanf("%d", &(etat->joueur) );
	
	// boucle de jeu
	do {
		printf("\n");
		afficheJeu(etat);
		
		if ( etat->joueur == 0 ) {
			// tour de l'humain
			
			do {
				coup = demanderCoup();
			} while ( !jouerCoup(etat, coup) );
									
		}
		else {
			// tour de l'Ordinateur
			
			ordijoue_mcts( etat, TEMPS );
			
		}
		
		fin = testFin( etat );
	}	while ( fin == NON ) ;

	printf("\n");
	afficheJeu(etat);
		
	if ( fin == ORDI_GAGNE )
		printf( "** L'ordinateur a gagné **\n");
	else if ( fin == MATCHNUL )
		printf(" Match nul !  \n");
	else
		printf( "** BRAVO, l'ordinateur a perdu  **\n");
	return 0;
}