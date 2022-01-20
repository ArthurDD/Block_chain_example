#include "blockchain.h"




void afficher_bloc(block *b)
{
	printf("\tTicket n°%d\n\n",b->index);
	printf("Hôte(-sse) de caisse : %s\n",b->auteur);
	printf("Date : %s\n", b->timestamp);
	printf("Achat :\t %s\n", b->ticket);
	printf("Hash : \t\t\t %s\n", b->hash);
	printf("Hash du bloc précédent : %s\n\n\n\n",b->prev_hash);
}




void afficher_block_chain(block_chain *b_c)
{
	block *b = b_c->tete;
	
	printf("\n\n\n\n\n\n\n\n\n-----------------------------------------------------------\n-\n-\n-\t\t CONSULTATION DE LA BLOCKCHAIN\n-\n-\n-----------------------------------------------------------\n\n\n\n\n\n");
	
	if (b == NULL)
	{
		printf("Erreur, la blockchain est vide !\n");
	}
	
	else
	{
		while (b != NULL)
		{	
			afficher_bloc(b);
			b = b->suiv;
		}	
	}
}	




	
void ModifierChaine(block *nb)  //fonction pour modifier les hash des blocs suivants 
{
	if (nb->suiv == NULL)	//Si c'est le dernier bloc de la chaine, on modifie seulement son hash
	{
		ajout_hachage(nb);
	}
	
	else	//Sinon, on modifie le hash et le prev_hash de tous les blocs d'après
	{
		char prev_hash[5*SHA256_DIGEST_LENGTH];
		strcpy(prev_hash, nb->hash);
		
		block *b = nb->suiv;
		
		while(b->suiv != NULL)
		{
			strcpy(nb->prev_hash,prev_hash);	//On modifie d'abord le prev hash puis ensuite on recalcule le hash
			ajout_hachage(b);
			
			strcpy(prev_hash,b->hash);
			b=b->suiv;
			
		}
	}
}






void AfficherTicket(block_chain *b_c, char* prenom)		//fonction pour afficher un ticket en fonction de son index
{
	block *b=b_c->tete;
	block *nb = NULL;	//Contiendra le potentiel bloc trouvé
	
	printf("Entrez l'index du ticket recherché :\t");
	int ind;
	scanf("%d",&ind);
	
	while(b!=NULL)
	{
		if (ind == b->index)	//SI c'est le bon, on sauvegarde l'adresse
		{
			nb = b;	  
		}
		
		b= b->suiv;
	}
			
	if(nb != NULL)		//Si on l'a trouvé, on l'affiche et le modifie (potentiellement)
	{
		afficher_bloc(nb);	//Si on est sur le bloc avec le bon index, on l'affiche
		
		block *save;  
		save=nb;		//on sauvegarde l'adresse pointée par nb
		printf("\n");
		
		
		
		printf("Voulez-vous modifier ce ticket ? (y) or (n)\t");
		char choix;
		scanf(" %c", &choix);
		
		if ( (choix == 'y') && strcmp(prenom,nb->auteur)==0)	//S'il veut modifier le ticket et qu'il en est l'auteur
		{
			printf("\n"); 
		
			ModifierTicket(nb);	//on propose de modifier le ticket et on modifie alors tous les blocs suivants
			modifier_txt(b_c);	//On modifie également dans le fichier txt			
			
		}
		
		else if ((choix == 'y') && strcmp(prenom, nb->auteur) != 0)
		{
			printf("Action impossible, vous n'en êtes pas l'auteur.\n");
			
		}
		
		else
		{

		}
	}
	
	else	//Si le bloc n'a pas été trouvé, on le signale
	{
		printf("Erreur, il n'y a pas de ticket ayant cet index.\n");	
	}
	
}



