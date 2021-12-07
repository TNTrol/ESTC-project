#include "convert_cmd.h"

static word_t m_words[COUNT_WORD];
static uint8_t m_word = 0;
static bool m_prev_space = false;
static uint8_t m_args[COUNT_WORD - 1];
static state_parse_t m_state = SUCCESS;
static command_t *m_commands;
static uint8_t m_count_command = 0;
static function_command m_callback = 0;

void reset_variable()
{
    m_word = 0;
    m_words[0].size = 0;
    m_state = SUCCESS;
}

bool parse_chars_to_uint8(const char *word, uint8_t size, uint8_t *out)
{
    uint8_t number = 0, temp = 0;
    for(uint8_t i = 0; i < size; ++i)
    {
        temp = word[i] - 48;
        if(temp > 255 - number * 10)
        {
            return false;
        }
        number = 10 * number + temp;
    }
    *out = number;
    return true;
}

uint8_t cmp_chars(const char *s1, const char *s2, uint8_t size)
{
    for (uint8_t i  = 0; i < size && s1[i] != '\0'; ++i)
    {
        if(s1[i] != s2[i])
        {
            return 1;
        }
    }
    return 0;
}

bool convert_command1(const char *word, uint8_t *index)
{
    for (uint8_t i = 0; i < m_count_command; ++i)
    {
        if(!cmp_chars(m_commands[i].name, word, SIZE_WORD))
        {
            *index = i;
            return true;
        }
    }
    return false;
}

bool parse_chars_command()
{
    if(m_state != SUCCESS)
    {
        m_callback(m_state, 255, 0);
        return false;
    }
    if(m_word == 0 && m_words[0].size == 0)
    {
        return false;
    }
    uint8_t index = 0;
    if(!convert_command1(m_words[0].arr, &index))
    {
        m_state = COMMAND_NOT_FOUND_ERROR;
        m_callback(m_state, 255, 0);
        return false;
    }
    if(m_word != m_commands[index].count_argument)
    {
        m_state = m_word > m_commands[index].count_argument ? UNEXPECTED_ARGUMENT_ERROR : MISSING_ARGUMENT_ERROR;
        m_callback(m_state, 255, 0);
        return false;
    }
    for(uint8_t i = 1; i <= m_commands[index].count_argument; ++i)
    {
        if(!parse_chars_to_uint8(m_words[i].arr, m_words[i].size, &m_args[i - 1]))
        {
            m_state = PARSE_ARGUMENT_ERROR;
            break;
        }
    }
    m_callback(m_state, index, m_state == SUCCESS ? m_args : 0);
    return true;
}

void print_char(char c)
{
    if(c == ' ')
    {
        m_prev_space = m_word > 0 || m_words[0].size > 0;
        return;
    }
    if(c == '\n' || c == '\0' || c == '\r')
    {
        parse_chars_command();
        reset_variable();
        return;
    }
    if(m_prev_space)
    {
        m_prev_space = false;
        m_word++;
        if(m_word < COUNT_WORD)
        {
            m_words[m_word].size = 0;
        }
    }
    if(m_word >= COUNT_WORD)
    {
        m_state = ARGUMENT_COUNT_ERROR;
        return;
    }
    if(m_words[m_word].size >= SIZE_WORD)
    {
        m_state = SIZE_WORD_ERROR;
        return;
    }
    m_words[m_word].arr[m_words[m_word].size++] = c;
}

void init_parse(function_command method, uint8_t size, command_t *commands)
{
    m_callback = method;
    m_count_command = size;
    m_commands = commands;
    reset_variable();
}