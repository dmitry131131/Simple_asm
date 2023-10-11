/**
 * @file
 * @brief Functions that read programm file
*/

#include <stdio.h>
#include <assert.h>

#include "OneginError.h"
#include "InputOutput.h"
#include "AsmRead.h"

textData* asm_prepare(const char* filename)
{
    assert(filename);

    static textData text = {};
    errorCode err = NO_ERRORS;

    CHECK_ERR_MAIN(get_text(filename, &text));

    text_to_lower(&text);

    remove_comments_from_text(&text);

    return &text;
}

textData* text_to_lower(textData* text)
{
    assert(text);

    for (size_t i = 0; i < text->bufferSize; i++)
    {
        if ((text->bufferName[i] >= 65) && (text->bufferName[i] < 92))
        {
            text->bufferName[i] += 32;
        }
    }

    return text;
}

textData* remove_comments_from_text(textData* text)
{
    assert(text);

    for (size_t i = 0; i < text->bufferSize; i++)
    {
        if (text->bufferName[i] == ';')
        {
            text->bufferName[i] = 0;
        }
    }

    return text;
}