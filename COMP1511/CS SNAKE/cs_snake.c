// cs_snake.c
// Written by Hanneli Yu <z5142302> on <10/14/2025>
//
// Description: < a modified version of the classic Snake game where the player 
// enters commands to move the snake and eat apples on the board >

// Provided Libraries
#include <stdio.h>
// Add your own #include statements below this line
#include <ctype.h>
#include <stdlib.h>
// Provided constants
#define ROWS 10 
#define COLS 10

#define NO_SNAKE -1

// Add your own #define constants below this line
#define MAX_PORTAL 5

// Provided enums
// Enum for features on the game board
enum entity {
    SNAKE_HEAD,
    RIVAL_SNAKE_HEAD,
    BODY_SEGMENT,
    EXIT_LOCKED,
    EXIT_UNLOCKED,
    WALL,
    APPLE_NORMAL,
    APPLE_REVERSE,
    APPLE_SPLIT,
    APPLE_EXPLODE,
    EXPLOSION,
    PASSAGE_UP,
    PASSAGE_DOWN,
    PASSAGE_LEFT,
    PASSAGE_RIGHT,
    PORTAL,
    EMPTY
};

// Provided structs
// Represents a tile/cell on the game board
struct tile {
    enum entity entity;
    int segment_split;
    int portal_number;
};

// Add your own structs below this line

struct portal_pair {
    int row1;
    int col1;
    int row2;
    int col2;
    int paired;
};
// Provided Function Prototypes
void initialise_board(struct tile board[ROWS][COLS]);
void print_board(
    struct tile board[ROWS][COLS], 
    int snake_row, 
    int snake_col
);
void print_game_statistics(
    int points,
    int moves_made,
    int num_apples_eaten,
    int num_apples_remaining,
    double completion_percentage,
    int maximum_points_remaining
);
void print_game_statistics_with_rival(
    int original_points,
    int original_moves_made,
    int original_num_apples_eaten,
    int rival_points,
    int rival_moves_made,
    int rival_num_apples_eaten,
    int num_apples_remaining,
    double completion_percentage,
    int maximum_points_remaining
);
void print_board_line(void);
void print_tile_spacer(void);
void print_board_header(void);

// Add your function prototypes below this line

void setup_single_wall(struct tile board[ROWS][COLS]);
void setup_exit_locked(struct tile board[ROWS][COLS]);
void setup_apple(struct tile board[ROWS][COLS]);
void setup_long_wall_vertical(struct tile board[ROWS][COLS], int row, int col, 
int length);
void setup_long_wall_horizontal(struct tile board[ROWS][COLS], int row, int col, 
int length);
void setup_long_wall(struct tile board[ROWS][COLS]);
int basic_setup_loop(struct tile board[ROWS][COLS], 
    struct portal_pair portals[MAX_PORTAL]);
int error_check_position(struct tile board[ROWS][COLS], int row, int col);
int error_check_occupation(struct tile board[ROWS][COLS], int row, int col);
void setup_spawn_snake(struct tile board[ROWS][COLS], int *snake_row, int *snake_col);
void setup_spawn_rival_snake(struct tile board[ROWS][COLS], int *rival_row, int *rival_col);
void basic_setup_slithering(struct tile board[ROWS][COLS], 
    int snake_row, int snake_col, int *points, int *moves, int *num_eaten, 
    int num_apples, int *time, struct tile reset_board[ROWS][COLS],
    struct portal_pair portals[MAX_PORTAL]);
int snake_move (struct tile board[ROWS][COLS], 
    int *snake_row, int *snake_col, int row_change, int col_change, 
    int *points, int *moves, int *num_eaten, int num_apples, int *time,
    struct portal_pair portals[MAX_PORTAL]);
void apple_eat_exit_unlock(struct tile board[ROWS][COLS], int *snake_row, 
    int *snake_col, int *points, int *num_eaten);
void exit_unlocking(struct tile board[ROWS][COLS]);
int apple_count(struct tile board[ROWS][COLS]);
void print_statistics(struct tile board[ROWS][COLS], int points, int moves, 
    int num_eaten, int num_apples);
int s_move(struct tile board[ROWS][COLS], int *snake_row, int *snake_col, 
    int pre_row, int pre_col, int *points, int *moves, int *num_eaten, 
    int num_apples, int *time, struct portal_pair portals[MAX_PORTAL]);
int winning_losing (struct tile board[ROWS][COLS], int snake_row, int snake_col,
    int pre_row, int pre_col, int *points, int *moves, int *num_eaten, 
    int num_apples);
void snake_reverse(struct tile board[ROWS][COLS], int *snake_row, int *snake_col);
void snake_split(struct tile board[ROWS][COLS], int *snake_row, int *snake_col);
int find_tail(struct tile board[ROWS][COLS], int head_row, int head_col, 
    int *end_row, int *end_col);
void find_farthest_segment(struct tile board[ROWS][COLS], int row, int col, 
    int head_row, int head_col, int *end_row, int *end_col, int *distance);
void reset_map(struct tile board[ROWS][COLS], 
    struct tile reset_board[ROWS][COLS], int *snake_row, int *snake_col,
    int *points, int *moves, int *num_eaten, int *time);
