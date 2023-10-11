#include <stdio.h>

#include "OneginError.h"
#include "AsmErrors.h"
#include "InputOutput.h"
#include "AsmRead.h"
#include "AssemblerFunc.h"

int main()
{
    textData* text = asm_prepare("text.txt");
    asmErrorCode error = NO_ASSEMBLER_ERRORS;

    if ((error = main_assembler_function(text)))
    {
        
        return 0;
    }

    remove_text(text);
    return 0;
}