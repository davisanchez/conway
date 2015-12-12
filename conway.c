//-------------------------------------------------------------------------------
// sciper: 248127
// nom: Sanchez Del Rio
// prenom: David
// fichier: conway.c
// date: 03.12.2015
// description du programme: projet PROG I Automne 2015-16 EPFL MT-EL
//-------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#define ZOOM_MAX 100
#define ZOOM_MIN 0
#define DIV_INT 2
#define FMT 3
#define NB_COL_MAX 70
#define MIN_CHANGE_NOIR 2
#define MAX_CHANGE_NOIR 3
#define CHANGE_BLANC 3

//-------------------------------------------------------------------------------
//                 NE PAS MODIFIER CES PROTOTYPES DE FONCTIONS

static void erreur_nbJ(int nbJ);
static void erreur_nbS(int nbS);
static void erreur_nbJ_nbS(int nbJ, int nbS);
static void erreur_zoom(int zoom);
int input_verbose();
int input_nbJ(int verbose);
int input_nbS(int verbose);
int input_zoom(int verbose);
void input_format(int verbose);
int check_noir(int i, int j, int nbL, int nbC, int dim [i][j]);
int check_blanc(int i, int j, int nbL, int nbC, int dim [i][j]);
void problems_nbJ_nbS(int nbJ, int nbS);
void scan_array(int verbose, int nbL, int nbC, int dim[nbL][nbC]);
void copy_array(int nbL, int nbC, int image[nbL][nbC], int dim[nbL][nbC]);
void change_colors(int nbL, int nbC, int image[nbL][nbC], int dim[nbL][nbC]);
void print_array(int nbL, int nbC, int image[nbL][nbC]);
void print_zoom(int nbL, int nbC, int image[nbL][nbC], int zoom);
void algorithm(int nbL, int nbC, int image[nbL][nbC], int dim[nbL][nbC], 
											int zoom, int nbJ, int nbS);
void number_of_lines(int number_of_plot_lines, int zoom, int nbL, int nbJ,
									int nbS, int nbC, int dim[nbL][nbC]);
//-------------------------------------------------------------------------------


int main(int argc, const char * argv[]) 
{
	int nbJ = 0, nbS = 0, verbose = 0, zoom = 0;
	int nbL = 0, nbC = 0;
	
	// Entrée des paramètres
	
	verbose = input_verbose();
	nbJ = input_nbJ(verbose);
	nbS = input_nbS(verbose);

	//Verifier les conditions d'erreur

	problems_nbJ_nbS(nbJ, nbS);
	zoom = input_zoom(verbose);

	// Entrée du reste des paramètres

	input_format(verbose);
	
	if(verbose)
		printf("Donner les dimensions du monde\n");
	scanf("%d %d", &nbC ,&nbL);
	
	//Initialisation
		
	int dim[nbL][nbC];
	int image[nbL][nbC];
	int number_of_plot_lines = zoom*nbL;
	
	scan_array(verbose, nbL, nbC, dim);
	
	number_of_lines(number_of_plot_lines, zoom, nbL, nbJ, nbS, nbC, dim);

	//Algorithme
	
	algorithm(nbL, nbC, image, dim, zoom, nbJ, nbS);

    return EXIT_SUCCESS ;
}


//-------------------------------------------------------------------------------
// Fonctions prédéfinies pour indiquer si les données sont correctes
// Les fonctions signalant une erreur provoquent la fin du programme
// en appelant exit(). Leur message d'erreur est toujours affiché.
//
//                 NE PAS MODIFIER CES FONCTIONS
//-------------------------------------------------------------------------------

// A appeler si le nombre de mises à jours n'est pas positif
static void erreur_nbJ(int nbJ) 
{
    printf("Le nombre de mises à jours nbJ n'est pas positif: %d\n", nbJ);
    exit(EXIT_FAILURE);
}

// A appeler si la periode de sauvegarde n'est pas positive
static void erreur_nbS(int nbS) 
{
    printf("La période de sauvegarde nbS n'est pas positive: %d\n", nbS);
    exit(EXIT_FAILURE);
}