void initialise_portals(struct portal_pair portals[MAX_PORTAL]);
int portal_paired_check(struct portal_pair portals[MAX_PORTAL]);
void setup_portal(struct tile board[ROWS][COLS], struct portal_pair portals[MAX_PORTAL]);
void portal_move (struct tile board[ROWS][COLS], int *snake_row, int *snake_col,
    int row_change, int col_change, struct portal_pair portals[MAX_PORTAL]);

// Provided sample main() function (you will need to modify this)
int main(void) {
    printf("Welcome to CS Snake!\n\n");
    //initialise board and portals
    struct tile board[ROWS][COLS];
    struct portal_pair portals[MAX_PORTAL];
    initialise_board(board);
    initialise_portals(portals);
    //setup
    int rival_play = basic_setup_loop(board, portals);
    print_board(board, NO_SNAKE, NO_SNAKE);
    //spawn snake
    int snake_row;
    int snake_col;
    setup_spawn_snake(board, &snake_row, &snake_col);
    if (rival_play == 0) {
        print_board(board, snake_row, snake_col);
    }
    int rival_row = NO_SNAKE;
    int rival_col = NO_SNAKE;
    if (rival_play == 1) {
        setup_spawn_rival_snake(board, &rival_row, &rival_col);
        print_board(board, snake_row, snake_col);
    }
    //reset map
    struct tile reset_board[ROWS][COLS];
    for (int row = 0; row < ROWS; row ++) {
        for (int col = 0; col < COLS; col ++) {
            reset_board[row][col] = board[row][col];
        }
    }
    //game play
    int points = 0;
    int moves = 0;
    int num_eaten = 0;
    int time = 0;
    basic_setup_slithering(board, snake_row, snake_col, &points, &moves, 
        &num_eaten, apple_count(board), &time, reset_board, portals);
    return 0;
}

// Add your function definitions below this line
// stage 1.1.1 - Basic Setup Loop - SINGLE WALL
void setup_single_wall(struct tile board[ROWS][COLS]) {
    int row = 0;
    int col = 0;
    scanf("%d %d", &row, &col);
    //ERROR CHECK
    if (error_check_position(board, row, col) == 1) {
        printf("ERROR: Invalid position, %d %d is out of bounds!\n", row, col);
    } else if (error_check_occupation(board, row, col) == 1) {
        printf("ERROR: Invalid tile, %d %d is occupied!\n", row, col);
        //SETUP WALL
    } else {
        board[row][col].entity = WALL;
    }
}
// stage 1.1.2 - Basic Setup Loop - EXIT_LOCKED
void setup_exit_locked(struct tile board[ROWS][COLS]) {
    int row = 0;
    int col = 0;
    scanf("%d %d", &row, &col);
    //ERROR CHECK
    if (error_check_position(board, row, col) == 1) {
        printf("ERROR: Invalid position, %d %d is out of bounds!\n", row, col);
    } else if (error_check_occupation(board, row, col) == 1) {
        printf("ERROR: Invalid tile, %d %d is occupied!\n", row, col);
        //SETUP LOCKED EXIT
    } else {
        board[row][col].entity = EXIT_LOCKED;
    }
}
// stage 1.1.3 - Basic Setup Loop - APPLE
void setup_apple(struct tile board[ROWS][COLS]) {
    int row = 0;
    int col = 0;
    char command_setup;
    scanf(" %c", &command_setup);
    //NORMAL APPLE
    if (command_setup == 'n') {
        scanf("%d %d", &row, &col);
        if (error_check_position(board, row, col) == 1) {
            printf("ERROR: Invalid position, %d %d is out of bounds!\n", 
                row, col);
        } else if (error_check_occupation(board, row, col) == 1) {
            printf("ERROR: Invalid tile, %d %d is occupied!\n", row, col);
        } else {
            board[row][col].entity = APPLE_NORMAL;
        }
        //REVERSE APPLE
    } else if (command_setup == 'r') {
        scanf("%d %d", &row, &col);
        if (error_check_position(board, row, col) == 1) {
            printf("ERROR: Invalid position, %d %d is out of bounds!\n", 
                row, col);
        } else if (error_check_occupation(board, row, col) == 1) {
            printf("ERROR: Invalid tile, %d %d is occupied!\n", row, col);
        } else {
            board[row][col].entity = APPLE_REVERSE;
        }
        //SPLIT APPLE
    } else if (command_setup == 's') {
        scanf("%d %d", &row, &col);
        if (error_check_position(board, row, col) == 1) {
            printf("ERROR: Invalid position, %d %d is out of bounds!\n", 
                row, col);
        } else if (error_check_occupation(board, row, col) == 1) {
            printf("ERROR: Invalid tile, %d %d is occupied!\n", row, col);
        } else {
            board[row][col].entity = APPLE_SPLIT;
        }
    }
}
// stage 1.1.4.1 - Basic Setup Loop - LONG WALL_vertical
void setup_long_wall_vertical(struct tile board[ROWS][COLS], int row, int col, 
int length) {
    int i = 0;
    int position_ok = 1;
    //ERROR CHECK
    while (i < length && position_ok == 1) {
        if (error_check_position(board, row + i, col) == 1) {
            printf(
                "ERROR: Invalid position, part of the wall is out of bounds!\n"
            );
            position_ok = 0;
        } 
        i ++;
    }
    if (position_ok == 1) {
        i = 0;
        while (i < length && position_ok == 1) {
            if (error_check_occupation(board, row + i, col) == 1) {
                printf("ERROR: Invalid tile, part of the wall is occupied!\n");
                position_ok = 0;
            }
            i ++;
        }
    }
    //SETUP VERTICAL LONG WALL
    if (position_ok == 1) {
        i = 0;
        while (i < length) {
            board[row + i][col].entity = WALL;
            i ++;
        }
    }
}
// stage 1.1.4.2 - Basic Setup Loop - LONG WALL_horizontal
void setup_long_wall_horizontal(struct tile board[ROWS][COLS], int row, int col, 
int length) {
    int i = 0;
    int position_ok = 1;
    //ERROR CHECK
    while (i < length && position_ok == 1) {
        if (error_check_position(board, row, col + i) == 1) {
            printf(
                "ERROR: Invalid position, part of the wall is out of bounds!\n"
            );
            position_ok = 0;
        } 
        i ++;
    }
    if (position_ok == 1) {
        i = 0;
        while (i < length && position_ok == 1) {
            if (error_check_occupation(board, row, col + i) == 1) {
                printf("ERROR: Invalid tile, part of the wall is occupied!\n");
                position_ok = 0;
            }
            i ++;
        }
    }
    //SETUP HORIZONTAL LONG WALL
    if (position_ok == 1) {
        i = 0;
        while (i < length) {
            board[row][col + i].entity = WALL;
            i ++;
        }
    }
}
// stage 1.1.4 - Basic Setup Loop - LONG WALL - VERTICAL & HORIZONTAL WALL
void setup_long_wall(struct tile board[ROWS][COLS]) {
    int row = 0;
    int col = 0;
    int length = 0;
    char command_setup;
    scanf(" %c", &command_setup);

    if (command_setup == 'v') {
        scanf("%d %d %d", &row, &col, &length);
        if (error_check_position(board, row, col) == 1) {
            printf("ERROR: Invalid position, %d %d is out of bounds!\n", 
                row, col);
        } else {
            setup_long_wall_vertical(board, row, col, length);
        }

    } else if (command_setup == 'h') {
        scanf("%d %d %d", &row, &col, &length);
        if (error_check_position(board, row, col) == 1) {
            printf("ERROR: Invalid position, %d %d is out of bounds!\n", 
                row, col);
        } else {
            setup_long_wall_horizontal(board, row, col, length);
        }  
    }
}

