/**
 * @file 
 * @brief Functions and enums with asembler errors information
*/
#ifndef ASM_ERRORS_H
#define ASM_ERRORS_H

enum asmErrorCode {
    NO_ASSEMBLER_ERRORS,
    BUFFER_CTOR_ERROR,
    BUFFER_DTOR_ERROR,
    WRONG_FILE_CREATE_MODE,
    FILE_CREATING_ERROR,
    ALLOC_MEMORY_ERROR,
    FWRITE_ERROR,
    INVALID_SYNTAX,
    WRONG_REGISTER_NAME,
    

};

#endif