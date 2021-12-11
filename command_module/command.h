#ifndef CONVERT_CMD_H
#define CONVERT_CMD_H
#include <stdint.h>
#include <stdbool.h>

#define COMMAND_BUFFER 30

typedef void(*command_handler)(const char *, uint8_t count_word);

typedef struct
{
    const char *name;
    command_handler handler;
}command_t;

typedef struct 
{
    command_t *commands;
    command_handler default_handler;
    uint8_t count_commands;
}command_ctx_t;

void push_char_to_command(char c);
void init_parse(uint8_t size, const command_t *commands, command_handler default_handler);

#endif