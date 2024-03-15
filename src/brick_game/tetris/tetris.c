#include "tetris.h"
#include "../../gui/cli/field.h"
#include <malloc/_malloc.h>
#include <stdlib.h>

Tetris_game *create_game(int field_width, int field_height, int figures_size,
                         int count, Tetris_block *figure_templates) {

  Tetris_game *game = (Tetris_game *)malloc(sizeof(Tetris_game));
  game->field = create_field(field_width, field_height);
  game->figures = create_figures(count, figures_size, figure_templates);
  game->ticks = TICKS_START;
  game->ticks_left = TICKS_START;
  game->score = 0;
  game->playing = PLAYING;
  return game;
}

void calculate_game_act(Tetris_game *game) {
  if (game->ticks_left <= 0) {
    game->ticks_left = game->ticks;
    move_down(game);
    if (collision(game)) {
      move_up(game);
      plant_figure(game);
      game->score += erase_lines(game);
      drop_new_figure(game);
      if (collision(game)) {
        game->playing = GAMEOVER;
      }
    }
  }

  switch (game->player->action) {
  case PLAYER_UP:
    Tetris_figure *temp_figure = rotate(game);
    Tetris_figure *prevous_figure = game->figure;
    game->figure = temp_figure;
    if (collision(game)) {
      game->figure = prevous_figure;
      free_figure(temp_figure);
    } else {
      free_figure(prevous_figure);
    }
    break;
  case PLAYER_RIGHT:
    move_right(game);
    if (collision(game))
      move_left(game);
    break;
  case PLAYER_LEFT:
    move_left(game);
    if (collision(game))
      move_right(game);
    break;
  case PLAYER_DOWN:
    move_down(game);
    if (collision(game))
      move_up(game);
    break;
  case PLAYER_INACTION:
  default:
    break;
  }
  game->ticks_left--;
}

void free_game(Tetris_game *game) {
  if (game) {
    free_field(game->field);
    free_figures(game->figures);
    free(game);
  }
}

void free_figure(Tetris_figure *figure) {
  if (figure) {
    if (figure->blocks) {
      free(figure->blocks);
    }
    free(figure);
  }
}

void move_down(Tetris_game *game) { game->figure->y++; }

void move_up(Tetris_game *game) { game->figure->y--; }

void move_right(Tetris_game *game) { game->figure->x++; }

void move_left(Tetris_game *game) { game->figure->x--; }

int collision(Tetris_game *game) {
  int result = 0;
  Tetris_figure *figure = game->figure;
  Tetris_field *field = game->field;
  for (int i = 0; i < figure->size; ++i) {
    for (int ii = 0; ii < figure->size; ++ii) {
      if (figure->blocks[i * figure->size + ii].is_block_on_field != 0) {
        int x = figure->x + ii;
        int y = figure->y + i;
        if (field->blocks[y * field->width + x].is_block_on_field != 0)
          result = 1;
        if (x < 0 || x >= field->width || y < 0 || y >= field->height)
          result = 1;
      }
    }
  }
  return result;
}

void plant_figure(Tetris_game *game) {
  Tetris_figure *figure = game->figure;
  for (int i = 0; i < figure->size; ++i) {
    for (int ii = 0; ii < figure->size; ++ii) {
      if (figure->blocks[i * figure->size + ii].is_block_on_field != 0) {
        int x = figure->x + ii;
        int y = figure->y + i;
        game->field->blocks[y * game->field->width + x].is_block_on_field =
            figure->blocks[i * figure->size + ii].is_block_on_field;
      }
    }
  }
}

int is_line_filed(int line, Tetris_field *field) {
  int result = 1;
  for (int i = 0; i < field->width; ++i) {
    if (field->blocks[line * field->width + i].is_block_on_field == 0)
      result = 0;
  }
  return result;
}

void drop_line(int line, Tetris_field *field) {
  if (line == 0) {
    for (int i = 0; i < field->width; ++i) {
      field->blocks[i].is_block_on_field = 0;
    }
  } else {
    for (int i = line; i > 0; --i) {
      for (int j = 0; j < field->width; ++j) {
        field->blocks[i * field->width + j].is_block_on_field =
            field->blocks[(j - 1) * field->width + j].is_block_on_field;
      }
    }
  }
}

int erase_lines(Tetris_game *game) {
  Tetris_field *field = game->field;
  int count;
  for (int i = field->height - 1; i >= 0; --i) {
    while (is_line_filed(i, field)) {
      drop_line(i, field);
      count++;
    }
  }
  return count;
}

void drop_new_figure(Tetris_game *game) {
  Tetris_figure *figure = create_figure(game);
  figure->x = game->field->width / 2 - figure->size / 2;
  figure->y = 0;
  int figures_number = rand() % game->figures->count;
  for (int i = 0; i < figure->size; ++i) {
    for (int j = 0; j < figure->size; ++j) {
      figure->blocks[i * figure->size].is_block_on_field =
          game->figures
              ->blocks[figures_number * figure->size * figure->size +
                       i * figure->size + j]
              .is_block_on_field;
    }
  }
  free_figure(game->figure);
  game->figure = figure;
}

Tetris_figure *create_figure(Tetris_game *game) {
  Tetris_figure *figure = (Tetris_figure *)malloc(sizeof(Tetris_figure));
  figure->x = 0;
  figure->y = 0;
  figure->size = game->figures->size;
  figure->blocks = (Tetris_block *)malloc(sizeof(Tetris_block) * figure->size *
                                          figure->size);
  return figure;
}

Tetris_figure *rotate(Tetris_game *game) {
  Tetris_figure *figure = create_figure(game);
  Tetris_figure *prevous_figure = game->figure;
  figure->x = prevous_figure->x;
  figure->y = prevous_figure->y;
  for (int i = 0; i < figure->size; ++i) {
    for (int j = 0; j < figure->size; ++j) {
      figure->blocks[i * figure->size + j].is_block_on_field =
          prevous_figure->blocks[j * figure->size + figure->size - 1 - i]
              .is_block_on_field;
    }
  }
  return figure;
}