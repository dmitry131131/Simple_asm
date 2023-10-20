/**
 * @file
 * @brief Main asembler functions
*/
#ifndef ASSEMBLER_FUNC_H
#define ASSEMBLER_FUNC_H

const int TAG_BUFFER_SIZE  = 100;
const int MAX_COMMAND_LEN  = 150;
const int MAX_REGISTER_LEN = 10;

enum registerNames {
    NO_NAME_REG = 0,
    RAX         = 1,
    RBX         = 2,
    RCX         = 3,
    RDX         = 4
};

struct Tag {
    char name[150];
    int ip;
};

struct TagBuffer {
    size_t size;
    Tag*   bufferName;
};

#define DESTRUCT_ALL_BUFFERS_AND_RETURN do{     \
                                                \
    error = buffer_dtor(&binBuffer);            \
    error = tag_buffer_dtor(&tagBuffer);        \
                                                \
    fclose(outputTextFile);                     \
    fclose(outputBinFile);                      \
                                                \
}while(0)

enum asmErrorCode main_assembler_function(textData* text);

asmErrorCode tag_buffer_ctor(TagBuffer* tag);

asmErrorCode tag_buffer_dtor(TagBuffer* tag);

asmErrorCode add_tag_to_buffer(const char* tagName, int ip, TagBuffer* tagBuffer);

asmErrorCode get_tag_ip(TagBuffer* tagBuffer, const char* tagName, int* ip);

#endif