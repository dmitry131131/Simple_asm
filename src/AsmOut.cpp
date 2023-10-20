/**
 * @file
 * @brief Functions output assembled commans sources
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "AsmErrors.h"
#include "AsmOut.h"

asmErrorCode buffer_ctor(outputBuffer* buffer, size_t size)
{
    assert(buffer);

    buffer->Buffer  = NULL;

    buffer->bufferPointer = 0;

    if (create_command_buffer(&(buffer->Buffer), size))
    {
        return ALLOC_MEMORY_ERROR;
    }

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode buffer_dtor(outputBuffer* buffer)
{
    assert(buffer);

    free(buffer->Buffer);

    buffer->Buffer  = NULL;

    buffer->bufferPointer = 0;

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode create_output_file(FILE** file, const char* filename, fileCreateMode modeCode)
{
    assert(filename);

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
        return WRONG_FILE_CREATE_MODE;
    }

    *file = fopen(filename, mode);

    if (!file)
    {
        return FILE_CREATING_ERROR;
    }

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode create_command_buffer(char** buffer, size_t size)
{
    assert(buffer);

    *buffer = (char*) calloc(size + 1, sizeof(char));

    if (!buffer)
    {
        return ALLOC_MEMORY_ERROR;
    }

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode write_header_info(FILE* outputTextFile, FILE* outputBinFile, int version, size_t commandCount)
{
    assert(outputBinFile);

    assert(outputTextFile);
    
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

asmErrorCode write_char_to_buffer(outputBuffer* buffer, char num)
{
    assert(buffer);

    (buffer->Buffer)[buffer->bufferPointer] = num;
    (buffer->bufferPointer)++;

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode write_int_to_buffer(outputBuffer* buffer, int num)
{
    assert(buffer);

    char* intPtr = (char*) &num;

    for (size_t i = 0; i < sizeof(int); i++)
    {
        (buffer->Buffer)[buffer->bufferPointer + i] = intPtr[i];
    }

    buffer->bufferPointer += sizeof(int);

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode write_buffer_to_file(FILE* file, outputBuffer* buffer)
{
    assert(file);

    if (fwrite(buffer->Buffer, sizeof(char), buffer->bufferPointer, file) != buffer->bufferPointer)
    {
        return FWRITE_ERROR;
    }

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode write_double_to_buffer(outputBuffer* buffer, double num)
{
    assert(buffer);

    char* doublePtr = (char*) &num;

    for (size_t i = 0; i < sizeof(double); i++)
    {
        (buffer->Buffer)[buffer->bufferPointer + i] = doublePtr[i];
    }

    buffer->bufferPointer += sizeof(double);

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode write_double_to_bin_file(FILE* file, double num)
{
    assert(file);

    if (fwrite(&num, sizeof(double), 1, file) != 1)
    {
        return FWRITE_ERROR;
    }

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode write_int_to_bin_file(FILE* file, int num)
{
    assert(file);

    if (fwrite(&num, sizeof(int), 1, file) != 1)
    {
        return FWRITE_ERROR;
    }

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode write_char_to_bin_file(FILE* file, char num)
{
    assert(file);

    if (fwrite(&num, sizeof(char), 1, file) != 1)
    {
        return FWRITE_ERROR;
    }

    return NO_ASSEMBLER_ERRORS;
}