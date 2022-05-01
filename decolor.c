#include "decolor.h"

//Colors for print
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

void help()
{
    printf("%s", KGRN);
    printf("#-------------------------------#\n");
    printf("|             *HELP*            |\n");
    printf("#-------------------------------#\n\n");
    printf("%s", KWHT);
}

int main(int argc,char *argv[])
{
    if(argc > 1)
    {
        if(strcmp(argv[1], "--help") == 0)
        {
            help();
        }
    }
    else
        create_window_decolor(argc,argv);

    return 0;
}