// Stage 3.2 - One-Way Passage
void setup_oneway_passage(struct tile board[ROWS][COLS]) {
    int row = 0;
    int col = 0;
    char command_setup;
    scanf(" %c", &command_setup);
    if (command_setup == '^') {
        scanf("%d %d", &row, &col);
        if (error_check_position(board, row, col) == 1) {
            printf("ERROR: Invalid position, %d %d is out of bounds!\n", 
                row, col);
        } else if (error_check_occupation(board, row, col) == 1) {
            printf("ERROR: Invalid tile, %d %d is occupied!\n", row, col);
        } else {
            board[row][col].entity = PASSAGE_UP;
        }                              
    } else if (command_setup == 'v') {
        scanf("%d %d", &row, &col);
        if (error_check_position(board, row, col) == 1) {
            printf("ERROR: Invalid position, %d %d is out of bounds!\n", 
                row, col);
        } else if (error_check_occupation(board, row, col) == 1) {
            printf("ERROR: Invalid tile, %d %d is occupied!\n", row, col);
        } else {
            board[row][col].entity = PASSAGE_DOWN;
        }                              
    } else if (command_setup == '<') {
        scanf("%d %d", &row, &col);
        if (error_check_position(board, row, col) == 1) {
            printf("ERROR: Invalid position, %d %d is out of bounds!\n", 
                row, col);
        } else if (error_check_occupation(board, row, col) == 1) {
            printf("ERROR: Invalid tile, %d %d is occupied!\n", row, col);
        } else {
            board[row][col].entity = PASSAGE_LEFT;
        }                              
    } else if (command_setup == '>') {
        scanf("%d %d", &row, &col);
        if (error_check_position(board, row, col) == 1) {
            printf("ERROR: Invalid position, %d %d is out of bounds!\n", 
                row, col);
        } else if (error_check_occupation(board, row, col) == 1) {
            printf("ERROR: Invalid tile, %d %d is occupied!\n", row, col);
        } else {
            board[row][col].entity = PASSAGE_RIGHT;
        }                              
    }
}

// stage 1.1 - Basic Setup Loop
int basic_setup_loop(struct tile board[ROWS][COLS], 
    struct portal_pair portals[MAX_PORTAL]) {
    printf("--- Map Setup ---\n");
    char command_setup;
    //SETTING UP
    int rival_play = 0;
    int end_setup = 0;
    while (!end_setup && scanf(" %c", &command_setup) == 1) {
        if (command_setup == 's') {
            end_setup = 1;
        } else if (command_setup == 'S') {
            rival_play = 1;
            end_setup = 1;
        } else if (command_setup == 'w') {
            setup_single_wall(board);
        } else if (command_setup == 'e') {
            setup_exit_locked(board);
        } else if (command_setup == 'a') {
            setup_apple(board);
            // Stage 1.3 - Adding Long Walls    
        } else if (command_setup == 'W') {
            setup_long_wall(board);
        } else if (command_setup == 'p') {
            setup_oneway_passage(board);
        } else if (command_setup == 't') {
            setup_portal(board, portals);
        }
    }
    return rival_play;
}

