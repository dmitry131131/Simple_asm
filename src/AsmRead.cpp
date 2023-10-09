/**
 * @file
 * @brief Functions that read programm file
*/

#include <stdio.h>

#include "OneginError.h"
#include "InputOutput.h"
#include "AsmRead.h"

textData* asm_prepare(const char* filename)
{
    if (!filename)
    {
        // noptr
    }
    static textData text = {};
    errorCode err = NO_ERRORS;

    CHECK_ERR_MAIN(get_text(filename, &text));

    return &text;
}

textData* text_to_lower(textData* text)
{
    if (!text)
    {
        // text noptr
    }
    for (size_t i = 0; i < text->bufferSize; i++)
    {
        if ((text->bufferName[i] >= 65) && (text->bufferName[i] < 92))
        {
            text->bufferName[i] += 32;
        }
    }

    return text;
}