# Simple SDL mini Makefile

CC=gcc -fsanitize=address

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -lm
LDFLAGS=
LDLIBS= -lm `pkg-config --libs sdl SDL_image SDL_gfx`

all: main

main: pixel_operations.o filter.o tools.o DevTools/shared_queue.o DevTools/queue.o DevTools/shared_stack.o DevTools/stack.o shape.o layer.o main.o

main.o: pixel_operations.h filter.h tools.h DevTools/shared_queue.h DevTools/queue.h DevTools/shared_stack.h DevTools/stack.h shape.h layer.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} DevTools/*.o
	${RM} DevTools/*.d
	${RM} main

# END
