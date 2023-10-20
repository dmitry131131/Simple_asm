/**
 * @file 
 * @brief Functions printing asembler errors information
*/
#include <stdio.h>

#include "Color_output.h"
#include "AsmErrors.h"

void print_assembler_error(asmErrorCode error, const char* lineText, size_t line)
{
    color_fprintf(stderr, COLOR_BLUE, STYLE_UNDERLINED,  "In line:");
    fputc(' ', stderr);
    color_fprintf(stderr, COLOR_DEFAULT, STYLE_INVERT_C, "%lu", line);
    fputs(": ", stderr);
    fputs(lineText, stderr);
    fputc('\n', stderr);

    print_assembler_error_message(error, stderr);
}

void print_assembler_error_message(asmErrorCode error, FILE* stream)
{
    color_fprintf(stream, COLOR_RED, STYLE_BOLD, "Assembler error: ");

    #define CHECK_CODE(code, message)               \
        case code:                                  \
            fprintf(stream, message);               \
            break;                                  \

    switch (error)
    {
        case NO_ASSEMBLER_ERRORS:
            break;
            
        CHECK_CODE(BUFFER_CTOR_ERROR,        "Can't construct buffer!\n");
        CHECK_CODE(BUFFER_DTOR_ERROR,        "Can't destruct buffer!\n");
        CHECK_CODE(WRONG_FILE_CREATE_MODE,   "Invalid file creting mode given!\n");
        CHECK_CODE(FILE_CREATING_ERROR,      "Can't create file!\n");
        CHECK_CODE(ALLOC_MEMORY_ERROR,       "Can't alloc memory for buffer!\n");
        CHECK_CODE(FWRITE_ERROR,             "Can't write information in file!\n");
        CHECK_CODE(INVALID_SYNTAX,           "Invalid syntax!\n");
        CHECK_CODE(WRONG_REGISTER_NAME,      "Invalid register name!\n");
        CHECK_CODE(FEW_CONSOLE_ARGS,         "Too few arguments for assembler!\n");
        CHECK_CODE(TAG_NOT_FOUND,            "Tag not found in tag buffer!\n");

    default:
        fprintf(stream, "Unknown error!\n");
        break;
    }
    #undef CHECK_CODE
}

void print_filename(FILE* stream ,const char* filename)
{
    color_fprintf(stream, COLOR_BLUE, STYLE_UNDERLINED, "File:");
    fputc(' ', stream);
    fputs(filename, stream);
    fputc('\n', stream);
}