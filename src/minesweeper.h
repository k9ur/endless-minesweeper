#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


//#define DRAW_PALETTE // Used for testing
//#define SAFE_MODE // Can't die
#define POWERUPS // Enable powerups

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define CELL_WIDTH 16
#define CELL_HEIGHT 16
#define CURSOR_LEN 6
#define CURSOR_WIDTH 2
#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define STARTING_WIDTH 4
#define STARTING_HEIGHT 4
#define MINE_DENSITY 0.28f
#define NUM_COLORS 9
#define RECTS_INIT 16
#define JUMP_DIST 3
#ifdef POWERUPS
#define NUM_PUPS 3
#define PUP_INIT 4
#define PUP_HINT 0
#define PUP_SAVE 1
#define PUP_SPACE 2
#define PUP_FIRST_NUM 5
#define SCREEN_PADDING_X 14
#define SCREEN_PADDING_Y 10
#define PUP_WIDTH 40
#define PUP_HEIGHT 40
#define PUP_SPACING_X 16
#define PUP_SPACING_Y 2
#endif

#define cursor_start_x ((SCREEN_WIDTH - CELL_WIDTH) >> 1)
#define cursor_start_y1 ((SCREEN_HEIGHT - CELL_HEIGHT) >> 1)
#define cursor_start_y2 (cursor_start_y1 + CURSOR_WIDTH)
#define cursor_end_x1 (cursor_start_x + CELL_WIDTH - CURSOR_LEN)
#define cursor_end_x2 (cursor_start_x + CELL_WIDTH - CURSOR_WIDTH)
#define cursor_end_y1 (cursor_start_y1 + CELL_HEIGHT - CURSOR_WIDTH)
#define cursor_end_y2 (cursor_start_y1 + CELL_HEIGHT - CURSOR_LEN)


typedef uint8_t Cell;
typedef Cell CellCache;
// 0 if not yet generated
// Bit 1 describes whether it's a mine
// Bit 2 describes whether it's clicked
// Bit 3 describes whether it's flagged
// Bit 4 describes whether the surrounding mines haven't been cached
// Bits 5-8 describe the number of mines around it (0-8)
// If surroundings are cached, should be clicked soon
// If surroundings are cached to 0 before being clicked, may result in CELL_NULL, so instead 9 is used instead of 0
// Therefore it'll only be 0 if not yet generated
#define CELL_NULL 0
#define CELL_SURR_NONE 9
#define CELL_MINE (1 << 0)
#define CELL_CLICKED (1 << 1)
#define CELL_FLAGGED (1 << 2)
#define CELL_UNCACHED (1 << 3)
#define CELL_BITS (CELL_CLICKED | CELL_MINE | CELL_FLAGGED | CELL_UNCACHED)
#ifdef POWERUPS
#define CELL_PERMAFLAG (CELL_FLAGGED | CELL_CLICKED) // To permanently flag (used by hints & saves)
#endif

typedef Cell Chunk[CHUNK_HEIGHT][CHUNK_WIDTH]; // Rectangle sized chunk of cells

typedef struct {
	int32_t x, y;
} Position;

typedef enum {
	UP, DOWN, LEFT, RIGHT
} Direction;

typedef struct { // Stores the +/+, +/-, -/+, -/- quadrant chunks, and the size of each quadrant
	Chunk **pypx, **pynx, **nypx, **nynx;
	uint32_t pypx_x, pynx_x, nypx_x, nynx_x, pypx_y, pynx_y, nypx_y, nynx_y;
} Grid;

typedef struct { // Stores visible rects (on screen)
	SDL_Rect mines[4];
	SDL_Rect *nums[NUM_COLORS + 1]; // Last is flags
	size_t nums_size[NUM_COLORS + 1];
	size_t nums_used[NUM_COLORS + 1];
	uint8_t mines_used : 3; // Max 4
} Rects;

#ifdef POWERUPS
typedef struct {
	SDL_Texture *restrict texts[NUM_PUPS]; // hint, save, space
	SDL_Rect *text_rects[NUM_PUPS];
	size_t pups_size[NUM_PUPS];
	size_t pups_used[NUM_PUPS];
} Powerups;
#endif