// A appeler si la periode de sauvegarde n'est pas positive
static void erreur_nbJ_nbS(int nbJ, int nbS) 
{
    printf("La combinaison du nombre de mises à jour %d et de la période de "
           "sauvegarde %d est interdite\n", nbJ, nbS);
    exit(EXIT_FAILURE);
}

// A appeler si le facteur de zoom n'est pas dans le bon intervalle
static void erreur_zoom(int zoom) 
{
    printf("Le facteur de zoom %d n'est pas compris dans l'intervalle ]0,100]\n",
           zoom);
    exit(EXIT_FAILURE);
}

void problems_nbJ_nbS(int nbJ, int nbS)
{
	if (nbJ < 0)
		erreur_nbJ(nbJ);
		
	if(nbS < 0) 
	erreur_nbS(nbS);
		
	if(nbJ == 0 && nbS > 1)
		erreur_nbJ_nbS(nbJ, nbS);
		
	if(nbJ != 0 && nbJ % nbS != 0)
		erreur_nbJ_nbS(nbJ, nbS);
}

int input_verbose()
{
	int verbose;
	
	// Entrée du verbose
	scanf("%d", &verbose);
	return verbose;

}

int input_nbJ(verbose)
{
	int nbJ;
	
	// Entrée de nbJ
	if (verbose)
		printf("Donner le nombre de mises à jour nbJ\n");
		
	scanf("%d", &nbJ);
		
	return nbJ;
}

int input_nbS(verbose)
{
	int nbS;
	
	// Entrée de nbS
	
	if (verbose)
		printf("Donner le nombre de mises à jour nbJ\n");
		
	scanf("%d", &nbS);
	return nbS;
}

int input_zoom(int verbose)
{
	int zoom = 0;
	
	if (verbose)
		printf("Donner la valeur du zoom\n");

	scanf("%d", &zoom);
		
	if (zoom < ZOOM_MIN || zoom > ZOOM_MAX )
		erreur_zoom(zoom);
		
	return zoom;
}

void input_format(int verbose)
{
	char format[FMT];
	
	if (verbose)	
		printf("Donner le format de l'image\n");
	scanf("%s", format);
} 

//calcule la valeur de la somme des cellules voisines
int calcul_somme(int i, int j, int nbL, int nbC, int dim[nbL][nbC])
{
	int m = 0, n = 0;
	int somme = 0;

	
	//creer un tableau aux alentours de la cellule choisie
	for(m = i-1; m <= i+1; m++)
	{
		for(n = j-1; n <= j+1; n++)
		{
			//ne pas prendre la cellule en elle meme
			if (m == i && n == j)
				continue;
			else
			{
				// Debut/fin du tableau, ne rien faire
				if(m < 0 || m > (nbL - 1) || n < 0 || n > (nbC - 1))
					continue;
				else
				{
					//on somme les valeurs des cellules voisines
					if (dim[m][n] == 1)
					    somme += 1;
				}	
			}
		}
		
	}
			
	return somme;
}

// fonction de passage de la couleur noire a blanche
int check_noir(int i, int j, int nbL, int nbC, int dim[nbL][nbC])
{
	
	int somme = 0;
	int change = 0;
	
	somme = calcul_somme(i, j, nbL, nbC, dim);
	
	if (!(somme == MIN_CHANGE_NOIR || somme == MAX_CHANGE_NOIR))
		//met le change a 1 -> change la couleur dans le tableau image
		change = 1;
			
	return change;
}

//fonction de passage de la couleur blanche a noire
int check_blanc(int i, int j, int nbL, int nbC, int dim[nbL][nbC])
{
	int somme = 0;
	int change = 0;
	
	somme = calcul_somme(i, j, nbL, nbC, dim);
	
	if (somme == CHANGE_BLANC)
		//met le change a 1 -> change la couleur dans le tableau image
		change = 1;

	return change;
}

//scanne le tableau en entrée
void scan_array(int verbose, int nbL, int nbC, int dim[nbL][nbC])
{
	if (verbose)
		printf("Entrez les valeurs du monde: \n");
		
	int i = 0, j = 0;
	
	for (i = 0; i< nbL; i++)
	{
		for (j = 0; j< nbC; j++)
		{
			scanf("%d", &dim[i][j]); 
		}
	}
}

