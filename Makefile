CC := clang

CFLAGS += -std=c99
CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -pedantic

CFLAGS += -fpic

SDL_PATH = $(shell brew --prefix sdl2)

SRC = $(wildcard src/*.c)

INC  += -I$(SDL_PATH)/include
LIBS += $(shell $(SDL_PATH)/bin/sdl2-config --prefix=$(SDL_PATH) --libs)

RELOAD_TARGET = ./$(BIN)/libreload.dylib
LIB_RELOAD_PATH += -L./bin/
SHARED_RELOAD += -lreload

BIN = bin
OBJ = $(SRC:%.c=$(BIN)/%.o)
DEP = $(SRC:%.c=$(BIN)/%.d)

HOT_OBJ += $(BIN)/src/render.o
HOT_OBJ += $(BIN)/src/hotreload.o

GAME := game

-include $(DEP)

.PHONY: all

all: game

$(BIN):
	mkdir -p $@
	mkdir -p $@/src

out: $(BIN)

$(OBJ): $(BIN)/%.o: %.c
	$(CC) $(CFLAGS) -MMD $(INC) -c $< -o $@

# $(RELOAD_TARGET): src/render.c src/hotreload.c
# 	$(CC) $(CFLAGS) -shared $(INC) $(LIBS) $^ -o $@


$(RELOAD_TARGET): $(OBJ)
	$(CC) $(CFLAGS) -shared $(INC) $(LIBS) $(HOT_OBJ) -o $@

# dy: $(OBJ)
# 	$(CC) -shared $(LIBS) $^ -o bin/$(LIBGAME).dylib

game: out $(RELOAD_TARGET) $(OBJ) 
	$(CC) $(OBJ) $(LIBS) -o bin/$(GAME)

clean:
	rm -rf bin
