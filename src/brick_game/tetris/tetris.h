#ifndef TETRIS_H
#define TETRIS_H

#define TICKS_START 30

enum { GAMEOVER, PLAYING };

enum { PLAYER_INACTION, PLAYER_UP, PLAYER_DOWN, PLAYER_LEFT, PLAYER_RIGHT };

typedef struct Tetris_block {
  int is_block_on_field;
} Tetris_block;

typedef struct Tetris_player {
  int action;
} Tetris_player;

typedef struct Tetris_figure {
  int x;
  int y;
  int size;
  Tetris_block *blocks;
} Tetris_figure;

typedef struct Tetris_figures {
  int count;
  int size;
  Tetris_block *blocks;
} Tetris_figures;

typedef struct Tetris_field {
  int width;
  int height;
  Tetris_block *blocks;
} Tetris_field;

typedef struct Tetris_game {
  Tetris_field *field;
  Tetris_figure *figure;
  Tetris_figures *figures;
  Tetris_player *player;
  int ticks;
  int ticks_left;
  int playing;
  int score;
} Tetris_game;

Tetris_figure *create_figure(Tetris_game *game);
Tetris_game *create_game(int field_width, int field_height, int figures_size,
                         int count, Tetris_block *figure_templates);
Tetris_figure *rotate(Tetris_game *game);
void calculate_game_act(Tetris_game *game);
void move_down(Tetris_game *game);
void move_up(Tetris_game *game);
void move_left(Tetris_game *game);
void move_right(Tetris_game *game);
int collision(Tetris_game *game);
void plant_figure(Tetris_game *game);
int erase_lines(Tetris_game *game);
int is_line_filed(int line, Tetris_field *field);
void drop_new_figure(Tetris_game *game);
void free_figure(Tetris_figure *figure);
void free_game(Tetris_game *game);

#endif