#include "field.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

Tetris_field *create_field(int width, int height) {
  Tetris_field *field = (Tetris_field *)malloc(sizeof(Tetris_field));
  field->width = width;
  field->height = height;
  field->blocks = (Tetris_block *)malloc(sizeof(Tetris_block) * width * height);
  for (int i = 0; i < width * height; ++i)
    field->blocks->is_block_on_field = 0;
  return field;
}

Tetris_figures *create_figures(int count, int figures_size,
                               Tetris_block *figure_templates) {
  Tetris_figures *figures = (Tetris_figures *)malloc(sizeof(Tetris_figures));
  figures->count = count;
  figures->size = figures_size;
  figures->blocks = figure_templates;
  return figures;
}

void print_game(Tetris_game *game) {
  Tetris_field *field = game->field;
  Tetris_figure *figure = game->figure;
  for (int i = 0; i < field->height; ++i) {
    for (int ii = 0; ii < field->width; ++ii) {
      int pixel = 1;
      if (field->blocks[i * field->width + ii].is_block_on_field != 0) {
        pixel = 2;
      } else {
        int x = ii - figure->x;
        int y = i - figure->y;
        if (x >= 0 && x < figure->size && y >= 0 && y < figure->size) {
          if (figure->blocks[y * figure->size + x].is_block_on_field != 0) {
            pixel = 2;
          }
        }
      }
      attron(COLOR_PAIR(pixel));
      mvaddch(i, ii, ' ');
      attroff(COLOR_PAIR(pixel));
    }
  printf("\n");
  }
}

void free_figures(Tetris_figures *figures) {
  if (figures)
    free(figures);
}

void free_field(Tetris_field *field) {
  if (field) {
    if (field->blocks) {
      free(field->blocks);
    }
    free(field);
  }
}
