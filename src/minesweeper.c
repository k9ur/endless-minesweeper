#include "minesweeper.h"
#include "logic.h"
#include "init.h"
#include "render.h"
#include "rects.h"
#include "cell.h"
#include "grid.h"
#include "powerups.h"
#include "reset.h"
#include "destroy.h"
#include "handlers.h"


int main(void) {
	SDL_Log("Game started\n");

	Game game = {
		// .window, .renderer, .grid, .rects
		.cursor = {
			.x = 0,
			.y = 0
		},
		.clicks = 0,
		.flags = 0,
		.seen = 0,
		.over = false,
		.quit = false,
		.redraw = true // Initial draw
	};
	if(!init(&game))
		return EXIT_FAILURE;

	SDL_Event e;
	do {
		if(SDL_WaitEvent(&e))
			handle_event(&game, &e);
		if(game.redraw) {
			render_clear(&game);
			render_rects(&game); // Redraw rects after clearing
#ifdef POWERUPS
			render_pups(&game);
#endif
			render_cursor(&game); // Cursor always on top
			render_update(&game);
			game.redraw = false;
		}
	} while(!game.quit);

	destroy(&game);

	SDL_Log("Closed sucessfully\n");
	return EXIT_SUCCESS;
}

