CC = gcc

CPPFLAGS = `pkg-config gtk+-3.0 --cflags sdl` -MMD
CFLAGS = -Werror -Wextra -std=c99 -g
LDLIBS = `pkg-config gtk+-3.0 --libs sdl` -lSDL_image -lSDL_gfx -lm

SRC = decolor.c GTK/gtk.c SDL/tools.c SDL/pixel_operations.c SDL/DevTools/queue.c SDL/DevTools/shared_queue.c SDL/shape.c SDL/DevTools/shared_stack.c SDL/DevTools/stack.c SDL/filter.c

OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: decolor

decolor: $(OBJ)

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} decolor
	${RM} ./*.o
	${RM} ./GTK/*.o
	${RM} ./SDL/*.o
	${RM} ./SDL/DevTools/*.o
