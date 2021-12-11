#include "command.h"
#include "stdlib.h"
#include "string.h"

static uint8_t m_word = 0;
static bool m_is_prev_space = false;
static const command_t *m_commands = NULL;
static uint8_t m_count_command = 0;

static char m_line_data[COMMAND_BUFFER];
static uint8_t m_position;
static command_handler m_default_handler = NULL;

void reset_variable()
{
    m_word = 0;
    m_position = 0;
    m_is_prev_space = false;
}

uint8_t convert_command(const char *word, uint8_t size)
{
    for (uint8_t i = 0; i < m_count_command; ++i)
    {
        if(strncmp(m_commands[i].name, word, size) == 0)
        {
            return i;
        }
    }
    return UINT8_MAX;
}

void parse_chars_command()
{
    if(m_word == 0 && m_position == 0)
    {
        return;
    }
    uint8_t len = strlen(m_line_data);
    uint8_t index_cmd = convert_command(m_line_data, len);
    if(index_cmd < UINT8_MAX)
    {
        m_commands[index_cmd].handler(m_line_data + len + (m_word > 0), m_word);
        return;
    }
    m_default_handler(m_line_data, m_word);
}

void push_char_to_command(char c)
{
    if(c == ' ')
    {
        m_is_prev_space = m_word > 0 || m_position > 0;
        return;
    }
    if(c == '\n' || c == '\0' || c == '\r')
    {
        m_line_data[m_position] = '\0';
        parse_chars_command();
        reset_variable();
        return;
    }
    if(m_position >= COMMAND_BUFFER - 1)
    {
        return;
    }
    if(m_is_prev_space)
    {
        m_is_prev_space = false;
        m_word++;
        m_line_data[m_position++] = '\0';
    }
    m_line_data[m_position++] = c;
}

void init_parse(uint8_t size, const command_t *commands, command_handler default_handler)
{
    m_count_command = size;
    m_commands = commands;
    m_default_handler = default_handler;
    reset_variable();
}