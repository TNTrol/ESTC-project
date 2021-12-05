#ifndef CONVERT_CMD_H
#define CONVERT_CMD_H
#include <stdint.h>
#include <stdbool.h>

typedef void(*func_convert)(bool flag, uint8_t data1, uint8_t data2, uint8_t data3);

void init_convert(func_convert callback);
void print_char(char c);
#endif