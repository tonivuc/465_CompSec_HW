#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

//Source: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

//Source: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int ciphertext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  ciphertext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

//Remember to append 0x20 to all words shorter than 16 bytes
//Returns number of bytes read
//The amount of chars in the dictionary is 206.661, far too many for a char array.
//Source: http://www.zentut.com/c-tutorial/c-read-text-file/
#define MAXCHAR 100
int encAndCompare(unsigned char *returnedArray) {

	//Variables
  unsigned char *iv = (unsigned char *)"NULNULNULNULNULNULNULNULNULNULNULNULNULNULNULNUL"; 	/* A 128 bit IV */
  unsigned char *plaintext = (unsigned char *)"This is a top secret.";   /* Message to be encrypted */
  unsigned char *goalCiphertext = (unsigned char *)"8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9";   //Ciphertext we are matching to (In hex format)
  unsigned char ciphertext[128]; //But we only use 64 bits


    FILE *fp;
    int i;
    char str[MAXCHAR];
    char workStr[MAXCHAR];
    char* filename = "words.txt";
 
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }
    while (fgets(str, MAXCHAR, fp) != NULL) {
        //printf("%s", str);
        strcpy(workStr,str);
        workStr[strcspn(workStr, "\n")] = 0;
    	for (i = (sizeof(workStr) / sizeof(workStr[0])); i < 16; i++) {
    		workStr[i] = 'Space'; //Apend to form length 16 char key
    	};
    	printf("Workstr: %s\n", workStr);
    	encrypt (plaintext, strlen ((char *)plaintext), workStr, iv, ciphertext);
    	if (strcmp(ciphertext,goalCiphertext) == 0) {
    		printf("We found the right key! It's %s\n", workStr);
    		returnedArray = workStr; //Return the key used for encryption
    		break;
    	}
    }
    	
    fclose(fp);
    return 0;
}

//Source: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
int main (void)
{
  /* A 128 bit encryption/decryption key 
  unsigned char *key = (unsigned char *)"8d20e5056a8d24d0"; //16 chars = 129 bit
  */
  
  //Correct key
  unsigned char correctKey[16]; //128 bits = 16 chars.

  /* Buffer for the decrypted text 
  unsigned char decryptedtext[128];
  */

  int decryptedtext_len, ciphertext_len;

  /* Encrypt the plaintext */
  //ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv, ciphertext);

  /* Do something useful with the ciphertext here */
  printf("Ciphertext is:\n");
  //BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

  encAndCompare(correctKey);


  /* Decrypt the ciphertext 
  decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
    decryptedtext);
    */

  /* Add a NULL terminator. We are expecting printable text 
  decryptedtext[decryptedtext_len] = '\0';
	*/
  /* Show the decrypted text 
  printf("Decrypted text is:\n");
  printf("%s\n", decryptedtext);
	*/

  return 0;
}