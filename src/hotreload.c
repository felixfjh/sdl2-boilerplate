#include "hotreload.h"
#include "render.h"
#include "utils.h"

void reload_update(SDL_Renderer *renderer)
{
	set_color(renderer, &COLOR_GRAY);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

}
