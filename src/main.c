#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "utils.h"
#include "hotreload.h"
#include "render.h"

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

typedef struct
{
	SDL_Window *window;
	SDL_Renderer *renderer;

	bool is_quit;
}
game_t;

/* HOTRELOAD section */
typedef struct 
{
	const char *lib_path;
	void *library;
	update_t *hot_update;
}
hotfunc_t;

void reload(hotfunc_t *hot)
{
	if (hot->library != NULL)
		dlclose(hot->library);

	hot->library = dlopen(hot->lib_path, RTLD_NOW);
	ASSERT(hot->library, "Error: Couldn't open %s: %s\n",
		hot->lib_path, dlerror());

	hot->hot_update = (update_t *) dlsym(hot->library, "reload_update");
	ASSERT(hot->hot_update, "Error: Couldn't find hot_update symbol.\n");
}

/* -------------------- */
void shutdown(game_t *state)
{
	SDL_DestroyWindow(state->window);
	SDL_DestroyRenderer(state->renderer);
	SDL_Quit();
}

void initialize(game_t *state)
{
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	LOG_INFO("SDL2 compiled version: %u.%u.%u\n",
		compiled.major, compiled.minor, compiled.patch);
	LOG_INFO("SDL2 linked version: %u.%u.%u\n",
		linked.major, linked.minor, linked.patch);

	ASSERT(!SDL_Init(SDL_INIT_VIDEO),
		"Error: Couldn't initialize video subsystem: %s\n",
		SDL_GetError());

	uint32_t flags =
		SDL_WINDOW_OPENGL |
		SDL_WINDOW_RESIZABLE;

	state->window = SDL_CreateWindow(
		"window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		flags);

	ASSERT(state->window,
		"Error: Couldn't create a window: %s\n",
	   	SDL_GetError());

	state->renderer = SDL_CreateRenderer(
		state->window, -1,
		SDL_RENDERER_PRESENTVSYNC | 
		SDL_RENDERER_ACCELERATED);

	ASSERT(state->renderer,
		"Error: Couldn't create a renderer: %s\n",
	   	SDL_GetError());
}

void process(game_t *state, SDL_Event *event)
{
	switch (event->type)
	{
		case SDL_QUIT:
			state->is_quit = true;
			break;
	}
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	game_t game =
	{
		.window = NULL,
		.renderer = NULL,
		.is_quit = false
	};
	game_t *const state = &game;

	hotfunc_t hotfunc = 
	{
		.lib_path = "bin/libreload.dylib",
		.library = NULL,
		.hot_update = NULL
	};

	hotfunc_t *const hot = &hotfunc;

	SDL_Event event;

	initialize(state);
	while (!state->is_quit)
	{
		while (SDL_PollEvent(&event))
		{
			process(state, &event);
		}

		reload(hot);
		hot->hot_update(state->renderer);
	}
	
	if (hot->library != NULL)
		dlclose(hot->library);

	shutdown(state);
	LOG_INFO("Game shutdown!\n");
	return 0;
}
