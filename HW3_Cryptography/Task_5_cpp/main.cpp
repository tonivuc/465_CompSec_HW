#include <iostream>
#include <cmath>

//32 bit = 4 byte

//Quite a bit of credit to this website, for showing me how to do this without bitshifting:
//http://www.techiedelight.com/des-implementation-c/

/*
 * Simplifications:
 * No need for the key-to-subkey function. We use the subkey 0x123456789abc for our one round
 */

//Tables
const int expansionMapping[] =
		{
				32,  1,  2,  3,  4,  5,
				4,  5,  6,  7,  8,  9,
				8,  9, 10, 11, 12, 13,
				12, 13, 14, 15, 16, 17,
				16, 17, 18, 19, 20, 21,
				20, 21, 22, 23, 24, 25,
				24, 25, 26, 27, 28, 29,
				28, 29, 30, 31, 32,  1
		};

//Constants
const unsigned int HALFINPUTSIZE = 4;
const unsigned int SUBKEYSIZE = 6;

//Variables
int expandedText[48];
int RIGHT[32];
int rightSideAsInt[32];

//Borrowing these functions, now idea how to do this:
//Input to this is a char represented as an integer
//intAsBinary is the return value
void intToBinary(int n, int intAsBinary[8])
{
	int k, m;
	int counter = 0;
	for (int i = 7; i >= 0; i--)
	{
		m = 1 << i;
		k = n & m;
		if (k == 0)
			intAsBinary[counter] = 0;
		else
			intAsBinary[counter] = 1;
		counter++;
	}
}




//Some static stuff


int expansion();

//Turn our char array into an array of integers.
void charArrayToIntArray(const char input[], int arrayLength, int output[]) {
	for (int i = 0; i < arrayLength; i++) {
		output[i] = int(input[i]);
	}
}

void expansionF(const int input[32], int output[48]) {

	for (int i = 0; i < 48; i++) {
		output[i] = input[expansionMapping[i]];
	}
}

void expansionFunction(int pos, int text) {
	for (int i = 0; i < 48; i++) { //Go through the length of the new 48 bit string thing
		if (expansionMapping[i] == pos + 1) { //If the
			expandedText[i] = text;
		}
	}
}

/*
void convertToBits(int ch[])
{
	int value = 0;
	for (int i = 7; i >= 0; i--) {
		value += (int)pow(2, i) * ch[7 - i];
	}

	fprintf(out, "%c", value);
}
 */


//Takes in two 32-byte arrays
//3rd argument will be modified
int feistelRound(unsigned const char leftInput[], unsigned const char rightInput[], unsigned const char key[], unsigned char rightSideOutput[]) {


	//DES function


	//Do this after running the DES function
	for (int i = 0; i < HALFINPUTSIZE; i++) {
		rightSideOutput[i] = rightSideOutput[i] ^ key[i];
	}

	return 0; //Success
}

//This function takes a 4 byte (32 bit) char array as input, as well as a 6-byte key.
//Then does Expansion, XOR with the key, S-box and finally permutation, before returning a new 32 byte block
int DESScrambler(unsigned const char prevRightSide[], unsigned const char key[], unsigned char output[]) {

	//expansion()
	return -1;
}


//Function to
//4 byte input, 6 byte output
int expansion(unsigned const char input[], unsigned char output[]) {

	for (int i = 0; i < 32; i++) {
		expansionFunction(i, input[i]);
	}

	/*
	for(int i = 1; i < SUBKEYSIZE-1; i++) {
		for (int j = i; j < i+4; j++) {
			output[j] = input[j]; //Add the 4 original bytes
		}
	}
	 */
	return -1;
}

int DESRound() {

	//Initial permutation

	//Fesistel round

	//No need to swap halves I think?

	//Final permutation
	return -1;
}


int main() {
	std::cout << "Hello, World!" << std::endl;
	int binary[8];
	intToBinary(3,binary);
	for (int i = 0; i < 8; i++) {
		printf("%d",binary[i]);
	}
	printf("\n");
	//printf("binary: %d",binary);
	return 0;
}