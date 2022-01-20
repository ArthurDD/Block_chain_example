#include "blockchain.h"



void hachage(block *b, char* hash)		
{
	char index_c[10]; 	//10 est le nombre de chiffres max
	sprintf(index_c,"%d", b->index);
	
	char nonce_c[20];  //20 pris au hasard
	sprintf(nonce_c, "%d", b->nonce);
	
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
	
	
	
	strcpy(hash, s);
}




bool ValidNewBlock(block *b, block *previous)	//Valide le bloc avant qu'il rentre dans la chaine
{
	char hash[2*SHA256_DIGEST_LENGTH];
	hachage(b,hash);

	if(previous->index+1!=b->index)
	{
		return false;
	}
		
	if(strcmp(previous->hash,b->prev_hash)!=0)
	{
		return false;
	}

	if(strcmp(hash,b->hash)!=0)
	{
		return false;
	}

	return true;
}




bool ValidGenesisBlock(block *b)		//Valider le premier bloc
{
	char hash[2*SHA256_DIGEST_LENGTH];
	hachage(b,hash);

	if(b->index!=0)
	{
		return false;
	}
	if(strcmp(b->prev_hash,"")!= 0)
	{
		return false;
	}
	if(strcmp(hash,b->hash)!=0)
	{
		return false;
	}
	return true;
}




bool ValidChain(block_chain *b_c)	//Valide toute la chaine (après modif ou pour vérifier si toute la chaine est bonne)
{
	block *previous;
	
	char hash[2*SHA256_DIGEST_LENGTH];
	hachage(b_c->tete, hash);

	if(ValidGenesisBlock(b_c->tete)==false)
	{
		return false;
	}

	previous=b_c->tete;
	b_c->tete=b_c->tete->suiv;

	while(b_c->tete->suiv!=NULL)
	{
		if(ValidNewBlock(b_c->tete,previous)==false)
		{
			return false;
		}

		previous=b_c->tete;
		b_c->tete=b_c->tete->suiv;
	}

	return true;
}




void POW(block *b)	//A appeler lorsqu'on ajoute un bloc à la blockchain ou lorsque l'on modifie un bloc
{

	FILE* fichier = fopen("chaine.txt","r");
	int difficulty;
	fscanf(fichier,"%d",&difficulty);	//On récupère la difficulty dans le fichier chaine.txt
	fclose(fichier);

	time_t t_debut;
	time_t t_fin;
	t_debut = time(NULL);

	int i;
	char proof[difficulty];		//Proof contient les "difficulty" premiers char du hash
	char list0[difficulty];
	char char_to_chain[2];

	strcpy(list0,"");	//On reset la case mémoire de la liste

	for(i=0;i<difficulty;i++)	//Initialisation de la liste de 0 et de proof
	{
		
		sprintf(char_to_chain,"%c",b->hash[i]);
		strcat(proof,char_to_chain);	
		strcat(list0,"0");		//Liste de "difficulty 0"
	}


	while(strcmp(proof,list0)!=0)	//Tant que le hash ne commence pas par "difficulty" 0, on incrémente nonce et on re-calcule le hash du bloc
	{
		b->nonce+=1;		//Correspond au nombre de fois que le hash a été calculé (le nonce est inclus dans le calcul du hash)
		
		ajout_hachage(b);		//On utilise la fonction du fichier blockchain.c pour recalculer le hash avec la nouvelle valeur de nonce

		strcpy(proof,"");	//Reset de proof
		
		for(i=0;i<difficulty;i++)	//On réécrit proof 
		{
			sprintf(char_to_chain,"%c",b->hash[i]);
			strcat(proof,char_to_chain);	
		}
	
	}
	t_fin = time(NULL);
	
	printf("\nBloc n°%d validé ! ;-)\n",b->index);
	printf("Durée de la validation : %ld seconde(s).\n", t_fin-t_debut);	
}
			



	