//Stage1.2 - Error Checking - POSITION
int error_check_position(struct tile board[ROWS][COLS], int row, int col) {
    if (row > 9 || row < 0 
        || col > 9 || col < 0) {
        return 1;
    } 
    return 0;
}
//Stage1.2 - Error Checking - OCCUPATION
int error_check_occupation(struct tile board[ROWS][COLS], int row, int col) {
    if (board[row][col].entity != EMPTY) {
        return 1;
    }
    return 0;
}

//Stage 1.4 - Spawn Snake
void setup_spawn_snake(struct tile board[ROWS][COLS], 
    int *snake_row, int *snake_col) {
    printf("--- Spawning Snake ---\n");
    int row_s_start;
    int col_s_start;
    int check_ok = 0;
    while (check_ok == 0) {
        printf("Enter the snake's starting position: ");
        scanf("%d %d", &row_s_start, &col_s_start);
        //ERROR CHECK
        if (error_check_position(board, row_s_start, col_s_start) == 1) {
            printf("ERROR: Invalid position, %d %d is out of bounds!\n", 
                row_s_start, col_s_start);
            check_ok = 0;
        } else if (
            error_check_occupation(board, row_s_start, col_s_start) == 1) {
            printf("ERROR: Invalid tile, %d %d is occupied!\n", 
                row_s_start, col_s_start);
            check_ok = 0;
            //OK TO PLAY - LOCATE SNAKE HEAD
        } else {
            board[row_s_start][col_s_start].entity = SNAKE_HEAD;
            *snake_row = row_s_start;
            *snake_col = col_s_start;
            check_ok = 1;
        }
    }
}
//Stage 4.2 - Ready Player 2 - Spawn Rival Snake
void setup_spawn_rival_snake(struct tile board[ROWS][COLS], 
    int *rival_row, int *rival_col) {
    printf("--- Spawning Rival Snake ---\n");
    int row_s_start;
    int col_s_start;
    int check_ok = 0;
    while (check_ok == 0) {
        printf("Enter the rival snake's starting position: ");
        scanf("%d %d", &row_s_start, &col_s_start);
        //ERROR CHECK
        if (error_check_position(board, row_s_start, col_s_start) == 1) {
            printf("ERROR: Invalid position, %d %d is out of bounds!\n", 
                row_s_start, col_s_start);
            check_ok = 0;
        } else if (
            error_check_occupation(board, row_s_start, col_s_start) == 1) {
            printf("ERROR: Invalid tile, %d %d is occupied!\n", 
                row_s_start, col_s_start);
            check_ok = 0;
            //OK TO PLAY - LOCATE RIVAL SNAKE HEAD
        } else {
            board[row_s_start][col_s_start].entity = RIVAL_SNAKE_HEAD;
            *rival_row = row_s_start;
            *rival_col = col_s_start;
            check_ok = 1;
        }
    }
}
//Stage 2.1: Basic Slithering
void basic_setup_slithering(struct tile board[ROWS][COLS], 
    int snake_row, int snake_col, int *points, int *moves, int *num_eaten, 
    int num_apples, int *time, struct tile reset_board[ROWS][COLS],
    struct portal_pair portals[MAX_PORTAL]) {
    printf("--- Gameplay Phase ---\n");
    char command_slithering;
    int quit = 0;
    while (scanf(" %c", &command_slithering) == 1 && quit == 0) {
        if (command_slithering == 'w') {
            quit = snake_move(board, &snake_row, &snake_col, -1, 0, points, 
                moves, num_eaten, num_apples, time, portals);
        } else if (command_slithering == 'a') {
            quit = snake_move(board, &snake_row, &snake_col, 0, -1, points, 
                moves, num_eaten, num_apples, time, portals);
        } else if (command_slithering == 's') {
            quit = snake_move(board, &snake_row, &snake_col, 1, 0, points, 
                moves, num_eaten, num_apples, time, portals);
        } else if (command_slithering == 'd') {
            quit = snake_move(board, &snake_row, &snake_col, 0, 1, points, 
                moves, num_eaten, num_apples, time, portals);
        } else if (command_slithering == 'p') {
            print_statistics(board, *points, *moves, *num_eaten, num_apples);
        } else if (command_slithering == 'r') {
            reset_map(board, reset_board, &snake_row, &snake_col, points, 
            moves, num_eaten, time);
        }
        if (quit == 1) {
            return;
        }
    }
    if (quit == 0) {
        printf("--- Quitting Game ---\n");
    }
}
//Stage 2.1: Basic Slithering - SNAKE MOVE
int snake_move (struct tile board[ROWS][COLS], 
    int *snake_row, int *snake_col, int row_change, int col_change, 
    int *points, int *moves, int *num_eaten, int num_apples, int *time,
    struct portal_pair portals[MAX_PORTAL]) {
    //SAVE PRE POSITION & FIND NEW POSITION
    int pre_row = *snake_row;
    int pre_col = *snake_col;
    *snake_row += row_change;
    *snake_col += col_change;
    board[pre_row][pre_col].entity = BODY_SEGMENT;
    board[pre_row][pre_col].segment_split = *time;
    (*time) ++;
    //PORTAL CHECK
    if (error_check_position(board, *snake_row, *snake_col) == 0 
    && board[*snake_row][*snake_col].entity == PORTAL) {
        portal_move(board, snake_row, snake_col, 
            row_change, col_change, portals);
    }
    //MOVE
    return s_move(board, snake_row, snake_col, pre_row, pre_col, 
        points, moves, num_eaten, num_apples, time, portals);
}

