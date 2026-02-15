// Assignment 2 25T3 COMP(1511|1911): CS Festival
// cs_festival.h
//
// This program was written by <Hanneli Yu> 
// on <11/14/2025>
//
// <A comprehensive festival planning system>

///////////////////////////////////////////////////////////////////////////////
// Provided Constants
///////////////////////////////////////////////////////////////////////////////

#define MAX_SIZE 100

///////////////////////////////////////////////////////////////////////////////
// Your Constants
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// Provided Enums
///////////////////////////////////////////////////////////////////////////////

enum genre {
    ROCK,
    POP,
    COUNTRY,
    JAZZ,
    METAL,
    HIPHOP,
    INVALID_GENRE
};

///////////////////////////////////////////////////////////////////////////////
// Provided Structs
///////////////////////////////////////////////////////////////////////////////

struct festival {
    // The name of your festival
    char name[MAX_SIZE];
    // The list of stages in the festival
    struct stage *stages;
};

struct stage {
    // The name of the stage
    char name[MAX_SIZE]; 
    // The list of performances on the stage
    struct performance *performances;
    // The next stage in the festival
    struct stage *next;
};

struct performance {
    // The name of the band performing
    char band_name[MAX_SIZE];
    // The start time of the performance 
    // (in minutes from the start of the festival)
    int start_time; 
    // The end time of the performance 
    // (in minutes from the start of the festival)
    int end_time;
    // The maximum duration allowed for the performance (in minutes)
    int max_duration; 
    // The genre of the performance
    enum genre genre; 
    // The setlist of songs for the performance
    struct song *setlist;
    // The next performance in the list
    struct performance *next;
};

struct song {
    // The title of the song
    char title[MAX_SIZE];
    // The duration of the song (in minutes)
    double duration; 
    // The popularity rating of the song (1-10)
    int popularity; 
    // The next song in the setlist
    struct song *next;
};
   
///////////////////////////////////////////////////////////////////////////////
// Provided Function Prototypes
///////////////////////////////////////////////////////////////////////////////

void print_welcome_banner();
void scan_name(char name[MAX_SIZE]);
enum genre scan_genre();
void print_help(void);
void print_festival(struct festival *festival);
void print_performance_header(struct performance *performance);
void print_stage_title(struct stage *stage);
void print_setlist(struct song *setlist);
void print_performance(struct performance *performance);
void print_song(struct song *song, int song_num);
void print_conflict_group_header(int conflict_time);
void print_conflict_performance(
    char stage_name[MAX_SIZE], 
    struct performance *performance
);

// Additional provided function prototypes
// You won't need to use these functions!
// We use them to implement some of the provided helper functions.
void format_time(int minutes, char buffer[MAX_SIZE]);
void get_genre_name(enum genre genre, char buffer[MAX_SIZE]);
int scan_token(char buffer[MAX_SIZE], int buffer_size);
enum genre string_to_genre(char genre_str[MAX_SIZE]);

///////////////////////////////////////////////////////////////////////////////
// Your Function Prototypes
///////////////////////////////////////////////////////////////////////////////

// Stage 1
struct festival *create_festival(char name[MAX_SIZE]);
struct stage *create_stage(char name[MAX_SIZE]);
struct performance *create_performance(
    char band_name[MAX_SIZE], 
    int start_time, 
    int max_duration, 
    enum genre genre
);
struct song *create_song(
    char title[MAX_SIZE], 
    double duration, 
    int popularity
);
void command_loop(struct festival *festival);
void add_stage(struct festival *festival);

// Add your function prototypes below
//stage 2.1 - append performance
void add_performance(struct festival *festival);
// Stage 2.2 - Insert Song
void insert_song(struct festival *festival);
void insert_song_position (struct performance *performance, 
    struct song *new_song, int position);
struct stage *find_stage (
    struct festival *festival, 
    char stage_name[MAX_SIZE]
);
struct performance *find_performance (
    struct stage *stage, 
    char band_name[MAX_SIZE]
);
int song_exists (
    struct performance *performance, 
    char title[MAX_SIZE], 
    double duration, 
    int popularity
);
double total_duration (struct performance *performance);
// Stage 2.3 - Print Most Popular Performance
void print_popular_perform (struct festival *festival);
double ave_popularity(struct performance *performance);
// Stage 2.4 - Print Band Summary
void print_band_sum (struct festival *festival);

// Stage 3.1 - Cancel Festival Plan
void free_festival (struct festival *festival);
void free_stage (struct stage *stage);
void free_performance (struct performance *performance);
void free_song (struct song *song);

// Stage 3.2 - Remove Stage and Performance
void remove_perform (struct festival *festival);
void remove_stage (struct festival *festival);
void update_removed_time(struct stage *stage);

int count_song (struct performance *performance);
// Stage 3.3 - Remove Song
void remove_song(struct festival *festival);
// Stage 3.4 - Update Performance Duration
void update_duration(struct festival *festival);

// Stage 3.5 - Move Performance
void move_perform(struct festival *festival);
int count_perform(struct stage *stage);
int insert_perform(struct stage *stage, struct performance *performance, 
    int position);
int move_error_check (struct stage *src_stage, struct stage *dest_stage, 
    char src_stage_name[MAX_SIZE], char dest_stage_name[MAX_SIZE], 
    char band_name[MAX_SIZE]);

// Stage 4.1 - Join Performances
void join_perform(struct festival *festival);
void view_conflicts (struct festival *festival);
void resolve_conflicts (struct festival *festival);
