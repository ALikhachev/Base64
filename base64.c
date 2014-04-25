#include <stdio.h>
#include <stdlib.h>
#include "base64.h"

const char base64_table[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void encodeblock(unsigned char *in, unsigned char *out, int len) {
    out[0] = (unsigned char) base64_table[ (int)(in[0] >> 2) ];
    out[1] = (unsigned char) base64_table[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
    out[2] = (unsigned char) (len > 1 ? base64_table[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
    out[3] = (unsigned char) (len > 2 ? base64_table[ (int)(in[2] & 0x3f) ] : '=');
}

void encode(char *inPath, char *outPath) {
    FILE *infile;
    FILE *outfile;
    if ((infile = fopen(inPath, "rb")) == NULL) {
        printf("Can't open input file (%s).\n", inPath);
        exit(1);
    }

    if ((outfile = fopen(outPath, "w")) == NULL) {
        printf("Output file (%s) can't be accessed.\n", outPath);
        exit(1);
    }
    unsigned char in[3];
    unsigned char out[4];
    size_t i, len, blocksout = 0, linesize = 72;

    *in = (unsigned char) 0;
    *out = (unsigned char) 0;
    while (feof(infile) == 0) {
        len = 0;
        for (i = 0; i < 3; i++) {
            in[i] = (unsigned char) getc(infile);
            if (feof(infile) == 0) {
                len++;
            }
            else {
                in[i] = (unsigned char) 0;
            }
        }
        if (len > 0) {
            encodeblock(in, out, len);
            for (i = 0; i < 4; i++) {
                if (putc((int)(out[i]), outfile) == 0){
                    perror("Output failed.");
                    break;
                }
            }
            blocksout++;
        }
        if (blocksout >= (linesize/4) || feof(infile) != 0) {
            if (blocksout > 0) {
                fprintf(outfile, "\n");
            }
            blocksout = 0;
        }
    }
    printf("File %s encoded to Base64. Result file path: %s", inPath, outPath);
    fclose(infile);
    fclose(outfile);
}

void decodeblock( unsigned char in[4], unsigned char out[3] )
{
      out[0] = (in[0] << 2) | ((in[1] & 0x30) >> 4);
      out[1] = ((in[1] & 0xf) << 4) | ((in[2] & 0x3c) >> 2);
      out[2] = ((in[2] & 0x3) << 6) | in[3];
}

void decode(char *inPath, char *outPath)
{
    FILE *infile;
    FILE *outfile;
    if ((infile = fopen(inPath, "r")) == NULL) {
        printf("Can't open input file (%s).\n", inPath);
        exit(1);
    }

    if ((outfile = fopen(outPath, "wb")) == NULL) {
        printf("Output file (%s) can't be accessed.\n", outPath);
        exit(1);
    }

    unsigned char in[4], out[3], c;
    size_t i, len, pos, size = 0;
    char inc = 0;
    char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

    while (!feof(infile)) {
        for (pos = 0, len = 0, i = 0; i < 4 && !feof(infile); i++) {
            c = (unsigned char) getc(infile);
            if (strchr("\n\r", c)) { // Если нам встретился символ перевода каретки, пропускаем его
                c = (unsigned char) getc(infile);
            }
            if (!feof(infile) && !strchr(chars, c)) {
                inc = 1;
            }
            pos = strchr(base64_table, c) - base64_table; // Получаем индекс считываемоего символа в таблице (де)кодирования
            if (pos >= 0) {
                size++;
                if (c != '=') {
                    len++;
                    in[i] = (unsigned char) pos;
                }
            }
        }
        if (len > 0) {
            decodeblock(in, out);
            for (i = 0; i < len - 1; i++) {
                putc(out[i], outfile);
                if (ferror(outfile) != 0) {
                    perror("Output failed.");
                    break;
                }
            }
        }
    }

    if (--size % 4 > 0) {
        printf("Input data length not divisible by 4 so output might be incorrect.\n");
    }
    if (inc) {
        printf("Input data contains illegal symbols so output might be incorrect.\n");
    }
    printf("File %s decoded from Base64. Result file path: %s", inPath, outPath);
    fclose(infile);
    fclose(outfile);
}
