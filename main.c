#include "blockchain.h"

  
int main(void) 
{ 
	block_chain b_c;
	b_c.tete = NULL;

		
	creation_block_chain(&b_c);

	Menu(&b_c);

	return 0; 
}
