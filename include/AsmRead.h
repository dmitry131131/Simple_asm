/**
 * @file
 * @brief Functions that read programm file 
*/
#ifndef ASM_READ_H
#define ASM_READ_H

textData* asm_prepare(const char* filename);

textData* text_to_lower(textData* text);

textData* remove_comments_from_text(textData* text);

#endif