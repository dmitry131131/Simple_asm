/**
 * @file
 * @brief Main asembler functions sources
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "OneginError.h"
#include "InputOutput.h"
#include "AsmErrors.h"
#include "AsmOut.h"
#include "AssemblerFunc.h"

#define DEF_CMD(name, num, args, ...)           \
    else if (!strcmp(command, name))            \
    {                                           \
        __VA_ARGS__                             \
    }                                           

enum asmErrorCode main_assembler_function(textData* text)
{
    assert(text);

    outputBuffer binBuffer  = {};
    asmErrorCode error      = NO_ASSEMBLER_ERRORS;

    if ((error = buffer_ctor(&binBuffer, text->bufferSize * 4)))
    {
        assert(!error);
        return error;
    }

    FILE* outputBinFile = NULL;
    if ((error = create_output_file(&outputBinFile, "outbin", BIN)))
    {
        assert(!error);
        return error;
    }

    FILE* outputTextFile = NULL;
    if ((error = create_output_file(&outputTextFile, "out.txt", TEXT)))
    {
        assert(!error);
        return error;
    }

    TagBuffer tagBuffer = {};
    if ((error = tag_buffer_ctor(&tagBuffer)))
    {
        return error;
    }


    #define WRITE_SINGLE_COMMAND(num) do{                   \
                                                            \
    fprintf(outputTextFile, #num "\n");                     \
    write_char_to_buffer(&binBuffer, num);                  \
                                                            \
    }while(0)


    write_header_info(outputTextFile, outputBinFile, 1, text->linesCount);

    for (size_t i = 0; i < text->linesCount; i++)
    {
        char command[MAX_COMMAND_LEN]       = {};
        double commandArg                   = 0;
        char registerName[MAX_REGISTER_LEN] = {};

        if(sscanf(text->linesPtr[i], "%s", command) != 1)
        {
            error = INVALID_SYNTAX;
            print_assembler_error(error, text->linesPtr[i], i+1);
            break;
        }

        if (command[0] == ':')
        {
            add_tag_to_buffer((command + 1), binBuffer.bufferPointer, &tagBuffer);
        }

        #include "Functions.def"

        else
        {
            error = INVALID_SYNTAX;
            print_assembler_error(error, text->linesPtr[i], i+1);
            break;
        }
    }

    if (error)
    {
        DESTRUCT_ALL_BUFFERS_AND_RETURN;

        return error;
    }

    if ((error = write_buffer_to_file(outputBinFile, &binBuffer)))
    {
        return error;
    }

    DESTRUCT_ALL_BUFFERS_AND_RETURN;

    #undef WRITE_SINGLE_COMMAND

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode tag_buffer_ctor(TagBuffer* tag)
{
    assert(tag);

    tag->bufferName = (Tag*) calloc(TAG_BUFFER_SIZE, sizeof(Tag));

    tag->size = 0;

    if (!tag->bufferName)
    {
        return ALLOC_MEMORY_ERROR;
    }

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode tag_buffer_dtor(TagBuffer* tag)
{
    assert(tag);

    free(tag->bufferName);

    tag->size = 0;

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode add_tag_to_buffer(const char* tagName, int ip, TagBuffer* tagBuffer)
{
    assert(tagName);
    assert(tagBuffer);

    for (size_t i = 0; i < tagBuffer->size; i++)
    {
        if (!strcmp(tagName, (tagBuffer->bufferName)[i].name))
        {
            (tagBuffer->bufferName)[i].ip = ip;
            return NO_ASSEMBLER_ERRORS;
        }
    }

    (tagBuffer->bufferName)[tagBuffer->size].ip   = ip;
    strcpy((tagBuffer->bufferName)[tagBuffer->size].name, tagName);

    (tagBuffer->size)++;

    return NO_ASSEMBLER_ERRORS;
}

asmErrorCode get_tag_ip(TagBuffer* tagBuffer, const char* tagName, int* ip)
{
    assert(tagBuffer);
    assert(tagName);

    for (size_t i = 0; i < tagBuffer->size; i++)
    {
        if (!strcmp(tagName, (tagBuffer->bufferName)[i].name))
        {
            *ip = (tagBuffer->bufferName)[i].ip;
            return NO_ASSEMBLER_ERRORS;
        }
    }

    return TAG_NOT_FOUND;
}