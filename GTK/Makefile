CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -Werror -std=c99 $$(pkg-config --cflags gtk+-3.0)
LDFLAGS=
LDLIBS=-lm $$(pkg-config --libs gtk+-3.0) -lm `sdl2-config --cflags --libs`
EXEC= main

SRC= gtk.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: ${EXEC}



${EXEC}: ${OBJ}

	${CC} ${LDLIBS} ${LDFLAGS} -o $@ $^ 



clean:
	rm -f ${OBJ} ${DEP} ${EXEC} main
