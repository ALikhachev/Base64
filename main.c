#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "base64.h"

int main(int argc, char *argv[]) {
    char mode = (argc > 1) ? argv[1][0] : 'e';
    char *infile = (argc > 2) ? argv[2] : "in.txt";
    char *outfile = (argc > 3) ? argv[3] : "out.txt";

    setlocale(LC_ALL, "");
    printf("Input file: %s\nOutput file: %s\nMode: %s\n\n", infile, outfile, (mode == 'e') ? "Encoding" : "Decoding");

    if (mode == 'e')
        encode(infile, outfile);
    else
        decode(infile, outfile);

    return 0;
}
