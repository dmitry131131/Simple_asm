/**
 * @file
 * @brief Main asembler functions sources
*/
#include <stdio.h>
#include <string.h>

#include "OneginError.h"
#include "InputOutput.h"
#include "AsmErrors.h"
#include "AsmOut.h"
#include "AssemblerFunc.h"

enum asmErrorCode main_assembler_function(textData* text)
{
    if (!text)
    {
        // text null
    }

    #define WRITE_SINGLE_COMMAND(num) do{               \
    fprintf(outputTextFile, #num "\n");                 \
                                                        \
    if (write_char_to_bin_file(outputBinFile, num))     \
    {                                                   \
                                                        \
    }                                                   \
    }while(0)

    FILE* outputBinFile = create_output_file("outbin", BIN);
    FILE* outputTextFile = create_output_file("out.txt", TEXT);

    write_header_info(outputTextFile, outputBinFile, 1, text->linesCount);

    for (size_t i = 0; i < text->linesCount; i++)
    {
        char command[25] = {};
        double commandArg = 0;
        char registerName[10] = {};

        if(sscanf(text->linesPtr[i], "%s", command) != 1)
        {
            printf("error!\n");
            // input error
        }

        if (!strcmp(command, "push"))
        {
            char cmd = 0;

            if(sscanf(text->linesPtr[i], "%s %lf", command, &commandArg) == 2)
            {
                cmd |= 1 << 0;
                cmd |= 1 << 4;

                fprintf(outputTextFile, "17 %lf\n", commandArg);

                if (write_char_to_bin_file(outputBinFile, cmd))
                {
                    // write to bin error
                }

                if (write_double_to_bin_file(outputBinFile, commandArg))
                {
                    // write to bin error
                }

            }
            else if (sscanf(text->linesPtr[i], "%s %s", command, registerName) == 2)
            {
                cmd |= 1 << 0;
                cmd |= 1 << 5;
                registerNames reg = NO_NAME_REG;

                fprintf(outputTextFile, "33 %lf\n", commandArg);

                if (write_char_to_bin_file(outputBinFile, cmd))
                {
                    // write to bin error
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
                        // register name error
                        break;
                    }
                }

                if (write_char_to_bin_file(outputBinFile, reg))
                {
                    // write to bin error
                }
            }

            else
            {
                // input push error
            }
        }

        else if (!strcmp(command, "pop"))
        {
            if (sscanf(text->linesPtr[i], "%s %s", command, registerName) != 2)
            {
                // input pop error
            }

            char cmd = 0;

            cmd |= 1 << 0;
            cmd |= 1 << 1;
            cmd |= 1 << 3;
            cmd |= 1 << 5;
            registerNames reg = NO_NAME_REG;

            fprintf(outputTextFile, "43 %lf\n", commandArg);

            if (write_char_to_bin_file(outputBinFile, cmd))
            {
                // write to bin error
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
                    // register name error
                    break;
                }
            }

            if (write_char_to_bin_file(outputBinFile, reg))
            {
                // write to bin error
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
        
        else if (!strcmp(command, "htl"))
        {
            WRITE_SINGLE_COMMAND(12);
        }
        
    }

    fclose(outputTextFile);
    fclose(outputBinFile);

    #undef WRITE_SINGLE_COMMAND

    return NO_ASSEMBLER_ERRORS;
}
