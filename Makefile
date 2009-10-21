CFLAGS=-Wall -pedantic --std=c99 `pkg-config --cflags --libs sdl` -lSDL_image -lSDL_ttf -lSDL_mixer

mati_mario: main.c menu.c
	$(CC) $(CFLAGS) -o $@ $^