//Stage 4.2: Basic Slithering - RIVAL SNAKE MOVE
int rival_snake_move (struct tile board[ROWS][COLS], 
    int *snake_row, int *snake_col, int row_change, int col_change, 
    int *points, int *moves, int *num_eaten, int num_apples, int *time,
    struct portal_pair portals[MAX_PORTAL]) {
    //SAVE PRE POSITION & FIND NEW POSITION
    int pre_row = *rival_row;
    int pre_col = *rival_col;
    *rival_row += row_change;
    *rival_col += col_change;
    board[pre_row][pre_col].entity = BODY_SEGMENT;
    board[pre_row][pre_col].segment_split = *time;
    (*time) ++;
    //PORTAL CHECK
    if (error_check_position(board, *rival_row, *rival_col) == 0 
    && board[*rival_row][*rival_col].entity == PORTAL) {
        portal_move(board, rival_row, rival_col, 
            row_change, col_change, portals);
    }
    //MOVE
    return rival_s_move(board, snake_row, snake_col, pre_row, pre_col, 
        points, moves, num_eaten, num_apples, time, portals);
}

//Stage 2.2: Eating Apples and Unlocking Exits
void apple_eat_exit_unlock(struct tile board[ROWS][COLS], int *snake_row, 
    int *snake_col, int *points, int *num_eaten) {
    
    if (board[*snake_row][*snake_col].entity == APPLE_NORMAL) {
        board[*snake_row][*snake_col].entity = EMPTY;
        (*points) += 5;
        (*num_eaten) ++;
    }
    if (board[*snake_row][*snake_col].entity == APPLE_REVERSE) {
        board[*snake_row][*snake_col].entity = EMPTY;
        (*points) += 10;
        (*num_eaten) ++;
        snake_reverse(board, snake_row, snake_col);
    }
    if (board[*snake_row][*snake_col].entity == APPLE_SPLIT) {
        board[*snake_row][*snake_col].entity = EMPTY;
        (*points) += 20;
        (*num_eaten) ++;
        snake_split(board, snake_row, snake_col);
    }
    if (apple_count(board) == 0) {
        exit_unlocking(board);
    }
}
//Stage 2.2: Unlocking Exits
void exit_unlocking(struct tile board[ROWS][COLS]) {
    for (int row = 0; row < ROWS; row ++) {
        for (int col = 0; col < COLS; col ++) {
            if (board[row][col].entity == EXIT_LOCKED) {
                board[row][col].entity = EXIT_UNLOCKED;
            }
        }
    }
}
//Stage 2.3: Points and Statistics - APPLES REMAINING
int apple_count(struct tile board[ROWS][COLS]) {
    int apple_count = 0;
    for (int i = 0; i < ROWS; i ++) {
        for (int j = 0; j < COLS; j ++) {
            if (board[i][j].entity == APPLE_NORMAL 
                || board[i][j].entity == APPLE_REVERSE 
                || board[i][j].entity == APPLE_SPLIT) {
                apple_count ++;
            }
        }
    }
    return apple_count;
}

//Stage 2.3: Points and Statistics
void print_statistics(struct tile board[ROWS][COLS], int points, int moves, 
    int num_eaten, int num_apples) {
    //REMAINING APPLES
    int num_apples_remaining = apple_count(board);
    //COMPLETION PERCENTAGE
    double completion_percentage;
    if (num_apples == 0) {
        completion_percentage = 100.0;
    } else {
        completion_percentage = 100.0 * num_eaten / num_apples;
    }
    //MAX POINTS REMAINING
    int maximum_points_remaining = 0;
    for (int row = 0; row < ROWS; row ++) {
        for (int col = 0; col < COLS; col ++) {
            if (board[row][col].entity == APPLE_NORMAL) {
                maximum_points_remaining += 5;
            } else if (board[row][col].entity == APPLE_REVERSE) {
                maximum_points_remaining += 10;
            } else if (board[row][col].entity == APPLE_SPLIT) {
                maximum_points_remaining += 20;
            }
        }
    }
    //PRINT STATISTICS
    print_game_statistics(points, moves, num_eaten, num_apples_remaining, 
        completion_percentage, maximum_points_remaining);
}

