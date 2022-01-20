#include "blockchain.h"





void ajout_bloc(block_chain *b_c, char* auteur, char* ticket)
{
	block *nb = (block*)malloc(sizeof(block));
	
	nb->nonce = 0;
	
	char date[30];
	strcpy(date,"");	//On met le mot vide dedans (pour éviter de pointer sur qqchose qui existe déjà
	heure(date);
	
	strcpy(nb->auteur, auteur);
	strcpy(nb->timestamp, date);
	strcpy(nb->ticket, ticket);
	strcpy(nb->prev_hash,"");
	
	ajout_fin_chaine(b_c, nb, false);	//Ajoute à la fin de la chaine b_c le nouveau bloc nb tout en ajoutant le prev_hash.
				

}


void ajout_fin_chaine(block_chain *b_c, block *nb, bool depuis_txt)	//On positionne le bloc à ajouter à la fin de la chaine
{
	block *elt = b_c->tete;
		
	if (elt == NULL)	//Si la chaine est vide, on met le bloc à ajouter à la tête de la chaine
	{
		ajout_hachage(nb);
		if (ValidGenesisBlock(nb))	//Vérification de la validité du premier bloc de la chaine (appelé Genesis block)
		{
			POW(nb);
			b_c->tete = nb;
		}
		
		else
			printf("Erreur, ajout impossible : bloc non valide.");
	}
	
	else		//Sinon, on parcourt la chaîne jusqu'a ce que elt pointe vers le dernier elt non NULL
	{
		while (elt->suiv != NULL)
		{
			elt = elt->suiv;
		}
		
		strcpy(nb->prev_hash, elt->hash);		//On ajoute le hash du bloc précédent à notre bloc 
		
		nb->index = elt->index +1;
		ajout_hachage(nb);	//Calcul et ajout du hash du nouveau bloc
		
		if (ValidNewBlock(nb,elt))	//Vérification de la validité du bloc
		{
			POW(nb);
			elt->suiv = nb;
			nb->suiv = NULL;
			
			if (depuis_txt == false)
			{
				add_au_txt(nb);
			}
		}
		
		else
		{
			printf("Erreur, ajout impossible : bloc non valide.\n");
		}
	}
}



void ajout_hachage(block* b)
{
	char index_c[10]; 	//10 est le nombre de chiffres max
	sprintf(index_c,"%d", b->index);
	
	char nonce_c[20];  //20 pris au hasard
	sprintf(nonce_c, "%d", b->nonce /*b->nonce*/);
	
	char s[290];
	strcpy(s,"");	//On la reset (pour éviter de pointer sur des choses déjà là avant)

	
	/*Concaténation de toutes les données dans s à prendre en compte pour le calcul du hash*/
	strcat(s,index_c);
	strcat(s,nonce_c);
	strcat(s,b->auteur);
	strcat(s,b->timestamp);
	strcat(s,b->ticket);
	
	strcat(s,b->prev_hash);	//A la fin, s contient toutes les infos du bloc concaténées en une chaîne de caractères.	
	
	unsigned char *d = SHA256(s, strlen(s), 0);
	
	/*On convertit ensuite d qui est en hexa en une chaine de caractères*/		
	strcpy(s,"");	//On va se re-servir de s		
	char hex[3];	
	strcpy(hex,"");	//On l'initialise aussi
	
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		strcpy(hex,"");
		sprintf(hex, "%02x", d[i]);	
		strcat(s,hex);
	}
		
	strcpy(b->hash, s);
	
	
		
}

void heure(char* date)
{
	time_t timestamp; 
	struct tm * t; 
  
	timestamp = time(NULL); 
	t = localtime(&timestamp); 

	char part1[15];
	char part2[15];
	
	
	/* Affiche la date et l'heure courante (format francais) */ 
	sprintf(part1, "Le_%02u/%02u/%04u_", t->tm_mday, t->tm_mon, 1900 + t->tm_year); 
	sprintf(part2, "à_%02uh_%02umin.", t->tm_hour, t->tm_min);
	strcat(date,part1);
	strcat(date,part2);
	
}




void interface_ajout(block_chain *b_c)	
{
	printf("\n\n\n\n\n\n\n\n\n-----------------------------------------------------------\n-\n-\n-\t\t AJOUT D'UN BLOC\n-\n-\n-----------------------------------------------------------\n\n\n\n\n\n");
	
	char nom[10];
	printf("Hôte(-sse) de caisse :\t");

	scanf("%s",nom);

	putchar('\n');
	
	char objet[80];
	char prix[10];
	char ticket[100];
	printf("\nObjet acheté :\t");
	scanf("%s",objet);
	printf("\nPrix de l'objet :\t");
	scanf("%s",prix);
	
	strcpy(ticket,objet);
	strcat(ticket,"____");
	strcat(ticket,prix);
	strcat(ticket,"€");
	
	printf("\n\nAjout en cours, merci de patienter...\n");
	ajout_bloc(b_c, nom, ticket);
}



void creation_manuelle_ticket(block* b)	//Fonction dont le but est de créer un ticket manuellement (correspondra aux faux tickets)
{
	char timestamp[30];
	char date[15];
	char heure[5];
	char min[5];

	char ticket[100];
	char objet[80], prix[10];
	char hash[SHA256_DIGEST_LENGTH];

	
	printf("\n\n\n\n\n\n\n\n\n-----------------------------------------------------------\n-\n-\n-\t CREATION MANUELLE D'UN TICKET\n-\n-\n-----------------------------------------------------------\n\n\n\n\n\n");
	
	printf("n° du ticket :\t");
	scanf("%d",&b->index);
	
	printf("\nValeur du nonce : \t");
	scanf("%d",&b->nonce);
	
	printf("\nHôte(-esse) de caisse :\t");
	scanf("%s",b->auteur);
	
	printf("\nDate au format 'JJ/MM/AAAA':\t");
	scanf("%s",date);
	printf("\nHeure au format 'hh':\t");
	scanf("%s",heure);
	printf("\nMinutes au format 'minmin';\t");
	scanf("%s",min);
	strcpy(timestamp,"Le ");
	strcat(timestamp,date);
	strcat(timestamp,", à ");
	strcat(timestamp,heure);
	strcat(timestamp,"h ");
	strcat(timestamp,min);
	strcat(timestamp,"min.");
	strcpy(b->timestamp,timestamp);
	
	printf("Objet acheté :\t");
	scanf("%s", objet);
	printf("Prix :\t");
	scanf("%s",prix);
	
	strcpy(ticket,objet);
	strcat(ticket,"____");
	strcat(ticket,prix);
	strcat(ticket,"€");
	strcpy(b->ticket,ticket);
	
	printf("Hash du ticket précédent :\t");
	scanf("%s",b->prev_hash);
	
	ajout_hachage(b);
}





		
