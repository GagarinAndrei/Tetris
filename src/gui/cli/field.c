#include "field.h"
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

Tetris_figures *create_figures(int count, int figure_size,
                               Tetris_block *figure_templates) {
  Tetris_figures *figures = (Tetris_figures *)malloc(sizeof(Tetris_figures));
  figures->count = count;
  figures->size = figure_size;
  figures->blocks = figure_templates;
  return figures;
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
