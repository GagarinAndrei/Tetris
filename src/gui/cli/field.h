#ifndef FIELD_H
#define FIELD_H
#include "../../brick_game/tetris/tetris.h"
#include <stdlib.h>

Tetris_field *create_field(int width, int height);
Tetris_figures *create_figures(int count, int figure_size,
                               Tetris_block *figure_templates);
void free_figures(Tetris_figures *figures);
void free_field(Tetris_field *field);
void print_game(Tetris_game *game);



#endif