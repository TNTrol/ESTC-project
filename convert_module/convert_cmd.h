#ifndef CONVERT_CMD_H
#define CONVERT_CMD_H
#include <stdint.h>
#include <stdbool.h>

#define COUNT_WORD 4
#define SIZE_WORD 4

typedef struct
{
    char arr[SIZE_WORD];
    uint8_t size;
}word_t;

typedef struct
{
    const char *name;
    const uint8_t count_argument;//--3
}command_t;

typedef enum
{
    SUCCESS = 0,
    UNEXPECTED_ARGUMENT_ERROR,
    MISSING_ARGUMENT_ERROR,
    COMMAND_NOT_FOUND_ERROR,
    PARSE_ARGUMENT_ERROR,
    ARGUMENT_COUNT_ERROR,
    SIZE_WORD_ERROR
}state_parse_t;

typedef void(*function_command)(state_parse_t state, uint8_t command_num, uint8_t *args);

void push_char_to_command(char c);
void init_parse(function_command method, uint8_t size, command_t *commands);
#endif