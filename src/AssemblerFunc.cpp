/**
 * @file
 * @brief Main asembler functions sources
*/
#include <stdio.h>
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

    if (buffer_ctor(&binBuffer))
    {
        return BUFFER_CTOR_ERROR;
    }

    if ((error = create_command_buffer(&(binBuffer.Buffer), text->bufferSize * 4)))
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

    #define WRITE_SINGLE_COMMAND(num) do{                   \
                                                            \
    fprintf(outputTextFile, #num "\n");                     \
    write_char_to_buffer(&binBuffer, num);                  \
                                                            \
    }while(0)

    write_header_info(outputTextFile, outputBinFile, 1, text->linesCount);

    for (size_t i = 0; i < text->linesCount; i++)
    {
        char command[25] = {};
        double commandArg = 0;
        char registerName[10] = {};

        if(sscanf(text->linesPtr[i], "%s", command) != 1)
        {
            error = INVALID_SYNTAX;
            print_assembler_error(error, text->linesPtr[i], i+1);
            break;
        }

        if (!strcmp(command, "push"))
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
        if ((error = buffer_dtor(&binBuffer)))
        {
            return error;
        }
        fclose(outputTextFile);
        fclose(outputBinFile);

        return error;
    }

    if ((error = write_buffer_to_file(outputBinFile, &binBuffer)))
    {
        return error;
    }

    if ((error = buffer_dtor(&binBuffer)))
    {
        return error;
    }
    fclose(outputTextFile);
    fclose(outputBinFile);

    #undef WRITE_SINGLE_COMMAND

    return NO_ASSEMBLER_ERRORS;
}
