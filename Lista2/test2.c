#include <mcrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() 
{
	MCRYPT td;
	int i;
	char *key; /* created using mcrypt_gen_key */
	char *block_buffer;
	char *IV;
	int blocksize;
	int keysize = 32; /* 192 bits == 24 bytes */

	key = calloc(1, keysize);
	strcpy(key, "kluczkluczkluczkluczkluczkluczkl");

	td = mcrypt_module_open("rijndael-256", NULL, "cbc", NULL);

	blocksize = mcrypt_enc_get_block_size(td);
	block_buffer = malloc(blocksize);
	/* but unfortunately this does not fill all the key so the rest bytes are
	* padded with zeros. Try to use large keys or convert them with mcrypt_gen_key().
	*/
	
	IV=malloc(mcrypt_enc_get_iv_size(td));

	/* Put random data in IV. Note these are not real random data,
	* consider using /dev/random or /dev/urandom.
	*/

	/* srand(time(0)); */
	 for (i=0; i < mcrypt_enc_get_iv_size(td); i++)
		IV[i]=rand();

	mcrypt_generic_init( td, key, keysize, IV);

	 /* Encryption in CBC is performed in blocks */
	 while ( fread (block_buffer, 1, blocksize, stdin) == blocksize ) 
	 {
		// mcrypt_generic (td, block_buffer, blocksize);
		mdecrypt_generic (td, block_buffer, blocksize);
             	fwrite ( block_buffer, 1, blocksize, stdout);
	}

	/* deinitialize the encryption thread */
	mcrypt_generic_deinit (td);

	/* Unload the loaded module */
	mcrypt_module_close(td);
	return 0;
}
