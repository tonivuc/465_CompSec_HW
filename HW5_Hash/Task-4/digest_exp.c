#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

 int main(int argc, char *argv[])
 {
     OpenSSL_add_all_digests();
     EVP_MD_CTX *mdctx;
     EVP_MD_CTX *mdctx2;

     const EVP_MD *md;
     const EVP_MD *md2;

     char mess1[30];
     char mess2[30];
     unsigned char md_value[EVP_MAX_MD_SIZE];
     unsigned char md_value2[EVP_MAX_MD_SIZE];
     unsigned int md_len, i;

     if (argv[1] == NULL) {
         printf("Usage: mdtest digestname\n");
         exit(1);
     }

     md = EVP_get_digestbyname(argv[1]);
     if (md == NULL) {
         printf("Unknown message digest %s\n", argv[1]);
         exit(1);
     }

     strcpy(mess1,"HELLO!HELLO!HELLO!HELLO!HELLO");
     strcpy(mess2,"HELLO!HELLO!HELLO!HELLO!HELLO");

     printf("%s\n", mess1);
     printf("%s\n", mess2);

     rand_string(mess1,30);

     printf("%s\n", mess1);

     mdctx = EVP_MD_CTX_create();
     mdctx2 = EVP_MD_CTX_create();

     EVP_DigestInit_ex(mdctx, md, NULL);
     EVP_DigestInit_ex(mdctx2, md, NULL);
     EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
     EVP_DigestUpdate(mdctx2, mess2, strlen(mess2));
     EVP_DigestFinal_ex(mdctx, md_value, &md_len);
     EVP_DigestFinal_ex(mdctx2, md_value2, &md_len);
     
     EVP_MD_CTX_destroy(mdctx);
     EVP_MD_CTX_destroy(mdctx2);

     printf("Digest 1 is: ");
     for (i = 0; i < md_len; i++)
         printf("%02x", md_value[i]);
     printf("\n");     

     printf("Digest 2 is: ");
     for (i = 0; i < md_len; i++)
         printf("%02x", md_value2[i]);
     printf("\n");   


     exit(0);
 }