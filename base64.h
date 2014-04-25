/*
  base64.h is a library to encode and decode file using base64 algorithm.
*/

#ifndef BASE64_H_INCLUDED
#define BASE64_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

/**
 * encode - accepts two parameters: input file and output file names.
 * Encodes bytes of input file using Base64 algorithm and outputting base64-code
 * to the output file.
 * If there is no file or it can't be accessed, error will be reported and program stopped.
 *
 * Example:
 *  encode("test.zip", "test.zip.b64");
 */
void encode(char *inPath, char *outPath);

/**
 * decode - accepts two parameters: input file and output file names.
 * Decodes Base64 code (string) to original file bytes and outputs them
 * to the output file.
 * If there is no file or it can't be accessed, error will be reported and program stopped.
 *
 * Example:
 *  decode("test.zip", "test.zip.b64");
 */
void decode(char *inPath, char *outPath);

#endif // BASE64_H_INCLUDED
