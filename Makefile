PLATFORM_OS ?= WINDOWS

UNAMEOS = $(shell uname)
ifeq ($(UNAMEOS),Linux)
	PLATFORM_OS = LINUX
endif
ifeq ($(UNAMEOS),FreeBSD)
	PLATFORM_OS = BSD
endif
ifeq ($(UNAMEOS),OpenBSD)
	PLATFORM_OS = BSD
endif
ifeq ($(UNAMEOS),NetBSD)
	PLATFORM_OS = BSD
endif
ifeq ($(UNAMEOS),DragonFly)
	PLATFORM_OS = BSD
endif
ifeq ($(UNAMEOS),Darwin)
	PLATFORM_OS = OSX
endif
ifndef PLATFORM_SHELL
	PLATFORM_SHELL = sh
endif

CC=gcc
CFLAGS=-Wall -std=c99 -Werror -D_DEFAULT_SOURCE

LIBS_DIR=libs
INC=-I/usr/local/include/ -I/src/ -I./libs/

ifeq ($(PLATFORM_OS), OSX)
	LIB=-L/usr/local/lib/ -lraylib -lm  -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
	LIB=-L/usr/local/lib/ -lraylib -lm
endif


SRC=./src

FILES= \
	$(wildcard src/*.c) \

OBJECTS=$(FILES:.c=.o)

APP=./build/engine

.PHONY: run build clean

run: build
	$(APP)

build: $(OBJECTS)
	$(CC) -o $(APP) $^ $(CFLAGS) $(INC) $(LIB)

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< -g -MMD

clean:
	rm $(SRC)/*.o
	rm $(SRC)/*.d
	rm $(APP)
