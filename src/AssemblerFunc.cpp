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

enum asmErrorCode main_assembler_function(textData* text)
{
    assert(text);

    outputBuffer binBuffer  = {};
    asmErrorCode error = NO_ASSEMBLER_ERRORS;

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

        else if (!strcmp(command, "jmp"))
        {
            fprintf(outputTextFile, "99 ");

            if ((error = write_char_to_buffer(&binBuffer, 13)))
            {
                break;
            }

            int tagIp = -1;
            if (sscanf(text->linesPtr[i], "%s %d", command, &tagIp) == 2)
            {
                fprintf(outputTextFile, "%d\n", tagIp);
            
                if ((error = write_int_to_buffer(&binBuffer, tagIp)))
                {
                    break;
                }
                continue;
            }

            char tagName[MAX_COMMAND_LEN] = {};

            if (sscanf(text->linesPtr[i], "%s %s", command, tagName) != 2)
            {
                error = INVALID_SYNTAX;
                print_assembler_error(error, text->linesPtr[i], i+1);
                break;
            }

            tagIp = -1;
            if ((error = get_tag_ip(&tagBuffer, tagName, &tagIp)))
            {
                print_assembler_error(error, text->linesPtr[i], i+1);
                break;
            }

            fprintf(outputTextFile, "%d\n", tagIp);
            
            if ((error = write_int_to_buffer(&binBuffer, tagIp)))
            {
                break;
            }

        }

        else if (!strcmp(command, "push"))
        {
            char cmd = 0;

            if(sscanf(text->linesPtr[i], "%s %lf", command, &commandArg) == 2)
            {
                cmd |= 1 << 0;
                cmd |= 1 << 4;

                fprintf(outputTextFile, "17 %lf\n", commandArg);

                if ((error = write_char_to_buffer(&binBuffer, cmd)))
                {
                    break;
                }

                if ((error = write_double_to_buffer(&binBuffer, commandArg)))
                {
                    break;
                }
            }

            else if (sscanf(text->linesPtr[i], "%s %s", command, registerName) == 2)
            {
                cmd |= 1 << 0;
                cmd |= 1 << 5;
                registerNames reg = NO_NAME_REG;

                fprintf(outputTextFile, "33 ");

                if ((error = write_char_to_buffer(&binBuffer, cmd)))
                {
                    break;
                }

                if (registerName[0] == 'r' && registerName[2] == 'x')
                {
                    switch (registerName[1])
                    {
                    case 97:
                        reg = RAX;
                        break;
                    case 98:
                        reg = RBX;
                        break;
                    case 99:
                        reg = RCX;
                        break;
                    case 100:
                        reg = RDX;
                        break;
                    
                    default:
                        error = WRONG_REGISTER_NAME;
                        print_assembler_error(error, text->linesPtr[i], i+1);
                        break;
                    }
                }
                else
                {
                    error = INVALID_SYNTAX;
                    print_assembler_error(error, text->linesPtr[i], i+1);
                    break;
                }

                if (error) break;

                fprintf(outputTextFile, "%d\n", reg);

                if ((error = write_char_to_buffer(&binBuffer, reg)))
                {
                    break;
                }
            }

            else
            {
                error = INVALID_SYNTAX;
                print_assembler_error(error, text->linesPtr[i], i+1);
                break;
            }
        }

        else if (!strcmp(command, "pop"))
        {
            if (sscanf(text->linesPtr[i], "%s %s", command, registerName) != 2)
            {
                error = INVALID_SYNTAX;
                print_assembler_error(error, text->linesPtr[i], i+1);
                break;
            }

            char cmd = 0;

            cmd |= 1 << 0;
            cmd |= 1 << 2;
            cmd |= 1 << 3;
            cmd |= 1 << 5;
            registerNames reg = NO_NAME_REG;

            fprintf(outputTextFile, "45 ");

            if ((error = write_char_to_buffer(&binBuffer, cmd)))
            {
                break;
            }

            if (registerName[0] == 'r' && registerName[2] == 'x')
            {
                switch (registerName[1])
                {
                case 97:
                    reg = RAX;
                    break;
                case 98:
                    reg = RBX;
                    break;
                case 99:
                    reg = RCX;
                    break;
                case 100:
                    reg = RDX;
                    break;
                
                default:
                        error = WRONG_REGISTER_NAME;
                        print_assembler_error(error, text->linesPtr[i], i+1);
                        break;
                    }
            }
            else
            {
                error = INVALID_SYNTAX;
                print_assembler_error(error, text->linesPtr[i], i+1);
                break;
            }

            if (error) break;

            fprintf(outputTextFile, "%d\n", reg);

            if ((error = write_char_to_buffer(&binBuffer, reg)))
            {
                break;
            }
        }

        else if (!strcmp(command, "add"))
        {
            WRITE_SINGLE_COMMAND(2);
        }

        else if (!strcmp(command, "sub"))
        {
            WRITE_SINGLE_COMMAND(3);
        }
        
        else if (!strcmp(command, "mul"))
        {
            WRITE_SINGLE_COMMAND(4);
        }
        
        else if (!strcmp(command, "div"))
        {
            WRITE_SINGLE_COMMAND(5);
        }
        
        else if (!strcmp(command, "out"))
        {
            WRITE_SINGLE_COMMAND(6);
        }
        
        else if (!strcmp(command, "sqrt"))
        {
            WRITE_SINGLE_COMMAND(7);
        }
        
        else if (!strcmp(command, "sin"))
        {
            WRITE_SINGLE_COMMAND(8);
        }
        
        else if (!strcmp(command, "cos"))
        {
            WRITE_SINGLE_COMMAND(9);
        }
        
        else if (!strcmp(command, "tan"))
        {
            WRITE_SINGLE_COMMAND(10);
        }

        else if (!strcmp(command, "in"))
        {
            WRITE_SINGLE_COMMAND(11);
        }
        
        else if (!strcmp(command, "hlt"))
        {
            WRITE_SINGLE_COMMAND(12);
        }
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