#include <stdio.h>

int main (void)
{

  unsigned char correctCiphertext[] = {
    0x8d, 0x20, 0xe5, 0x05, 0x6a, 0x8d, 0x24, 0xd0,
    0x46, 0x2c, 0xe7, 0x4e, 0x49, 0x04, 0xc1, 0xb5,
    0x13
  };

  printf("Correct ciphertext: ");

  int i;
  for (i= 0; i < sizeof(correctCiphertext); i++) {
    printf("0x%02x",correctCiphertext[i]);
  }

  return 0;
}