//Stage 2.4: Winning and Losing
int winning_losing (struct tile board[ROWS][COLS], int snake_row, int snake_col,
    int pre_row, int pre_col, int *points, int *moves, int *num_eaten, 
    int num_apples) {
    (*moves)++;

    if (board[snake_row][snake_col].entity == EXIT_UNLOCKED) {
        board[snake_row][snake_col].entity = SNAKE_HEAD;
        print_board(board, snake_row, snake_col);
        printf("--- Game Over ---\nSsslithered out with a full stomach!\n");
        print_statistics(board, *points, *moves, *num_eaten, num_apples);
        return 1;
    }
    if (board[snake_row][snake_col].entity == BODY_SEGMENT 
        || board[snake_row][snake_col].entity == EXIT_LOCKED 
        || board[snake_row][snake_col].entity == WALL || 
        error_check_position(board, snake_row, snake_col) == 1 
    ) {
        board[snake_row][snake_col].entity = SNAKE_HEAD;
        print_board(board, snake_row, snake_col);
        printf("--- Game Over ---\n");
        printf("Guessss I was the prey today.\n");
        print_statistics(board, *points, *moves, *num_eaten, num_apples);
        return 1;
    }
    //ONE WAY PASSAGE
    if (board[snake_row][snake_col].entity == PASSAGE_UP 
        || board[snake_row][snake_col].entity == PASSAGE_DOWN 
        || board[snake_row][snake_col].entity == PASSAGE_RIGHT 
        || board[snake_row][snake_col].entity == PASSAGE_RIGHT) {
        int row_change = snake_row - pre_row;
        int col_change = snake_col - pre_col;
        int pass = 0;
        if ((board[snake_row][snake_col].entity == PASSAGE_UP 
            && row_change == -1 && col_change == 0) || 
            (board[snake_row][snake_col].entity == PASSAGE_DOWN 
            && row_change == 1 && col_change == 0) || 
            (board[snake_row][snake_col].entity == PASSAGE_LEFT 
            && row_change == 0 && col_change == -1) ||
            (board[snake_row][snake_col].entity == PASSAGE_RIGHT 
            && row_change == 0 && col_change == 1)) {
            pass = 1;
        }
        if (pass == 0) {
            board[snake_row][snake_col].entity = SNAKE_HEAD;
            print_board(board, snake_row, snake_col);
            printf("--- Game Over ---\n");
            printf("Guessss I was the prey today.\n");
            print_statistics(board, *points, *moves, *num_eaten, num_apples);
            return 1;
        } else {
            board[snake_row][snake_col].entity = EMPTY;
        }
    }
    return 0;
}
//Stage 2.4: SNAKE MOVE
int s_move(struct tile board[ROWS][COLS], int *snake_row, int *snake_col, 
    int pre_row, int pre_col, int *points, int *moves, int *num_eaten, 
    int num_apples, int *time, struct portal_pair portals[MAX_PORTAL]) {
    //CHECK WIN OR LOSE
    int quit = winning_losing(board, *snake_row, *snake_col, pre_row, pre_col, 
        points, moves, num_eaten, num_apples);
    //IF NOT LOSE - APPLE EAT & UPDATE BOARD
    if (quit == 0) {
        apple_eat_exit_unlock(board, snake_row, snake_col, points, num_eaten);
        board[*snake_row][*snake_col].entity = SNAKE_HEAD;
        print_board(board, *snake_row, *snake_col);
        //IF LOSE - UPDATE HEAD POSITION
    } else {
        board[*snake_row][*snake_col].entity = SNAKE_HEAD;
    }
    return quit;
}

//Stage 3.1 - Snake Reverse
void snake_reverse(struct tile board[ROWS][COLS], 
    int *snake_row, int *snake_col) {
    int head_row = *snake_row;
    int head_col = *snake_col;
    int end_row = -1;
    int end_col = -1;
    //FIND TAIL
    if (!find_tail(board, head_row, head_col, &end_row, &end_col)) {
        return;
    }
    //FIND MAX TIME FOR BODY SEGMENTS
    int max_time = -1;
    for (int row = 0; row < ROWS; row ++) {
        for (int col = 0; col < COLS; col ++) {
            if (board[row][col].entity == BODY_SEGMENT 
                && board[row][col].segment_split > max_time) {
                max_time = board[row][col].segment_split;
            }
        }
    }

    board[head_row][head_col].segment_split = max_time + 1;
    board[head_row][head_col].entity = BODY_SEGMENT;
    //REMOVE OLD BODY SEGMENTS
    board[end_row][end_col].entity = EMPTY;
    *snake_row = end_row;
    *snake_col = end_col;
    int new_maxtime = max_time + 1;
    //UPDATE TIME FOR BODY SEGMENTS
    for (int row = 0; row < ROWS; row ++) {
        for (int col = 0; col < COLS; col ++) {
            if (board[row][col].entity == BODY_SEGMENT) {
                board[row][col].segment_split 
                = new_maxtime - board[row][col].segment_split;
            }
        }
    }
}
//Stage 3.1 - Snake Reverse - FIND TAIL
int find_tail(struct tile board[ROWS][COLS], int head_row, int head_col, 
    int *end_row, int *end_col) {
    *end_row = -1;
    *end_col = -1;
    int distance = -1;
    for (int row = 0; row < ROWS; row ++) {
        for (int col = 0; col < COLS; col ++) {
            find_farthest_segment(board, row, col, head_row, head_col, end_row, 
            end_col, &distance);
        }
    }
    return (*end_row != -1);
}
//Stage 3.1 - Snake Reverse - FIND THE FARTHEST SEGMENT
void find_farthest_segment(struct tile board[ROWS][COLS], int row, int col, 
    int head_row, int head_col, int *end_row, int *end_col, int *distance) {
    if (board[row][col].entity != BODY_SEGMENT) {
        return;     
    }
    int dis = abs(row - head_row) + abs(col - head_col);
    if (dis > *distance) {
        *distance = dis;
        *end_row = row;
        *end_col = col;
    }
}

