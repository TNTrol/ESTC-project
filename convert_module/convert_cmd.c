#include "convert_cmd.h"

#define SIZE_CONVERT 4

static const char *m_hvs = "hsv", *m_rgb = "rgb";
static uint8_t m_numbers[SIZE_CONVERT - 1];
static uint8_t m_word = 0;
static uint8_t m_pos = 0;
static bool m_prev_space = false;
static bool m_is_rgb = false;
static func_convert m_callback;

void init_convert(func_convert callback)
{
    m_callback = callback;
}

static void reset()
{
    m_word = 0;
    m_pos = 0;
}

static bool start_cmd(char c)
{
    switch (c)
    {
        case 'r':
            m_is_rgb = true;
            m_pos = 0;
            break;
        case 'h':
            m_is_rgb = false;
            m_pos = 0;
            break;
        default:
            return false;
    }
    return true;
}

static bool convert_command(char c)
{
    if(m_pos == 0)
    {
        return start_cmd(c);
    }
    if(m_is_rgb)
    {
        return m_rgb[m_pos] == c || start_cmd(c);
    }
    return m_hvs[m_pos] == c || start_cmd(c);
}

static inline void calculate()
{
    m_callback(m_is_rgb, m_numbers[0], m_numbers[1], m_numbers[2]);
}

void print_char(char c)
{
    if(c == ' ')
    {
        if(!m_prev_space && m_word == SIZE_CONVERT - 1)
        {
            calculate();
            reset();
        }
        m_prev_space = m_word > 0 || (m_word == 0 && m_pos > 0);
        return;
    }
    if(c == '\n' || c == '\0')
    {
        if(m_word == SIZE_CONVERT - 1)
        {
            calculate();
        }
        reset();
        return;
    }
    if(m_prev_space)
    {
        m_prev_space = false;
        m_numbers[m_word] = 0;
        m_word++;
        m_pos = 0;
    }
    if(m_word > 0 && 48 <= c && c < 58)
    {
        uint16_t a = m_numbers[m_word - 1] * 10;
        if(a > 255 || 255 - a < c - 48)
        {
            reset();
        }
        else
        {
            m_numbers[m_word - 1] = a + (c - 48);
            ++m_pos;
        }
    }
    else if(m_word > 0)
    {
        if(m_word == SIZE_CONVERT - 1 && m_pos > 0)
        {
            calculate();
        }
        reset();
    }
    if(m_word == 0)
    {
        if(convert_command(c))
        {
            ++m_pos;
        }
        else
        {
            reset();
        }
    }
}