/**
 * @file
 * @brief Functions output assembled commans
*/
#ifndef ASM_OUT_H
#define ASM_OUT_H

enum fileCreateMode {
    BIN,
    TEXT
};

FILE* create_output_file(const char* filename, fileCreateMode modeCode);

asmErrorCode write_header_info(FILE* outputTextFile, FILE* outputBinFile, int version, size_t commandCount);

asmErrorCode write_int_to_bin_file(FILE* file, int num);

asmErrorCode write_char_to_bin_file(FILE* file, char num);

asmErrorCode write_double_to_bin_file(FILE* file, double num);

#endif