CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c99 -O1 -g -fsanitize=address $(shell pkg-config --cflags gtk+-3.0 sdl2 SDL2_image) -D__NO_INLINE___
LDFLAGS = 
LDLIBS = -lm $(shell pkg-config --libs gtk+-3.0 sdl2 SDL2_image)

SRC = SDL/main.c  SDL/pixel_operations.c SDL/SDLFunction.c GTK/gtk.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}
	${CC} ${CFLAGS} $^ -o $@ ${LDLIBS}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main


-include ${DEP}