//Stage 3.3 - Snake Split
void snake_split(struct tile board[ROWS][COLS], int *snake_row, int *snake_col
) {
    int num_segment = 0;
    for (int row = 0; row < ROWS; row ++) {
        for (int col = 0; col < COLS; col ++) {
            if (board[row][col].entity == BODY_SEGMENT) {
                num_segment ++;
            }
        }
    }
    if (num_segment == 0) {
        return;
    }
    int remove_segment;
    if (num_segment % 2 == 0) {
        remove_segment = num_segment / 2;
    } else {
        remove_segment = (num_segment + 1) / 2;
    }
    //STORE BODY SEGMENT
    struct segment_info {
        int row;
        int col;
        int time;
    };
    struct segment_info segments[100];
    int n = 0;
    for (int row = 0; row < ROWS; row ++) {
        for (int col = 0; col < COLS; col ++) {
            if (board[row][col].entity == BODY_SEGMENT) {
                segments[n].row = row;
                segments[n].col = col;
                segments[n].time = board[row][col].segment_split;
                n ++;
            }
        }
    }
    //FIND SEGMENTS - OLDEST TO LATEST
    for (int i = 0; i < n - 1; i ++) {
        for (int j = 0; j < n - 1; j ++) {
            if (segments[j].time > segments[j + 1].time) {
                struct segment_info last = segments[j];
                segments[j] = segments[j + 1];
                segments[j + 1] = last;
            }
        }
    }
    //SPLIT
    for (int i = 0; i < remove_segment; i ++) {
        board[segments[i].row][segments[i].col].entity = EMPTY;
        board[segments[i].row][segments[i].col].segment_split = 0;
    }
}

//Stage 3.4 - Reset
void reset_map(struct tile board[ROWS][COLS], 
    struct tile reset_board[ROWS][COLS], int *snake_row, int *snake_col,
    int *points, int *moves, int *num_eaten, int *time) {
    printf("--- Resetting Map ---\n");
    for (int row = 0; row < ROWS; row ++) {
        for (int col = 0; col < COLS; col ++) {
            board[row][col] = reset_board[row][col];
        }
    }
    int reset = 0;
    for (int row = 0; row < ROWS && !reset; row ++) {
        for (int col = 0; col < COLS; col ++) {
            if (reset_board[row][col].entity == SNAKE_HEAD) {
                *snake_row = row;
                *snake_col = col;
                reset = 1;
            }
        }
    }
    *points = 0;
    *moves = 0;
    *num_eaten = 0;
    *time = 0;
    print_board(board, *snake_row, *snake_col);
}

//Stage 3.5 - Portals - INITIALISE PORTALS
void initialise_portals(struct portal_pair portals[MAX_PORTAL]) {
    for (int i = 0; i < MAX_PORTAL; i ++) {
        portals[i].row1 = -1;
        portals[i].col1 = -1;
        portals[i].row2 = -1;
        portals[i].col2 = -1;
        portals[i].paired = 0;
    }
}
//Stage 3.5 - Portals - CHECKING HOW MANY PAIRS
int portal_paired_check(struct portal_pair portals[MAX_PORTAL]) {
    int n = 0;
    for (int i = 0; i < MAX_PORTAL; i ++) {
        if (portals[i].paired == 1) {
            n ++;
        }
    }
    return n;
}
//Stage 3.5 - Portals - SETUP
void setup_portal(struct tile board[ROWS][COLS], 
    struct portal_pair portals[MAX_PORTAL]) {
    int row1;
    int col1;
    int row2;
    int col2;
    scanf("%d %d %d %d", &row1, &col1, &row2, &col2);
    //ERROR CHECK
    if (portal_paired_check(portals) > MAX_PORTAL) {
        printf("ERROR: Invalid placement,");
        printf(" maximum number of portal pairs already reached!\n");
        return;
    }
    if (error_check_position(board, row1, col1) == 1) {
        printf("ERROR: Invalid position for first portal in pair,");
        printf(" %d %d is out of bounds!\n", row1, col1);
        return;
    } 
    if (error_check_occupation(board, row1, col1) == 1) {
        printf("ERROR: Invalid tile for first portal in pair,");
        printf(" %d %d is occupied!\n", row1, col1);
        return;
    }
    if (error_check_position(board, row2, col2) == 1) {
        printf("ERROR: Invalid position for second portal in pair,");
        printf(" %d %d is out of bounds!\n", row2, col2);
        return;
    } 
    if (error_check_occupation(board, row2, col2) == 1) {
        if (row1 != row2 || col1 != col2) {
            printf("ERROR: Invalid tile for second portal in pair,");
            printf(" %d %d is occupied!\n", row2, col2);
            return;
        }
    }
    //LOOP PORTAL SETUP
    int portal_number = -1;
    for (int i = 0; i < MAX_PORTAL; i ++) {
        if (portals[i].paired == 0) {
            portal_number = i;
            i = MAX_PORTAL;
        }
    }
    portals[portal_number].row1 = row1;
    portals[portal_number].col1 = col1;
    portals[portal_number].row2 = row2;
    portals[portal_number].col2 = col2;
    portals[portal_number].paired = 1;
    board[row1][col1].entity = PORTAL;
    board[row1][col1].portal_number = portal_number;
    board[row2][col2].entity = PORTAL;
    board[row2][col2].portal_number = portal_number;
}
//Stage 3.5 - Portals - CHANGE POSITION
void portal_move (struct tile board[ROWS][COLS], int *snake_row, int *snake_col,
    int row_change, int col_change, struct portal_pair portals[MAX_PORTAL]) {

    int portal_number = board[*snake_row][*snake_col].portal_number;
    struct portal_pair portal = portals[portal_number];

    if (*snake_row == portal.row1 && *snake_col == portal.col1) {
        *snake_row = portal.row2 + row_change;
        *snake_col = portal.col2 + col_change;
    } else {
        *snake_row = portal.row1 + row_change;
        *snake_col = portal.col1 + col_change;
    }
}
// =============================================================================
// EDITABLE Provided Functions
// =============================================================================

