#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>

uint32_t circle_increment(uint32_t number, uint32_t max);

#define MOD_INCREMENT(mod)  circle_increment(mod, MOD_NUMBER)
typedef enum
{
    MOD_NONE = 0,
    MOD_H,
    MOD_S,
    MOD_V,
    MOD_NUMBER
}modificator_t;

#endif