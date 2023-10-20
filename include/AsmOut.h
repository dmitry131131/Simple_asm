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

struct outputBuffer {
    char* Buffer;
    size_t bufferPointer;
};

asmErrorCode buffer_ctor(outputBuffer* buffer, size_t size);

asmErrorCode write_buffer_to_file(FILE* file, outputBuffer* buffer);

asmErrorCode buffer_dtor(outputBuffer* buffer);

asmErrorCode create_output_file(FILE** file, const char* filename, fileCreateMode modeCode);

asmErrorCode write_header_info(FILE* outputTextFile, FILE* outputBinFile, int version, size_t commandCount);

asmErrorCode write_int_to_bin_file(FILE* file, int num);

asmErrorCode write_char_to_bin_file(FILE* file, char num);

asmErrorCode write_double_to_bin_file(FILE* file, double num);

asmErrorCode create_command_buffer(char** buffer, size_t size);

asmErrorCode write_double_to_buffer(outputBuffer* buffer, double num);

asmErrorCode write_char_to_buffer(outputBuffer* buffer, char num);

asmErrorCode write_int_to_buffer(outputBuffer* buffer, int num);

#endif