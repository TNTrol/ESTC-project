#include "command.h"
#include "stdlib.h"
#include "string.h"

static command_ctx_t m_command_ctx = {.count_word = 0, .is_prev_space = false, .position = 0};

void reset_variable()
{
    m_command_ctx.position = 0;
    m_command_ctx.count_word = 0;
    m_command_ctx.is_prev_space = false;
}

uint8_t convert_command(const char *word, uint8_t size)
{
    for (uint8_t i = 0; i < m_command_ctx.count_commands; ++i)
    {
        if(strncmp(m_command_ctx.commands[i].name, word, size) == 0)
        {
            return i;
        }
    }
    return COMMAND_NOT_FOUND;
}

void parse_chars_command()
{
    if(m_command_ctx.count_word == 0 && m_command_ctx.position == 0)
    {
        return;
    }
    uint8_t len = strlen(m_command_ctx.line);
    uint8_t index_cmd = convert_command(m_command_ctx.line, len);
    if(index_cmd != COMMAND_NOT_FOUND)
    {
        m_command_ctx.commands[index_cmd].handler(m_command_ctx.line + len + (m_command_ctx.count_word > 0), m_command_ctx.count_word);
        return;
    }
    m_command_ctx.default_handler(m_command_ctx.line, m_command_ctx.count_word);
}

void push_char_to_command(char c)
{
    if(c == ' ')
    {
        m_command_ctx.is_prev_space = m_command_ctx.count_word > 0 || m_command_ctx.position > 0;
        return;
    }
    if(c == '\n' || c == '\0' || c == '\r')
    {
        m_command_ctx.line[m_command_ctx.position] = '\0';
        parse_chars_command();
        reset_variable();
        return;
    }
    if(m_command_ctx.position >= COMMAND_BUFFER - 1)
    {
        return;
    }
    if(m_command_ctx.is_prev_space)
    {
        m_command_ctx.is_prev_space = false;
        ++m_command_ctx.count_word;
        m_command_ctx.line[m_command_ctx.position++] = '\0';
    }
    m_command_ctx.line[m_command_ctx.position++] = c;
}

void init_parse(uint8_t size, const command_t *commands, const command_handler default_handler)
{
    m_command_ctx.commands = commands;
    m_command_ctx.count_commands = size;
    m_command_ctx.default_handler = default_handler;
}