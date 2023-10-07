#include <stdio.h>

#include "OneginError.h"
#include "AsmErrors.h"
#include "InputOutput.h"
#include "AsmRead.h"
#include "AssemblerFunc.h"

int main()
{
    textData* text = asm_prepare("text.txt");

    main_assembler_function(text);

    remove_text(text);
    return 0;
}