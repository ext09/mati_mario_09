CFLAGS=-Wall -pedantic --std=c99 `pkg-config --cflags --libs sdl` -lSDL_image -lSDL_ttf

mati_mario: main.c
	$(CC) $(CFLAGS) -o $@ $^

