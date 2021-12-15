#include "color_module.h"
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

void hsv_to_rgb(const hsv_t *hsv, rgb_t *rgb)
{
    uint32_t region, remainder, p, q, t;
    if (hsv->s == 0)
    {
        rgb->r = hsv->v;
        rgb->g = hsv->v;
        rgb->b = hsv->v;
        return;
    }
    region = hsv->h / 43;
    remainder = (hsv->h - (region * 43)) * 6;

    p = (hsv->v * (255 - hsv->s)) >> 8;
    q = (hsv->v * (255 - ((hsv->s * remainder) >> 8))) >> 8;
    t = (hsv->v * (255 - ((hsv->s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb->r = hsv->v; 
            rgb->g = t; 
            rgb->b = p;
            break;
        case 1:
            rgb->r = q; 
            rgb->g = hsv->v; 
            rgb->b = p;
            break;
        case 2:
            rgb->r = p; 
            rgb->g = hsv->v; 
            rgb->b = t;
            break;
        case 3:
            rgb->r = p; 
            rgb->g = q; 
            rgb->b = hsv->v;
            break;
        case 4:
            rgb->r = t; 
            rgb->g = p; 
            rgb->b = hsv->v;
            break;
        default:
            rgb->r = hsv->v; rgb->g = p; rgb->b = q;
            break;
    }
}

void rgb_to_hsv(const rgb_t *rgb, hsv_t* hsv)
{
    unsigned char rgb_min, rgb_max;
    rgb_max = MAX(MAX(rgb->r, rgb->g), rgb->b);
    rgb_min = MIN(MIN(rgb->r, rgb->g), rgb->b);

    hsv->v = rgb_max;
    if (hsv->v == 0)
    {
        hsv->h = 0;
        hsv->s = 0;
        return;
    }

    hsv->s = 255 * ((long)(rgb_max - rgb_min)) / hsv->v;
    if (hsv->s == 0)
    {
        hsv->h = 0;
        return;
    }

    if (rgb_max == rgb->r)
    {
        hsv->h = 0 + 43 * (rgb->g - rgb->b) / (rgb_max - rgb_min);
    }
    else if (rgb_max == rgb->g)
    {
        hsv->h = 85 + 43 * (rgb->b - rgb->r) / (rgb_max - rgb_min);
    }    
    else
    {
        hsv->h = 171 + 43 * (rgb->r - rgb->g) / (rgb_max - rgb_min);
    }   
}