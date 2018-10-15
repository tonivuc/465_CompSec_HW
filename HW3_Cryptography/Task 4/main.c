#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

//Each of these hex values correspond to one char
unsigned char correctCiphertext[] = {
	0x8d, 0x20, 0xe5, 0x05, 0x6a, 0x8d, 0x24, 0xd0,
	0x46, 0x2c, 0xe7, 0x4e, 0x49, 0x04, 0xc1, 0xb5,
	0x13, 0xe1, 0x0d, 0x1d, 0xf4, 0xa2, 0xef, 0x2a,
	0xd4, 0x54, 0x0f, 0xae, 0x1c, 0xa0, 0xaa, 0xf9
};

//Use to compile: gcc main.c -o main -lcrypto

//Source: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

//Don't really use this function any more
int stringToHex(unsigned char *yourString, unsigned int stringLen, unsigned char *buf) { //
  
  for (size_t i = 0; i < stringLen; i++) {
    sprintf(buf, "%s%x", buf, yourString[i]); //I get it! need %s to keep whatever was there before
  }
  
  //printf(buf+ '\n');
  return 0;
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
#define MAXCHAR 16
char* encAndCompare(unsigned char *returnedArray) {

	//Variables
    unsigned char iv[] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    unsigned char *plaintext = (unsigned char *)"This is a top secret."; 
    unsigned char ciphAsHex[128];  
    //unsigned char *key = (unsigned char *)"median          ";	//16 chars long

    unsigned char *correctAnsw = (unsigned char *)"median          ";	//16 chars long
    unsigned char *workStr;
	workStr = (char *)malloc(sizeof(char)*(MAXCHAR+1)); 

    FILE *fp;
    int i;
    char str[MAXCHAR];
    char* filename = "words.txt";
 
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }
    while (fgets(str, MAXCHAR, fp) != NULL) {

    	int retStringSize = strlen(str);

		unsigned char ciphertext[128]; //But we only use 64 bits
		unsigned char ciphAsHex[64]; //But we only use 64 bits 	
		
        str[retStringSize-1] = 0; //Remove the \n 
        int retStringSizeReal = strlen(str);
        strcpy(workStr,str);
        
    	for (i = retStringSizeReal; i < 16; i++) {
    		workStr[i] = ' '; //Apend to form length 16 char key
    	};
    	workStr[16] = 0; //Terminate string just to be sure

    	encrypt (plaintext, strlen ((char *)plaintext), workStr, iv, ciphertext);

    	if (strncmp(correctCiphertext, ciphertext,32) == 0) {
    		printf("We found the right key! It's %s\n", workStr);
    		returnedArray = workStr; //Return the key used for encryption
    		//break;
    		fclose(fp);
    		return workStr;
    	}
    }
    	
    fclose(fp);
    return 0;
}

int main (void)
{
  
  //Correct key storage
  unsigned char correctKey[16]; //128 bits = 16 chars.

  int decryptedtext_len, ciphertext_len;
  char * retVal = encAndCompare(correctKey);

  printf("Correct key: %s", retVal);

  return 0;
}