void AfficherParAuteur(block_chain *b_c, char* prenom)  //fonction pour afficher tous les tickets faits par celui qui veut les voir
{
	block *nb=b_c->tete;
	while(nb!=NULL)
	{
		if(strcmp(prenom,nb->auteur)==0)	
		{
			afficher_bloc(nb);		//on affiche le ticket
			printf("\nVoulez-vous modifier ce ticket?\n"); //on propose de modifier le ticket
			printf("1- Modifier ce ticket\n");
			printf("2- Passer au ticket suivant\n\n\nVotre choix :\t");
			int choix;
			scanf("%d",&choix);
			printf("\n");
			
			if(choix==1)
			{
				ModifierTicket(nb); //On modifie le ticket
				modifier_txt(b_c);	//On modifie également dans le fichier txt
				printf("\n\n\n\n\n\nTicket modifié ! Ticket suivant :\n\n\n\n\n\n");
			}
			
			
			
		}
		nb=nb->suiv; //on passe au ticket suivant
	}
	printf("Tous les tickets de cet auteur ont été affichés.\n");
}





void ModifierTicket(block *nb)    //fonction pour modifier un ticket une fois que l'on a vérifié qu'il en était bien l'auteur
{
	printf("\n\n\nQue voulez-vous faire sur le ticket?\n\n");
	printf("Soit modifier :\n");
	printf("1 : Auteur\n");
	printf("2 : Date\n");
	printf("3 : Achat\n\nSoit\n");
	printf("4 : Quitter\n\n\nChoix : ");
	int choix;
	scanf("%d",&choix);
	printf("\n");
	
	char continuer;
	
	if(choix<1 || choix>4)
	{
		printf("Erreur, entrez un numéro valide.\n");
		ModifierTicket(nb);
	}
	else
	{
		if(choix==1)
		{
			printf("Entrez le nom du caissier :\t");
			char prenom[20];
			scanf("%s",prenom);
			strcpy(nb->auteur,prenom);
			
			printf("\n\n\nModification effectuée !\n\n");
			printf("Continuer la modification ? (y) or (n)\t");
			scanf(" %c", &continuer);
			
			if (continuer == 'y')
			{
				ModifierChaine(nb);	//On recalcule les hashs des blocs suivants
				ModifierTicket(nb);	
			}
			
			else
			{
				ModifierChaine(nb);  //on recalcule les hashs des blocs suivants
				
			}

		}
		else if(choix==2)
		{
			char date[15];
			char heure[5];
			char min[5];
			char timestamp[30];
			
			printf("Entrez la date au format 'JJ/MM/AAAA':\t");	//Toutes ces lignes sont pour mettre au bon format le timestamp
			scanf("%s",date);
			printf("\nHeure au format 'hh':\t");
			scanf("%s",heure);
			printf("\nMinutes au format 'minmin';\t");
			scanf("%s",min);
			strcpy(timestamp,"Le_");
			strcat(timestamp,date);
			strcat(timestamp,",_à_");
			strcat(timestamp,heure);
			strcat(timestamp,"h_");
			strcat(timestamp,min);
			strcat(timestamp,"min.");
			strcpy(nb->timestamp, timestamp);			//Jusqu'ici
			
			
			printf("\n\n\nModification effectuée !\n\n");
			printf("Continuer la modification ? (y) or (n)\t");
			scanf(" %c", &continuer);
			
			if (continuer == 'y')
			{	
				ModifierChaine(nb);
				ModifierTicket(nb);
			}
			
			else
			{
				ModifierChaine(nb);  //on modifie les hash des blocs suivants
			}
		}
		else if(choix==3)
		{
			printf("Entrez le nom de l'article :\t");	//Toutes ces lignes sont pour mettre au bon format le ticket
			char objet[80];
			scanf("%s",objet);
			
			printf("\nEntrez le prix :\t");
			char prix[10];
			scanf("%s",prix);
			
			char ticket[100];
			
			strcpy(ticket,objet);
			strcat(ticket,"____");
			strcat(ticket,prix);
			strcat(ticket,"€");				//Jusqu'ici
			
			strcpy(nb->ticket,ticket);
			
						
			printf("\n\n\nModification effectuée !\n\n");
			printf("Continuer la modification ? (y) or (n)\t");
			scanf(" %c", &continuer);
			
			if (continuer == 'y')
			{	
				ModifierChaine(nb);
				ModifierTicket(nb);
			}
			else
			{
				ModifierChaine(nb);  //on modifie les hash des blocs suivants (+ le hash du bloc nb)
			}
		}
		
		else if(choix==4)
		{
			
		}
	}
}