typedef struct { // Stores the grid, cursor position, cells
	SDL_Window *restrict window;
	SDL_Renderer *restrict renderer;
	Grid grid;
#ifdef POWERUPS
	Powerups pups;
#endif
	uint64_t clicks, flags, seen;
	Position cursor;
	Rects rects;
	bool over; // Whether a mine has been clicked
	bool quit; // Whether the game has been exited
	bool redraw;
} Game;


const SDL_Color WHITE = {
	.r = 255,
	.g = 255,
	.b = 255,
	.a = 255
};
const SDL_Color BLACK = {
	.r = 0,
	.g = 0,
	.b = 0,
	.a = 255
};
const SDL_Color COLOR_CURSOR = {
	.r = 175,
	.g = 175,
	.b = 175,
	.a = 255
};
const SDL_Color COLOR_MINE = {
	.r = 255,
	.g = 0,
	.b = 0,
	.a = 255
};
const SDL_Color COLOR_NUM[NUM_COLORS + 1] = {
	{ .r = 50,  .g = 50,  .b = 50,  .a = 255 },
	{ .r = 40,  .g = 40,  .b = 255, .a = 255 },
	{ .r = 0,   .g = 135, .b = 0,   .a = 255 },
	{ .r = 255, .g = 135, .b = 0,   .a = 255 },
	{ .r = 70,  .g = 0,   .b = 135, .a = 255 },
	{ .r = 135, .g = 0,   .b = 0,   .a = 255 },
	{ .r = 0,   .g = 135, .b = 135, .a = 255 },
	{ .r = 200, .g = 180, .b = 0,   .a = 255 },
	{ .r = 135, .g = 135, .b = 135, .a = 255 },
	WHITE // flag colour
};
const SDL_Color COLOR_BG = BLACK;
#ifdef POWERUPS
const SDL_Color *COLOR_PUP = &COLOR_NUM[PUP_FIRST_NUM];

const float PUP_CHANCES[NUM_PUPS] = { 0.25f, 0.75f, 1.0f };
#endif


bool rect_cmp(const SDL_Rect *const, const SDL_Rect *const);

bool init(Game *const);
bool rand_init(void);
bool game_init(Game *const);
bool grid_init_quarter(Chunk ***const, uint32_t *const restrict, uint32_t *const restrict);
bool grid_init(Game *const);
bool rects_init(Game *const);

void render_update(Game *const);
void render_clear(Game *const);
void render_cursor(Game *const);
void render_rects(Game *const);

void game_over(Game *const, const Position *const);
void rects_add(Game *const, const Position *const, const CellCache);
void rects_rem(Game *const, const Position *const, const CellCache);
void rects_move_all(Game *const, const Direction, const bool);

CellCache cell_calc_cache(Game *const, const Position *const);
CellCache cell_get_cache(const Cell *const);
void cell_set_cache(Cell *const, const CellCache);

Chunk *grid_get_quarter_chunk(Chunk ***const, uint32_t *const restrict, uint32_t *const restrict, const Position *const);
Chunk *grid_get_chunk(Game *const, const Position *const);
Cell *grid_get(Game *const, const Position *const);
Cell *grid_set(Game *const, const Position *const, const Cell);
Cell *grid_get_and_gen(Game *const, const Position *const);
void grid_gen(Game *const, const Position *const);
void grid_click_num(Game *const, const Position *const);
void grid_click(Game *const, const Position *const);
void grid_flag(Game *const, const Position *const);
void grid_move(Game *const, const Direction, const bool);

#ifdef POWERUPS
bool pups_init(Game *const);
void render_pups(Game *const);
void pups_handle(Game *const, const CellCache);
bool pups_use_hint(Game *const, const Position *const);
bool pups_use_save(Game *const, const Position *const);
bool pups_use_space(Game *const, const Position *const);
void pups_reset(Game *const);
void pups_destroy(Game *const);
#endif

void reset(Game *const);
void game_reset(Game *const);
void grid_reset_quarter(Chunk *const*const, const uint32_t, const uint32_t);
void grid_reset(Game *const);
void rects_reset(Game *const);

void destroy(Game *const);
void game_destroy(Game *const);
void grid_destroy_quarter(Chunk ***const, const uint32_t);
void grid_destroy(Game *const);
void rects_destroy(Game *const);

void handle_event(Game *const, const SDL_Event *const);
void handle_keydown_event(Game *const, const SDL_KeyboardEvent *const);
void handle_window_event(Game *const, const SDL_WindowEvent *const);


#endif // MINESWEEPER_H

