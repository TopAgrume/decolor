#include "css.h"

/////////////////////////////////////////////
            //CSS//
////////////////////////////////////////////

char char_to_int(char c)
{
    if ('0' <= c && c <= '9')
        return c-'0';
    
    else
        return c-'A'+10;
}

int hexa_to_int(char b1, char b0)
{
    b1 = char_to_int(b1);
    b0 = char_to_int(b0);

    return (int)(16*b1 + b0);
}

char int_to_hexa(char c)
{
    if (0 <= c && c <= 9)
        return c+'0';

    else
        return c+'A'-10;
}

char* ints_to_hexas(int color)
{
    char* col_hex = malloc(3*sizeof(char));
    col_hex[2] = '\0';

    col_hex[0] = int_to_hexa(color/16);
    col_hex[1] = int_to_hexa(color%16);

    return col_hex;
}

int hexas_to_ints(char* color)
{
    return (((int)char_to_int(color[0]))*16) + ((int)(char_to_int(color[1])));
}

void color_calculator(char* color, float percent, int offset, char is_black_and_white)
{
    int r = (int) (percent*hexa_to_int(color[0], color[1])) + offset;
    int g = (int) (percent*hexa_to_int(color[2], color[3])) + offset;
    int b = (int) (percent*hexa_to_int(color[4], color[5])) + offset;

    if (is_black_and_white)
    {
        r = 0.3*r + 0.59*g + 0.11*b;
        g = r;
        b = r;
    }

    if (r>255)
        r = 255;
    if (r<0)
        r = 0;

    if (g>255)
        g = 255;
    if (g<0)
        g = 0;

    if (b>255)
        b = 255;
    if (b<0)
        b = 0;

    char* hex;
    hex = ints_to_hexas(r);
    color[0] = hex[0];
    color[1] = hex[1];
    
    hex = ints_to_hexas(g);
    color[2] = hex[0];
    color[3] = hex[1];
    
    hex = ints_to_hexas(b);
    color[4] = hex[0];
    color[5] = hex[1];

    free(hex);
}


void color_update(char* color, char* color_changed)
{
    for (int i = 0; i < 6; i++)
        color_changed[i] = color[i];
}


void CSS_rewrite_light(char color[])
{
    //Rewriting
    FILE* file = fopen("CSS/color-theme-light.css", "r+");
    
    char color_changed[] = "FFFFFF";
    fprintf(file, "@define-color bg_color #%s;\n", color);

    color_update(color, color_changed);
    color_calculator(color_changed, 1.1, 10, 0);
    fprintf(file, "@define-color fg_color #%s;\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 0.8, 0, 0);
    fprintf(file, "@define-color dark_bg_color  #%s;\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 0.9, 0, 0);
    fprintf(file, "@define-color dark_fg_color  #%s;\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 0.75, 0, 0);
    fprintf(file, "@define-color bf_last_color #%s;\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 0.25, -10, 1);
    fprintf(file, "@define-color text_color #%s;\n\n", color_changed);
 
    fclose(file);
}

void CSS_rewrite_dark(char color[])
{
    //Rewriting
    FILE* file = fopen("CSS/color-theme-dark.css\0", "r+");
    
    char color_changed[] = "FFFFFF";
    fprintf(file, "@define-color bg_color #%s;\n", color);

    color_update(color, color_changed);
    color_calculator(color_changed, 2.5, 15, 0);
    fprintf(file, "@define-color fg_color #%s;\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 0.8, 0, 0);
    fprintf(file, "@define-color dark_bg_color  #%s;\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 1.1, 50, 0);
    fprintf(file, "@define-color dark_fg_color  #%s;\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 3, 30, 0);
    fprintf(file, "@define-color bf_last_color #%s;\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 3.5, 140, 1);
    fprintf(file, "@define-color text_color #%s;\n\n", color_changed);
 
    fclose(file);
}

/*
void color_print()
{   
    GdkRGBA col;
    gtk_color_chooser_get_rgba(ThemeColor,&col);
    char* hex;
    char color[] = "FFFFFF";
    
    hex = ints_to_hexas((int) (255*col.red));
    color[0] = hex[0]; color[1] = hex[1];

    hex = ints_to_hexas((int) (255*col.green));
    color[2] = hex[0]; color[3] = hex[1];

    hex = ints_to_hexas((int) (255*col.blue));
    color[4] = hex[0]; color[5] = hex[1];
    
    free(hex);


    char color_changed[] = "FFFFFF";
    printf("bg_color : #%s\n", color);

    color_update(color, color_changed);
    color_calculator(color_changed, 1.1, 10, 0);
    printf("fg_color : #%s\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 0.8, 0, 0);
    printf("dark_bg_color : #%s\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 0.9, 0, 0);
    printf("dark_fg_color : #%s\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 0.75, 0, 0);
    printf("bf_last_color : #%s\n", color_changed);

    color_update(color, color_changed);
    color_calculator(color_changed, 0.25, -10, 1);
    printf("text__color : #%s\n\n", color_changed);
}*/
