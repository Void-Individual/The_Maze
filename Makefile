SDL_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL_LIBS := $(shell pkg-config --libs sdl2)
INCLUDES := -I/usr/include -I./src

CFLAGS := $(SDL_CFLAGS) $(INCLUDES) -Wall -Werror -Wextra -ggdb
LIBS := $(SDL_LIBS) -lm

FILES := $(shell find src/ -name *.c)

OUT_DIR := build/
OUT_BIN := raycast

all: compile run

compile:
	mkdir -p $(OUT_DIR)
	gcc $(FILES) $(CFLAGS) $(LIBS) -o $(OUT_DIR)$(OUT_BIN)

run:
	./$(OUT_DIR)$(OUT_BIN)
