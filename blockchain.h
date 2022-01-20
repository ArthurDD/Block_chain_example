#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>



typedef struct _block
{
        int index;
	int nonce;
        char auteur[20];
        char timestamp[30];	//Format "Le_dd/mm/yyyy_à_hhH_mmMIN." 
        char ticket[100];	//Aura la forme suivante :"objet_acheté____prix€"
        unsigned char hash[5*SHA256_DIGEST_LENGTH];
        unsigned char prev_hash[5*SHA256_DIGEST_LENGTH];
        struct _block* suiv;
} block;

typedef struct
{
        block* tete;
} block_chain;



/*Prototypes des fonctions du fichier blockchain.c*/
void ajout_bloc(block_chain *b_c, char* auteur, char* ticket);		//Ajout autorisé fait par les hôtes de caisse
void ajout_fin_chaine(block_chain *b_c, block *nb, bool depuis_txt);	//ajoute le block nb en fin de chaine et l'écrit également dans le fichier texte s'il n'y est pas déjà (si depuis_txt = false)
void ajout_hachage(block* b);				//Calcule et ajoute le hash au bloc b
void heure(char* date);		//Renvoie l'heure sous la forme "Le_dd/mm/yyyy_à_hhH_mmMIN."
void interface_ajout(block_chain *b_c);	//Interface pour rentrer les infos et appeler "ajout_block"
void creation_manuelle_ticket(block* b);


/*Prototype des fonctions du fichier interface.c*/
void afficher_bloc(block *b);		//Affiche le contenu d'un bloc
void afficher_block_chain(block_chain *b_c);	//Affiche toute la block chain
void ModifierChaine(block *nb);	//fonction qui prend en argument un block de la blockchain qu'on vient de modifier et qui recalcule les hashs de tous les blocs suivants.
void AfficherTicket(block_chain *b_c, char* prenom);	//fonction pour afficher un ticket en fonction de son index
void ModifierTicket(block *nb);		//fonction pour modifier un ticket une fois que l'on a vérifié qu'il en était bien l'auteur
void AfficherParAuteur(block_chain *b_c, char* prenom);		//Affiche tous les tickets de l'auteur "prenom"
void recherche_blocs(block_chain *b_c);			//Interface pour la recherche de blocs
void Menu(block_chain *b_c);			//Menu principal


/*Prototype des fonctions du fichier admin*/
void creation_block_chain(block_chain *b_c);	//Crée la blockchain à partir du fichier texte
void add_au_txt(block *b);			//Ajout au fichier texte d'un bloc ajouté via la fonction d'ajout de bloc
void modifier_txt(block_chain *b_c);		//Modifie une ligne du fichier texte lorsqu'on modifie un bloc.


/*Protoypes des fonctions du fichier proofofwork.c*/
void hachage(block *b, char* d);	//Calcule le hash du bloc b et le stocke dans d
bool ValidNewBlock(block *b, block *previous);	//Vérifie si le hash est bon et lance le POW du bloc
bool ValidGenesisBlock(block *b);	//Vérifie si le hash est bon et lance le POW du premier bloc
bool ValidChain(block_chain *b_c);	//Valide toute la chaine en appelant les fonctions  "ValidGenesisBlock" et "ValidNewBlock"
void POW(block *b);			//Fonction de Proof of work.

