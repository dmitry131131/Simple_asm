/**
 * @file
 * @brief Main asembler functions
*/
#ifndef ASSEMBLER_FUNC_H
#define ASSEMBLER_FUNC_H

enum registerNames {
    NO_NAME_REG = 0,
    RAX         = 1,
    RBX         = 2,
    RCX         = 3,
    RDX         = 4
};

enum asmErrorCode main_assembler_function(textData* text);

#endif