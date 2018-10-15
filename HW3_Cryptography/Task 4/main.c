#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

//Use to compile: gcc main.c -o main -lcrypto

//Source: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int numElementsNotSpace(unsigned char * inputStr, int stringLen) {
	int i;
	int numChars = 0;
	for (i = 0; i < stringLen; i++) {
		if (inputStr[i] != ' ' && inputStr[i] != 'Space' && inputStr[i] != 'NUL' && inputStr[i] != 0) {
			printf("I'm counting that %d\n",inputStr[i]);
			numChars++;
		}
	}
	return numChars;
}

int stringToHex(unsigned char *yourString, unsigned int stringLen, unsigned char *buf) { //
  
  for (size_t i = 0; i < stringLen; i++) {
    sprintf(buf, "%s%x", buf, yourString[i]);
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
int encAndCompare(unsigned char *returnedArray) {

	//Variables
  unsigned char *iv = (unsigned char *)"NULNULNULNULNULNULNULNULNULNULNULNULNULNULNULNUL"; 	/* 16 NULs, a 128 bit IV */ //OLD: NULNULNULNULNULNULNULNULNULNULNULNULNULNULNULNUL
  unsigned char *plaintext = (unsigned char *)"This is a top secret.";   /* Message to be encrypted */
  unsigned char *goalCiphertext = (unsigned char *)"8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9";   //Ciphertext we are matching to (In hex format) 64 hexadecimals. Giving

    //unsigned long goalCiphHex = 0x8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9;

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
    	printf("Size of key read from file, including newline: %d\n",retStringSize);

		unsigned char ciphertext[128]; //But we only use 64 bits
		unsigned char ciphAsHex[64]; //But we only use 64 bits 	
		unsigned char workStr[MAXCHAR];
        //str[strcspn(str, "\n")] = 0; //Make the last element 0?
        str[retStringSize-1] = 0; //Remove the \n 

        int retStringSizeReal = strlen(str);
        printf("Key size without newline%d\n: ",retStringSizeReal);

        //printf("str: %sdone\n",str);
        strcpy(workStr,str);
        //printf("workStr: %sdone\n",workStr);
        
    	for (i = retStringSizeReal; i < 16; i++) {
    		//printf("looping index %d\n",i);
    		workStr[i] = ' '; //Apend to form length 16 char key
    	};
    	workStr[16] = 0; //Terminate string just to be sure
    	//printf("char 15:%cOK\n", workStr[15]);
    	printf("The key: %s\n", workStr);
    	encrypt (plaintext, strlen ((char *)plaintext), workStr, iv, ciphertext);
    	printf("Ciph bfr.: %s\n",ciphertext);

    	//char buf[255] = {0};
  		//char yourString[255] = { "Hello" };
  		stringToHex(ciphertext,strlen(ciphertext),ciphAsHex);
  		printf("Ciph after.: %s\n",ciphAsHex);
  		//printf("String to hex %s\n",buf);
    	if (strcmp(ciphAsHex,goalCiphertext) == 0) {
    		printf("We found the right key! It's %s\n", workStr);
    		returnedArray = workStr; //Return the key used for encryption
    		break;
    	}
    }
    	
    fclose(fp);
    return 0;
}

void testFunction() {
  unsigned char *iv = (unsigned char *)"NULNULNULNULNULNULNULNULNULNULNULNULNULNULNULNUL"; 	/* 16 NULs, a 128 bit IV */ //OLD: NULNULNULNULNULNULNULNULNULNULNULNULNULNULNULNUL
  unsigned char *plaintext = (unsigned char *)"This is a top secret.";   /* Message to be encrypted */
  unsigned char *goalCiphertext = (unsigned char *)"8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9";   //Ciphertext we are matching to (In hex format) 64 hexadecimals. Giving
  unsigned char ciphertext[128]; //But we only use 64 bits
  unsigned char ciphAsHex[64]; //But we only use 64 bits    

  unsigned char *key = (unsigned char *)"zucchinis       ";

  /*
  char buf[255] = {0};
  char yourString[255] = { "X�p��3T$�" };
  stringToHex(yourString,5,buf);
  printf("buf: %s\n",buf);
  */

  //Input and output can be any size, but key must be 16 byte (128 bit)
  encrypt (plaintext, strlen ((char *)plaintext), key, iv, ciphertext);	

  printf("Ciphertext: %s\n",ciphertext);
}

void testFunc2() {
  unsigned char iv[] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}; 	/* 16 NULs, a 128 bit IV */ //OLD: NULNULNULNULNULNULNULNULNULNULNULNULNULNULNULNUL
  unsigned char *plaintext = (unsigned char *)"This is a top secret.";   /* Message to be encrypted */
  unsigned char *goalCiphertext = (unsigned char *)"8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9";   //Ciphertext we are matching to (In hex format) 64 hexadecimals. Giving
  unsigned char ciphertext[128];
  unsigned char ciphAsHex[128];  

  unsigned char *key = (unsigned char *)"median          ";	

  encrypt (plaintext, strlen ((char *)plaintext), key, iv, ciphertext);	

  stringToHex(ciphertext,strlen(ciphertext),ciphAsHex);
  printf("goalCiphertext: %s\n",goalCiphertext);
  printf("Ciphertext: %s\n",ciphAsHex);
}

//Source: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
int main (void)
{
  
  //Correct key
  unsigned char correctKey[16]; //128 bits = 16 chars.

  int decryptedtext_len, ciphertext_len;

  /* Do something useful with the ciphertext here */
  printf("Ciphertext is:\n");
  //testFunction();
  //encAndCompare(correctKey);
  testFunc2();





  return 0;
}

