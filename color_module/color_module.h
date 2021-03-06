#ifndef COLOR_MODULE_H
#define COLOR_MODULE_H
#include <stdint.h>

typedef struct 
{
    uint8_t h;
    uint8_t v;
    uint8_t s;
}hsv_t;

typedef struct 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}rgb_t;

typedef union
{
    hsv_t hsv;
    rgb_t rgb;
    uint8_t components[3];
} color_t;


void hsv_to_rgb(const hsv_t* hvs, rgb_t *rgb);
void rgb_to_hsv(const rgb_t *rgb, hsv_t* hsv);

#endif