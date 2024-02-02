# endless-minesweeper
Infinitely generating minimalistic Minesweeper. 
Also has an optional powerups mode to make the game less boring

Colours have similar meanings to in the original Minesweeper:
- Blue = 1
- Green = 2
- Orange = 3
- Purple = 4
- Brown = 5
- Cyan = 6
- Yellow = 7
- Grey = 8

## Controls
Arrow keys, WASD, or numpad 2,4,6,8 to move around

Numpad 1,3,7,9 can be used to move diagonally

Space, period, c, or numpad 5 to click

F, comma, or numpad 0 to flag

H to use a hint (if powerups are enabled)

## Compilation
To compile, use:
```
gcc --std=c17 -O2 -s -DNDEBUG -lSDL2 -lSDL2_image src/minesweeper.c -o minesweeper
```