//copie le tableau image1 dans image2
void copy_array(int nbL, int nbC, int image2[nbL][nbC], 
								int image1[nbL][nbC])
{
	int i, j;
	
	for (i = 0; i < nbL; i++)
	{
		for (j = 0; j < nbC; j++)
			image2[i][j] = image1[i][j];  
	}
}

//affiche le tableau de sortie image
void print_array(int nbL, int nbC, int image[nbL][nbC])
{
	int i, j;
		for (i = 0; i < nbL; i++)
		{	
			for (j = 0; j < nbC; j++)
			{
				//condition de retour a la ligne
				if((j+1) % (NB_COL_MAX/2) == 0 && j != 0)
					printf("\n");
					
				printf("%d ", image[i][j]);
			}
			printf("\n");
		}
} 

//conditions pour changer les cases de couleur
void change_colors(int nbL, int nbC, int image1[nbL][nbC],
									int image2[nbL][nbC])
{
	int change = 0;
	int i,j;
	
	for (i = 0; i < nbL; i++)
	{
		for (j = 0; j < nbC; j++)
		{
			if(image2[i][j] == 0)
			    //parcourir le tableau en appelant les fonctions
			    change = check_blanc(i, j, nbL, nbC, image2);
			else
			    change = check_noir(i, j, nbL, nbC, image2); 
			    
			if (change == 1)
		        image1[i][j] = !image2[i][j];
		}
		
	} 
	
}

//Afficher le résultat avec un zoom
void print_zoom(int nbL, int nbC, int image[nbL][nbC], int zoom)
{
	int i = 0, j = 0;
	int m, n;
	int nbL_zoom, nbC_zoom;
	
	nbL_zoom = zoom * nbL;
	nbC_zoom = zoom * nbC;
	
	int tab_zoom[nbL_zoom][nbC_zoom];
	for (i = 0; i < nbL; i++)
	{
		for (j = 0; j < nbC; j++)
		{
			// tableau de taille zoom
			for (m = zoom * i; m < zoom * i + zoom; m++)
			{
				for (n = zoom * j; n < zoom * j + zoom; n++)
					tab_zoom[m][n] = image[i][j];
			}
					
		}
	}
	
	print_array(nbL_zoom, nbC_zoom, tab_zoom); 		
					
} 

// fonction qui appelle toutes les autres pour executer l'algorithme
void algorithm(int nbL, int nbC, int image[nbL][nbC], int dim[nbL][nbC],
											int zoom, int nbJ, int nbS)
{
	int i, j;
		
	for (i = 1; i <= nbJ; i++)
	{
        //copie image dans dim
		copy_array(nbL, nbC, image, dim);
		
		//change les cases de couleur
		change_colors(nbL, nbC, image, dim);
		
		if (i % nbS == 0 && nbS != 0)
		{
			//affiche une ligne de séparation
			for(j = 0; j < zoom * nbC; j++)
				printf("1 ");
				
			printf("\n");
			//affiche le tableau avec le zoom
			print_zoom(nbL, nbC, image, zoom); 
		}
		//copie image dans dim
		copy_array(nbL, nbC, dim, image);
	}
}


void number_of_lines(int number_of_plot_lines, int zoom, int nbL, int nbJ, 
									int nbS, int nbC, int dim[nbL][nbC])
{

	number_of_plot_lines = zoom*nbL;
	
	if (nbS > 0)
        {
			// ajoute une ligne de continuation
			number_of_plot_lines += 1; 
			// repète (nbJ/nbS) fois +l'input
            number_of_plot_lines *= (nbJ/nbS+1); 
            // le dernier affichage n'a pas de continuation
            number_of_plot_lines -= 1;
	    } 
	
	if (nbS != 0)
	{
		printf("P1\n%d %d\n", zoom*nbC, number_of_plot_lines);
		print_zoom(nbL, nbC, dim, zoom);
	}
	
}
