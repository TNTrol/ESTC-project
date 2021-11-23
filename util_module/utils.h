#ifndef STRUCTS_H
#define STRUCTS_H

#define CICLE_INCREMENT(number, max) (((number) + 1) % (max))
#define MOD_INCREMENT(mod)  CICLE_INCREMENT(mod, MOD_NUMBER)
typedef enum
{
    MOD_NONE = 0,
    MOD_H,
    MOD_S,
    MOD_V,
    MOD_NUMBER
}modificator_t;


#endif