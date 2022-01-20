#include "blockchain.h"




void creation_block_chain(block_chain *b_c)		//Crée une block chain à partir d'un fichier texte
{
	FILE *fichier = fopen("chaine.txt","r");
	
	if (fichier!=NULL)
	{ 
		char nom[20];
		char date[30];
		char article[100];
		int difficulty;
	
		fscanf(fichier,"%d",&difficulty);	//Récupération de la difficulty
	
		printf("Création de la blockchain à partir du fichier texte. Cela peut prendre un certain temps.\nDifficulté du Proof of Work : %d\n\n", difficulty);

		while (fscanf(fichier, "%s\n%s\n%s", nom,date,article) == 3)
		{
			block *nb = malloc(sizeof(block));
			
			strcpy(nb->auteur,nom);
			strcpy(nb->timestamp,date);
			strcpy(nb->ticket,article);
			strcpy(nb->prev_hash,"");
			
			ajout_fin_chaine(b_c, nb, true);
		}
	
		fclose(fichier);
	}
	else
	{
		printf("Erreur, impossible d'ouvrir le fichier.\n");
		exit(0);
	}
}



void add_au_txt(block *b)
{
	FILE *fichier = fopen("chaine.txt","a");
	
	if (fichier != NULL)
	{
		fprintf(fichier, "%s\n%s\n%s\n\n",b->auteur,b->timestamp,b->ticket);
		fclose(fichier);
	}
	else
	{
		printf("Erreur, fichier introuvable.\n");
		exit(0);
	}
}



void modifier_txt(block_chain *b_c)	//Ré-écriture de la blockchain dans le fichier txt une fois qu'elle a été modifiée
{
	
	FILE *fichier= fopen("chaine.txt","r");	//On récupère tout d'abord la difficulté
	
	if (fichier != NULL)
	{
		int difficulty;
		fscanf(fichier,"%d",&difficulty);
		fclose(fichier);			
		
		fichier = fopen("chaine.txt", "w+");	//Puis on ré-ouvre le fichier en l'effaçant pour tout réécrire, en commençant par la difficulté.
		fprintf(fichier,"%d\n",difficulty);
		
		block *b = b_c->tete;
		
		while (b != NULL)
		{
			fprintf(fichier, "%s\n%s\n%s\n\n",b->auteur,b->timestamp,b->ticket);
			b = b->suiv;
		}
		fclose(fichier);
	}
	
	else
	{
		printf("Erreur lors de l'ouverture du fichier, modification impossible.\n");
		exit(0);
	}
}



	
