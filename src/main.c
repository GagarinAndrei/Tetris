#include "brick_game/tetris/tetris.h"
#include "gui/cli/field.h"
#include "gui/cli/templates.h"
#include <curses.h>
#include <stdbool.h>
#include <time.h>

Tetris_block templates[] = {
    0,0,0,0,0,
    0,0,1,0,0,
    0,1,1,1,0,
    0,0,0,0,0,
    0,0,0,0,0,

    0,0,0,0,0,
    0,0,1,1,0,
    0,0,1,0,0,
    0,0,1,0,0,
    0,0,0,0,0,
    
    0,0,1,0,0,
    0,0,1,0,0,
    0,0,1,0,0,
    0,0,1,0,0,
    0,0,0,0,0,

    0,0,0,0,0,
    0,0,1,1,0,
    0,0,1,1,0,
    0,0,0,0,0,
    0,0,0,0,0,

    0,0,0,0,0,
    0,1,1,0,0,
    0,0,1,1,0,
    0,0,0,0,0,
    0,0,0,0,0,
};

int main() {
    Tetris_game *game = create_game(10, 20, 5, 5, templates);
    Tetris_player player;
    struct timespec sp_start, sp_end, ts1, ts2 = {0, 0};

    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    cbreak();
    noecho();
    nodelay(stdscr, true);
    scrollok(stdscr, true);


    player.action = PLAYER_INACTION;
    game->player = &player;
    drop_new_figure(game);
    while (game->playing != GAMEOVER) {
        clock_gettime(CLOCK_MONOTONIC, &sp_start);

        int ch = getch();
        switch (ch) {
            case 'w':
                player.action = PLAYER_UP;
                break;
            case 's':
                player.action = PLAYER_DOWN;
                break;
            case 'a':
                player.action = PLAYER_LEFT;
                break;
            case 'd':
                player.action = PLAYER_RIGHT;
                break;
            default:
                player.action = PLAYER_INACTION;
                break;
        }

        calculate_game_act(game);
        print_game(game);
        clock_gettime(CLOCK_MONOTONIC, &sp_end);
        if (sp_end.tv_sec - sp_start.tv_sec <= 0 && (ts2.tv_nsec = 33000000 - (sp_end.tv_nsec - sp_start.tv_nsec)) > 0) {
            nanosleep(&ts2, &ts1);
        }
    }
    free_game(game);

    endwin();
    return 0; }