void recherche_blocs(block_chain *b_c)   //menu pour chercher un ou plusieurs tickets
{
	printf("\n\n\n\n\n\n\n\n\n-----------------------------------------------------------\n-\n-\n-\t\t RECHERCHE DE TICKETS\n-\n-\n-----------------------------------------------------------\n\n\n\n\n\n");

	char prenom[20];
	printf("Tout d'abord, veuillez entrer votre prénom:\t");   //on garde en mémoire son nom pour savoir s'il a le droit de modifiier
	scanf("%s",prenom);

	bool levier = false;	//Le levier va faire en sorte que l'on sortira de la boucle seulement quand on aura donné une réponse valide.
	while (levier == false)
	{		
		printf("\nQue souhaitez vous faire %s ?\n\n",prenom);
		printf("1- Rechercher un ticket via son index.\n");	
		printf("2- Afficher tous les tickets que vous avez édités.\n");
		printf("3- Quitter\n");
		
		int choix;
		printf("\n\nVotre choix : ");
		scanf("%d",&choix);
		printf("\n\n\n\n\n\n");
		
		if(choix<1 || choix>3)
		{
			printf("Erreur, entrez un numéro valide.\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		}
		
		else
		{
			if(choix==1)
			{
				AfficherTicket(b_c,prenom);
				
				printf("\n\n\nSouhaitez-vous faire autre chose ? (y) or (n)\t");
				char rep;
				scanf(" %c",&rep);
				printf("\n");
				
				if (rep =='n')
				{
					levier = true;
				}
				
			}
			else if(choix==2)
			{
				AfficherParAuteur(b_c,prenom);
				
				printf("\n\n\nSouhaitez-vous faire autre chose ? (y) or (n)\t");
				char rep;
				scanf(" %c",&rep);
				printf("\n");
				
				if (rep =='n')
				{
					levier = true;
				}
			}
			else
			{
				levier = true;
			}
		}
	}
	

}					
				



void Menu(block_chain *b_c)
{

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n-------------------------------------------------------------------------------\n-\n-\n-\t\t PROJET C - DRIANT Arthur et GERARD Hugues\n-\n-\n-------------------------------------------------------------------------------\n\n\n\n\n\n");
	printf("Bienvenue dans ce service de gestion des tickets de caisse\n");
	bool levier = false;	
	while (levier == false)
	{		
		printf("\nQue souhaitez vous faire ?\n\n");
		printf("1- Ajouter un ticket\n");
		printf("2- Afficher la blockchain\n");	
		printf("3- Rechercher un ticket\n");
		printf("4- Quitter\n");
		
		int choix;
		printf("\n\nVotre choix : ");
		scanf("%d",&choix);
		printf("\n\n\n\n\n\n");
		
		if(choix<1 || choix>4)
		{
			printf("Erreur, entrez un numéro valide.\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		}
		else
		{
			if(choix==1)
			{
				interface_ajout(b_c);	

				printf("\n\n\nQuitter l'application ? (y) or (n)\t");
				char rep;
				scanf(" %c",&rep);
				printf("\n");
				
				if (rep =='y')
				{
					levier = true;
				}
				
			}
			
			else if (choix==2)
			{
				afficher_block_chain(b_c);
				
				printf("\n\n\nVoulez-vous faire autre chose ? (y) or (n)\t");
				char rep;
				scanf(" %c",&rep);
				printf("\n");
				
				if (rep =='n')
				{
					levier = true;
				}
			}
			
			else if(choix==3)
			{
				recherche_blocs(b_c);
				
				printf("\n\n\nQuitter l'application ? (y) or (n)\t");
				char rep;
				scanf(" %c",&rep);
				printf("\n");
				
				if (rep =='y')
				{
					levier = true;
				}
				
			}
			
			else
			{
				levier=true;
			}
		}

	}
	printf("Merci d'avoir utilisé ce service ! :-)\n");
}
