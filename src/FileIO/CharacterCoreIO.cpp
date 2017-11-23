
#include "FileIO/CharacterCoreIO.h"

/*
 * Note: I mean, yes, you can use this function by yourself to hash your saved game
 * but be aware that I'll change these primes for different releases.
 * If you can reverse engineer them from the binary after it's built,
 * it's okay, you may cheat. 
 */

#define A 3423347 
#define B 153589 
#define FIRSTH 11

std::string CharacterCoreIO::hashFile(const std::string& input) const { 
	const char* cInput = input.c_str();
	unsigned h = FIRSTH;
	while (*cInput) {
		h = (h * A) ^ (cInput[0] * B);
		cInput++;
	}
	return std::to_string(h);
}
