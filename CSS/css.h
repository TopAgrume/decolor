#ifndef CSS_H
#define CSS_H

#include <stdio.h>
#include <stdlib.h>

char char_to_int(char c);

int hexa_to_int(char b1, char b0);

char int_to_hexa(char c);

char* ints_to_hexas(int color);

int hexas_to_ints(char* color);

void color_calculator(char* color, float percent, int offset, char is_black_and_white);

void color_update(char* color, char* color_changed);

void CSS_rewrite_light(char color[]);

void CSS_rewrite_dark(char color[]);

#endif
