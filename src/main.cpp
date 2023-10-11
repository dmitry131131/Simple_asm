#include <stdio.h>

#include "OneginError.h"
#include "AsmErrors.h"
#include "InputOutput.h"
#include "AsmRead.h"
#include "AssemblerFunc.h"

int main(int argc, char* argv[])
{
    textData* text = NULL;
    asmErrorCode error = NO_ASSEMBLER_ERRORS;

    if (argc == 1)
    {
        print_assembler_error_message(FEW_CONSOLE_ARGS, stderr);
        return 0;
    }

    for (int i = 1; i < argc; i++)
    {
        text = asm_prepare(argv[i]);
        if ((error = main_assembler_function(text)))
        {
            print_assembler_error_message(error, stderr);
            return 0;
        }
    }

    remove_text(text);
    return 0;
}