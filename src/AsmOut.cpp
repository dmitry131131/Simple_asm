/**
 * @file
 * @brief Functions output assembled commans sources
*/
#include <stdio.h>

#include "AsmErrors.h"
#include "AsmOut.h"

FILE* create_output_file(const char* filename, fileCreateMode modeCode)
{
    char mode[10] = {};
    mode[0] = 'w';
    switch (modeCode)
    {
    case BIN:
        mode[1] = 'b';
        break;

    case TEXT:
        break;
    
    default:
        break;
    }

    FILE* file = fopen(filename, mode);

    if (!file)
    {
        // creating file error
    }

    return file;
}

asmErrorCode write_header_info(FILE* outputTextFile, FILE* outputBinFile, int version, size_t commandCount)
{
    if (!outputTextFile)
    {
        // nullptr error
    }
    if (!outputBinFile)
    {
        // nullptr error
    }
    
    fprintf(outputTextFile, "AD\n");
    fprintf(outputTextFile, "Version %d\n", version);
    fprintf(outputTextFile, "%lu commands\n", commandCount);

    if (write_char_to_bin_file(outputBinFile, 'A'))
    {
        // write error
    }
    write_char_to_bin_file(outputBinFile, 'D');
    write_int_to_bin_file(outputBinFile, version);
    write_int_to_bin_file(outputBinFile, (int) commandCount);

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode write_char_to_bin_file(FILE* file, char num)
{
    if (!file)
    {
        // file nullptr error
    }

    if (fwrite(&num, sizeof(char), 1, file) != 1)
    {
        // write error
    }

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode write_double_to_bin_file(FILE* file, double num)
{
    if (!file)
    {
        // file nullptr error
    }

    if (fwrite(&num, sizeof(double), 1, file) != 1)
    {
        // write error
    }

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode write_int_to_bin_file(FILE* file, int num)
{
    if (!file)
    {
        // file nullptr error
    }

    if (fwrite(&num, sizeof(int), 1, file) != 1)
    {
        // write error
    }

    return NO_ASSEMBLER_ERRORS;
}