// Given a 2D board array, initialise all tiles to EMPTY.
void initialise_board(struct tile board[ROWS][COLS]) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            board[row][col].entity = EMPTY;
        }
    }
}

// Prints the game board, showing the snake's head position on the board.
void print_board(
    struct tile board[ROWS][COLS], 
    int snake_row, 
    int snake_col
) {
    print_board_line();
    print_board_header();
    print_board_line();
    for (int row = 0; row < ROWS; row++) {
        print_tile_spacer();
        for (int col = 0; col < COLS; col++) {
            printf(" ");
            struct tile tile = board[row][col];
            if (tile.entity == SNAKE_HEAD) {
                printf("^~^");
            } else if (tile.entity == RIVAL_SNAKE_HEAD) {
                printf("^v^");
            } else if (tile.entity == WALL) {
                printf("|||");
            } else if (tile.entity == BODY_SEGMENT) {
                printf("###");
            } else if (tile.entity == EXIT_LOCKED) {
                printf("[X]");
            } else if (tile.entity == EXIT_UNLOCKED) {
                printf("[ ]");
            } else if (tile.entity == APPLE_NORMAL) {
                printf("(`)");
            } else if (tile.entity == APPLE_REVERSE) {
                printf("(R)");
            } else if (tile.entity == APPLE_SPLIT) {
                printf("(S)");
            } else if (tile.entity == PASSAGE_UP) {
                printf("^^^");
            } else if (tile.entity == PASSAGE_DOWN) {
                printf("vvv");
            } else if (tile.entity == PASSAGE_LEFT) {
                printf("<<<");
            } else if (tile.entity == PASSAGE_RIGHT) {
                printf(">>>");
            } else if (tile.entity == PORTAL) {
                printf("~O~");
            } else if (tile.entity == EXPLOSION) {
                printf("***");
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
    print_tile_spacer();
}


// =============================================================================
// DO NOT EDIT Provided Functions 
// =============================================================================

// Prints statistics about the game
void print_game_statistics(
    int points,
    int moves_made,
    int num_apples_eaten,
    int num_apples_remaining,
    double completion_percentage,
    int maximum_points_remaining
) {
    printf("============ Game Statistics ============\n");
    printf("Totals:\n");
    printf("  - Points: %d\n", points);
    printf("  - Moves Made: %d\n", moves_made);
    printf("  - Number of Apples Eaten: %d\n", num_apples_eaten);
    printf("Completion:\n");
    printf("  - Number of Apples Remaining: %d\n", num_apples_remaining);
    printf("  - Apple Completion Percentage: %.1f%%\n", completion_percentage);
    printf("  - Maximum Points Remaining: %d\n", maximum_points_remaining);
    printf("=========================================\n");
}

// Prints statistics about the game for both snakes when there are two players
void print_game_statistics_with_rival(
    int original_points,
    int original_moves_made,
    int original_num_apples_eaten,
    int rival_points,
    int rival_moves_made,
    int rival_num_apples_eaten,
    int num_apples_remaining,
    double completion_percentage,
    int maximum_points_remaining
) {
    printf("============ Game Statistics ============\n");
    printf("Original Snake Totals:\n");
    printf("  - Points: %d\n", original_points);
    printf("  - Moves Made: %d\n", original_moves_made);
    printf("  - Number of Apples Eaten: %d\n", original_num_apples_eaten);
    printf("Rival Snake Totals:\n");
    printf("  - Points: %d\n", rival_points);
    printf("  - Moves Made: %d\n", rival_moves_made);
    printf("  - Number of Apples Eaten: %d\n", rival_num_apples_eaten);
    printf("Completion:\n");
    printf("  - Number of Apples Remaining: %d\n", num_apples_remaining);
    printf("  - Apple Completion Percentage: %.1f%%\n", completion_percentage);
    printf("  - Maximum Points Remaining: %d\n", maximum_points_remaining);
    printf("=========================================\n");
}

// Helper function for print_board().
void print_board_header(void) {
    printf("|            C S _ S N A K E            |\n");
}

// Helper function for print_board(). 
void print_board_line(void) {
    printf("+");
    for (int col = 0; col < COLS; col++) {
        printf("---+");
    }
    printf("\n");
}

// Helper function for print_board(). 
void print_tile_spacer(void) {
    printf("+");
    for (int col = 0; col < COLS; col++) {
        printf("   +");
    }
    printf("\n");
}

