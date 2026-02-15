// Assignment 2 25T3 COMP(1511|1911): CS Festival
// cs_festival.c
//
// This program was written by <Hanneli Yu> 
// on <11/14/2025>
//
// <A comprehensive festival planning system>

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "cs_festival.h"

#define YES 1
#define NO 0
#define GAP 10

// Stage 1.1
// Function to create and initialise a festival
// Params:
//      name - the name of the festival
// Returns: 
//      A pointer to the newly created festival
struct festival *create_festival(char name[MAX_SIZE]) {
    struct festival *node = malloc(sizeof(struct festival));
    strcpy(node->name, name);
    node->stages = NULL;
    return node;
}

// Function to create and initialise a stage
// Params:
//      name - the name of the stage
// Returns: 
//      A pointer to the newly created stage
struct stage *create_stage(char name[MAX_SIZE]) {
    struct stage *node = malloc(sizeof(struct stage));
    strcpy(node->name, name);
    node->performances = NULL;
    node->next = NULL;
    return node;
}

// Function to create and initialise a performance
// Params:
//      band_name   - the name of the band
//      start_time  - the start time of the performance 
//                    (in minutes from the start of the festival)
//      max_duration- the maximum duration allowed for the performance 
//                    (in minutes)
//      genre       - the genre of the performance 
// Returns: 
//      A pointer to the newly created performance
struct performance *create_performance(
    char band_name[MAX_SIZE], 
    int start_time, 
    int max_duration, 
    enum genre genre
) {
    struct performance *node = malloc(sizeof(struct performance));
    strcpy(node->band_name, band_name);
    node->start_time = start_time;
    node->max_duration = max_duration;
    node->end_time = start_time + max_duration;
    node->genre = genre;
    node->setlist = NULL;
    node->next = NULL;
    return node;
}

// Function to create and initialise a song
// Params:
//      title      - the title of the song
//      duration   - the duration of the song (in minutes)
//      popularity - the popularity rating of the song 
// Returns: 
//      A pointer to the newly created song
struct song *create_song(
    char title[MAX_SIZE], 
    double duration, 
    int popularity
) {
    struct song *node = malloc(sizeof(struct song));
    strcpy(node->title, title);
    node->duration = duration;
    node->popularity = popularity;
    node->next = NULL;
    return node;
}

// Stage 1.2
// Function to run the main command loop for the program
// Params:
//      festival - a pointer to the festival
// Returns: None
void command_loop(struct festival *festival) {
    char command;
    printf("Enter command: ");
    while (scanf(" %c", &command) == 1 && command != 'q') {
        if (command == '?') {
            print_help();
        } else if (command == 'a') {
            scanf(" %c", &command);
            if (command == 's') {
                add_stage(festival);
            } else if (command == 'p') {
                add_performance(festival);
            }
        } else if (command == 'p') {
            print_festival(festival);
        } else if (command == 'i') {
            scanf(" %c", &command);
            if (command == 'g') {
                insert_song(festival);
            }
        } else if (command == '*') {
            print_popular_perform(festival);
        } else if (command == 'P') {
            print_band_sum(festival);
        } else if (command == 'x') {
            scanf(" %c", &command);
            if (command == 'p') {
                remove_perform(festival);
            } else if (command == 's') {
                remove_stage(festival);
            } else if (command == 'g') {
                remove_song(festival);
            }
        } else if (command == 'u') {
            update_duration(festival);
        } else if (command == 'm') {
            move_perform(festival);
        } else if (command == 'j') {
            join_perform(festival);
        } else if (command == 'c') {
            scanf(" %c", &command);
            if (command == 'v') {
                view_conflicts(festival);
            } else if (command == 'r') {
                resolve_conflicts(festival);
            } 
        } else {
            printf("ERROR: Invalid command.\n");
        }
        printf("Enter command: ");
    }
    return;
}

// Stage 1.3
// Function to add a stage to the festival
// Params:
//      festival - a pointer to the festival
// Returns: None
void add_stage(struct festival *festival) { 
    char stage_name[MAX_SIZE];
    scan_name(stage_name);
    //error check
    struct stage *current = festival->stages;
    while (current != NULL) {
        if (strcmp(stage_name, current->name) == 0) {
            printf("ERROR: Stage with name '%s' already exists.\n", stage_name);
            return;
        }
        current = current->next;
    }
    //add new stage
    struct stage *new_stage = create_stage(stage_name);
    new_stage->next = festival->stages;
    festival->stages = new_stage;
    printf("Stage '%s' added.\n", stage_name);
    return;
}

// Stage 1.4 Part 1
// Function to print the setlist of a performance
// Params:
//      setlist - a pointer to the first song in the setlist
// Returns: None
void print_setlist(struct song *setlist) {

    if (setlist == NULL) {
        printf("  (no songs yet)\n");
        return;
    }

    struct song *current = setlist;
    int song_num = 1;
    while (current != NULL) {
        print_song(current, song_num);
        current = current->next;
        song_num++;
    }
    return;
}

// Stage 1.4 Part 2
// Function to print the entire festival, including stages and performances
// Params:
//      festival - a pointer to the festival
// Returns: None
void print_festival(struct festival *festival) {
    printf("Festival: %s\n",festival->name);

    if (festival->stages == NULL) {
        printf("  (no stages yet)\n");
        return;
    }

    struct stage *current_stage = festival->stages;
    while (current_stage != NULL) {
        print_stage_title(current_stage);

        if (current_stage->performances == NULL) {
            printf("  (no performances yet)\n");
        }
        struct performance *current_perform = current_stage->performances;
        while (current_perform != NULL) {
            printf("\n");
            print_performance_header(current_perform);
            print_setlist(current_perform->setlist);
            current_perform = current_perform->next;
        }
        printf("\n");
        current_stage = current_stage->next;
    }
    return;
}


// Add your function definitions below this line
// Stage 2.1 - Append Performance
void add_performance(struct festival *festival) {
    char add_stage_name[MAX_SIZE];
    scan_name(add_stage_name);
    char add_band_name[MAX_SIZE];
    scan_name(add_band_name);
    enum genre add_genre = scan_genre();
    int max_duration;
    scanf("%d", &max_duration);
    //error check
    if (add_genre == INVALID_GENRE) {
        printf("ERROR: Invalid genre.\n");
        return;
    }
    if (max_duration <= 0) {
        printf("ERROR: Duration must be positive.\n");
        return;
    }
    //find stage and add performance
    struct stage *current_stage = find_stage(festival, add_stage_name);
    if (current_stage == NULL) {
        printf("ERROR: No stage with name '%s' found.\n", add_stage_name);
        return;
    }
    if (find_performance(current_stage, add_band_name) != NULL) {
        printf("ERROR: Performance for band '%s' ", add_band_name);
        printf("already exists on stage '%s'.\n", add_stage_name);
        return;
    }
    int start_time = 0;
    if (current_stage->performances != NULL) {
        struct performance *last_perform = current_stage->performances;
        while (last_perform->next != NULL) {
            last_perform = last_perform->next;
        }
        start_time = last_perform->end_time + GAP;
    }
    struct performance *new_perform = create_performance(add_band_name, 
        start_time, max_duration, add_genre);
    if (current_stage->performances == NULL) {
        current_stage->performances = new_perform;
    } else {
        struct performance *last_perform = current_stage->performances;
        while (last_perform->next != NULL) {
            last_perform = last_perform->next;
        }
        last_perform->next = new_perform;
    }
    printf("Performance for band '%s' added to stage '%s'.\n", 
        add_band_name, add_stage_name);
}

// Stage 2.2 - Insert Song
void insert_song(struct festival *festival) {
    char stage_name[MAX_SIZE];
    scan_name(stage_name);
    char band_name[MAX_SIZE];
    scan_name(band_name);
    char title[MAX_SIZE];
    scan_name(title);
    double duration;
    scanf("%lf", &duration);
    int popularity;
    scanf("%d", &popularity);
    int position;
    scanf("%d", &position);
    //error check
    if (popularity < 1 || popularity > 10) {
        printf("ERROR: Song popularity must be between 1 and 10 (inclusive).");
        printf("\n");
        return;
    }
    if (duration <= 0) {
        printf("ERROR: Duration must be positive.\n");
        return;
    }
    struct stage *current_stage = find_stage(festival, stage_name);
    if (current_stage == NULL) {
        printf("ERROR: No stage with name '%s' found.\n", stage_name);
        return;
    }
    struct performance *current_perform = find_performance(current_stage, 
        band_name);
    if (current_perform == NULL) {
        printf("ERROR: No performance for band '%s' found on stage '%s'.\n", 
            band_name, stage_name);
        return;
    }
    if (song_exists(current_perform, title, duration, popularity) == YES) {
        printf("ERROR: Song '%s' with duration '%.1lf' and ", title, duration);
        printf("popularity '%d' already exists in the setlist.\n", popularity);
        return;
    }
    double sum_duration = total_duration(current_perform);
    if (sum_duration + duration > current_perform->max_duration) {
        printf("ERROR: Adding song would exceed max duration ");
        printf("of performance for band '%s'.\n", band_name);
        return;
    }
    //insert song at the correct position
    struct song *new_song = create_song(title, duration, popularity);
    insert_song_position(current_perform, new_song, position);
    printf("Song '%s' added to performance for band '%s' on stage '%s'.\n", 
        title, band_name, stage_name);
}

// Insert Song at the correct position
void insert_song_position (struct performance *performance, 
    struct song *new_song, int position) {
    int n = count_song(performance);
    if (position <= 1) {
        new_song->next = performance->setlist;
        performance->setlist = new_song;
        return;
    } 
    if (position > n) {
        if (performance->setlist == NULL) {
            performance->setlist = new_song;
        } else {
            struct song *last = performance->setlist;
            while (last->next != NULL) {
                last = last->next;
            }
            last->next = new_song;
        }
        return;
    }
    //insert in the middle
    struct song *previous_song = performance->setlist;
    for (int i = 1; i < position - 1; i++) {
        previous_song = previous_song->next;
    }
    new_song->next = previous_song->next;
    previous_song->next = new_song;
}

// Find the stage in the festival
struct stage *find_stage (
    struct festival *festival, 
    char stage_name[MAX_SIZE]
) {
    struct stage *current_stage = festival->stages;
    while (current_stage != NULL) {
        if (strcmp(stage_name, current_stage->name) == 0) {
            return current_stage;
        }
        current_stage = current_stage->next;
    }
    return NULL;
}

// Find the performance on a stage
struct performance *find_performance (
    struct stage *stage, 
    char band_name[MAX_SIZE]
) {
    struct performance *current_perform = stage->performances;
    while (current_perform != NULL) {
        if (strcmp(band_name, current_perform->band_name) == 0) {
            return current_perform;
        }
        current_perform = current_perform->next;
    }
    return NULL;
}

// Check if the song exists in a performance
int song_exists (
    struct performance *performance, 
    char title[MAX_SIZE], 
    double duration, 
    int popularity
) {
    struct song *current_song = performance->setlist;
    while (current_song != NULL) {
        if (strcmp(current_song->title, title) == 0 
        && current_song->duration == duration 
        && current_song->popularity == popularity) {
            return YES;
        }
        current_song = current_song->next;
    }
    return NO;

}

// Sum the duration of all songs in a performance
double total_duration (struct performance *performance) {
    double sum_duration = 0.0;
    struct song *current_song = performance->setlist;
    while (current_song != NULL) {
        sum_duration = sum_duration + current_song->duration;
        current_song = current_song->next;
    }
    return sum_duration;
}

// Stage 2.3 - Print Most Popular Performance
void print_popular_perform (struct festival *festival) {
    //check if performances exist
    if (festival->stages == NULL) {
        printf("No performances found.\n");
        return;
    }
    int performance_found = NO;
    struct stage *stage_check = festival->stages;
    while (stage_check != NULL) {
        if (stage_check->performances != NULL) {
            performance_found = YES;
        }
        stage_check = stage_check->next;
    }
    if (performance_found == NO) {
        printf("No performances found.\n");
        return;
    } else {
        printf("======== Popular performances ========\n");
    }
    //find max average popularity
    double max_average = 0.0;
    struct stage *current_stage = festival->stages;
    while (current_stage != NULL) {
        struct performance *current_perform = current_stage->performances;
        while (current_perform != NULL) {
            double average = ave_popularity(current_perform);
            if (average > max_average) {
                max_average = average;
            }
            current_perform = current_perform->next;
        }
        current_stage = current_stage->next;
    }
    //print 
    current_stage = festival->stages;
    while (current_stage != NULL) {
        struct performance *current_perform = current_stage->performances;
        while (current_perform != NULL) {
            double ave = ave_popularity(current_perform);
            if (ave == max_average) {
                print_stage_title(current_stage);
                print_performance_header(current_perform);
                print_setlist(current_perform->setlist);
                printf("\n");
            }
            current_perform = current_perform->next;
        }
        current_stage = current_stage->next;
    }
    return;
}

// Find the average popularity of songs in a performance
double ave_popularity(struct performance *performance) {
    if (performance->setlist == NULL) {
        return 0.0;
    }
    int i = 0;
    int sum = 0;
    struct song *current_song = performance->setlist;
    while (current_song != NULL) {
        sum = sum + current_song->popularity;
        i++;
        current_song = current_song->next;
    }
    double average = (double) sum / (double) i;
    return average;
}

// Stage 2.4 - Print Band Summary
void print_band_sum (struct festival *festival) {
    char band_name[MAX_SIZE];
    scan_name(band_name);
    if (festival->stages == NULL) {
        printf("No performances found for band '%s'.\n", band_name);
        return;
    }
    //count band performances
    int num_performances = 0;
    struct stage *current_stage = festival->stages;
    while (current_stage != NULL) {
        struct performance *current_perform = current_stage->performances;
        while (current_perform != NULL) {
            if (strcmp(current_perform->band_name, band_name) == 0) {
                num_performances++;
            }
            current_perform = current_perform->next;
        }

        current_stage = current_stage->next;
    }
    if (num_performances == 0) {
        printf("No performances found for band '%s'.\n", band_name);
        return;
    }
    //print summary
    printf("Band: %s\n", band_name);
    printf("Total Performances: %d\n\n", num_performances);
    current_stage = festival->stages;
    while (current_stage != NULL) {
        struct performance *current_perform = current_stage->performances;
        while (current_perform != NULL) {
            if (strcmp(current_perform->band_name, band_name) == 0) {
                print_stage_title(current_stage);
                print_performance_header(current_perform);
                print_setlist(current_perform->setlist);
                printf("\n");
            }
            current_perform = current_perform->next;
        }
        current_stage = current_stage->next;
    }
    return;
}

// Stage 3.1 - Cancel Festival Plan
void free_festival (struct festival *festival) {
    if (festival == NULL) {
        return;
    }
    free_stage(festival->stages);
    free(festival);
}

void free_stage (struct stage *stage) {
    while (stage != NULL) {
        struct stage *next = stage->next;
        free_performance(stage->performances);
        free(stage);
        stage = next;
    }
}

void free_performance (struct performance *performance) {
    while (performance != NULL) {
        struct performance *next = performance->next;
        free_song(performance->setlist);
        free(performance);
        performance = next;
    }
}

void free_song (struct song *song) {
    while (song != NULL) {
        struct song *next = song->next;
        free(song);
        song = next;
    }
}

// Stage 3.2 - Remove Stage and Performance
// Remove performances
void remove_perform (struct festival *festival) {
    char stage_name[MAX_SIZE];
    scan_name(stage_name);
    char band_name[MAX_SIZE];
    scan_name(band_name);
    //error check
    struct stage *current_stage = find_stage(festival, stage_name);
    if (current_stage == NULL) {
        printf("ERROR: No stage with name '%s' found.\n", stage_name);
        return;
    }
    if (current_stage->performances == NULL) {
        printf("ERROR: No performance for band '%s' found on stage '%s'.\n", 
            band_name, stage_name);
        return;
    }
    //remove band performances
    struct performance *current_perform = current_stage->performances;
    struct performance *previous_perform = NULL;

    while (current_perform != NULL) {
        if (strcmp(current_perform->band_name, band_name) == 0) {
            if (previous_perform == NULL) {
                current_stage->performances = current_perform->next;
            } else {
                previous_perform->next = current_perform->next;
            }
            free_song(current_perform->setlist);
            free(current_perform);
            update_removed_time(current_stage);
            printf("Performance for band '%s' removed from stage '%s'.\n", 
            band_name, stage_name);
            return;
        }
        previous_perform = current_perform;
        current_perform = current_perform->next;
    }
    printf("ERROR: No performance for band '%s' found on stage '%s'.\n", 
            band_name, stage_name);
}
// Remove stages
void remove_stage (struct festival *festival) {
    char stage_name[MAX_SIZE];
    scan_name(stage_name);
    
    if (festival->stages == NULL) {
        printf("ERROR: No stage with name '%s' found.\n", stage_name);
        return;
    }

    struct stage *current_stage = festival->stages;
    struct stage *previous_stage = NULL;

    while (current_stage != NULL) {
        if (strcmp(current_stage->name, stage_name) == 0) {
            if (previous_stage == NULL) {
                festival->stages = current_stage->next;
            } else {
                previous_stage->next = current_stage->next;
            }
            free_performance(current_stage->performances);
            free(current_stage);
            printf("Stage '%s' removed.\n", stage_name);
            return;
        }
        previous_stage = current_stage;
        current_stage = current_stage->next;
    }
    printf("ERROR: No stage with name '%s' found.\n", stage_name);
}

// Update start & end time for any changes
void update_removed_time(struct stage *stage) {
    if (stage->performances == NULL) {
        return;
    }
    struct performance *current_perform = stage->performances;
    int time = 0;
    while (current_perform != NULL) {
        current_perform->start_time = time;
        current_perform->end_time = time + current_perform->max_duration;
        time = current_perform->end_time + GAP;

        current_perform = current_perform->next;
    }

}
// Stage 3.3 - Remove Song
void remove_song(struct festival *festival) {
    char stage_name[MAX_SIZE];
    scan_name(stage_name);
    char band_name[MAX_SIZE];
    scan_name(band_name);
    int position;
    scanf("%d", &position);
    struct stage *current_stage = find_stage(festival, stage_name);
    if (current_stage == NULL) {
        printf("ERROR: No stage with name '%s' found.\n", stage_name);
        return;
    }
    struct performance *current_perform = find_performance(current_stage, 
        band_name);
    if (current_perform == NULL) {
        printf("ERROR: No performance for band '%s' found on stage '%s'.\n", 
            band_name, stage_name);
        return;
    }
    if (current_perform->setlist == NULL) {
        printf("ERROR: Performance for band '%s' has no songs to remove.\n", 
            band_name);
        return;
    }
    int song_num = count_song(current_perform);
    if (position <= 0 || position > song_num) {
        printf("ERROR: Invalid song position '%d' ", position);
        printf("for performance of band '%s'.\n", band_name);
        return;
    }
    //remove songs
    struct song *current_song = current_perform->setlist;
    struct song *previous_song = NULL;
    int current_position = 1;
    while (current_song != NULL) {
        if (current_position == position) {
            char song_title[MAX_SIZE];
            strcpy(song_title, current_song->title);
            if (previous_song == NULL) {
                current_perform->setlist = current_song->next;
            } else {
                previous_song->next = current_song->next;
            }
            free(current_song);
            printf("Song '%s' removed from performance ", song_title);
            printf("for band '%s' on stage '%s'.\n", band_name, stage_name);
            return;
        }
        previous_song = current_song;
        current_song = current_song->next;
        current_position++;
    }
}
// Count the num of songs in a peformance
int count_song (struct performance *performance) {
    int n = 0;
    struct song *count_song = performance->setlist;
    while (count_song != NULL) {
        n++;
        count_song = count_song->next;
    }
    return n;
}

// Stage 3.4 - Update Performance Duration
void update_duration(struct festival *festival) {
    char stage_name[MAX_SIZE];
    scan_name(stage_name);
    char band_name[MAX_SIZE];
    scan_name(band_name);
    int new_duration;
    scanf("%d", &new_duration);
    //error check
    struct stage *current_stage = find_stage(festival, stage_name);
    if (current_stage == NULL) {
        printf("ERROR: No stage with name '%s' found.\n", stage_name);
        return;
    }
    struct performance *current_perform = find_performance(current_stage, 
        band_name);
    if (current_perform == NULL) {
        printf("ERROR: No performance for band '%s' found on stage '%s'.\n", 
            band_name, stage_name);
        return;
    }
    if (new_duration <= 0) {
        printf("ERROR: Duration must be positive.\n");
        return;
    }
    //update duration
    current_perform->max_duration = new_duration;
    double current_duration = total_duration(current_perform);
    //adjust songs
    while (current_perform->setlist != NULL 
        && current_duration > new_duration) {
        struct song *previous_song = NULL;
        struct song *current_song = current_perform->setlist;
        while (current_song->next != NULL) {
            previous_song = current_song;
            current_song = current_song->next;
        }
        if (previous_song == NULL) {
            current_perform->setlist = NULL;
        } else {
            previous_song->next = NULL;
        }
        current_duration = current_duration - current_song->duration;
        free(current_song);
    }
    //update time
    update_removed_time(current_stage);
    printf("Performance for band '%s' on stage '%s' ", band_name, stage_name);
    printf("updated to max duration '%d' minutes.\n", new_duration);

}

// Stage 3.5 - Move Performance
void move_perform(struct festival *festival) {
    char src_stage_name[MAX_SIZE];
    scan_name(src_stage_name);
    char dest_stage_name[MAX_SIZE];
    scan_name(dest_stage_name);
    char band_name[MAX_SIZE];
    scan_name(band_name);
    int position;
    scanf("%d", &position);
    struct stage *src_stage = find_stage(festival, src_stage_name);
    struct stage *dest_stage = find_stage(festival, dest_stage_name);
    //error check
    if (move_error_check(src_stage, dest_stage, src_stage_name, 
        dest_stage_name, band_name) == NO) {
        return;
    }
    //move performances
    struct performance *current_perform = src_stage->performances;
    struct performance *previous_perform = NULL;
    while (current_perform != NULL && 
        strcmp(current_perform->band_name, band_name) != 0) {
        previous_perform = current_perform;
        current_perform = current_perform->next;
    }
    struct performance *move_perform = find_performance(src_stage, band_name);
    if (current_perform != NULL) {
        if (previous_perform == NULL) {
            src_stage->performances = current_perform->next;
        } else {
            previous_perform->next = current_perform->next;
        }
        current_perform->next = NULL;
        move_perform = current_perform;
    }
    int insert_position = insert_perform(dest_stage, move_perform, position);
    //update time
    update_removed_time(dest_stage);
    if (src_stage != dest_stage) {
        update_removed_time(src_stage);
    }
    printf("Performance for band '%s' moved from ", band_name);
    printf("stage '%s' to stage '%s' ", src_stage_name, dest_stage_name);
    printf("at position '%d'.\n", insert_position);
}

// count performamces on a stage
int count_perform(struct stage *stage) {
    int i = 0;
    struct performance *current_perform = stage->performances;
    while (current_perform != NULL) {
        i++;
        current_perform = current_perform->next;
    }
    return i;
}

// Insert perform at a relocated position
int insert_perform(struct stage *stage, struct performance *performance, 
    int position) {
    int num_performances = count_perform(stage);
    int insert_position = 0;
    if (position <= 1) {
        insert_position = 1;
    } else if (position > num_performances) {
        insert_position = num_performances + 1;
    } else {
        insert_position = position;
    }
    if (insert_position <= 1) {
        performance->next = stage->performances;
        stage->performances = performance;
    } else {
        struct performance *insert_previous = stage->performances;
        int i = 1;
        while (i < insert_position - 1 && insert_previous->next != NULL) {
            insert_previous = insert_previous->next;
            i++;
        }
        performance->next = insert_previous->next;
        insert_previous->next = performance;
    }
    return insert_position;
}

// Error check for performance moving
int move_error_check (struct stage *src_stage, struct stage *dest_stage, 
    char src_stage_name[MAX_SIZE], char dest_stage_name[MAX_SIZE], 
    char band_name[MAX_SIZE]) {
    if (src_stage == NULL) {
        printf("ERROR: No stage with name '%s' found.\n", src_stage_name);
        return NO;
    }
    if (dest_stage == NULL) {
        printf("ERROR: No stage with name '%s' found.\n", dest_stage_name);
        return NO;
    }
    if (src_stage != dest_stage) {
        if (find_performance(dest_stage, band_name) != NULL) {
            printf("ERROR: Performance for band '%s' ", band_name);
            printf("already exists on stage '%s'.\n", dest_stage_name);
            return NO;
        }
    }
    struct performance *move_perform = find_performance(src_stage, band_name);
    if (move_perform == NULL) {
        printf("ERROR: No performance for band '%s' found on stage '%s'.\n", 
            band_name, src_stage_name);
        return NO;
    }
    return YES;
}

// Stage 4.1 - Join Performances
void join_perform(struct festival *festival) {
    char stage_name1[MAX_SIZE];
    scan_name(stage_name1);
    char band_name1[MAX_SIZE];
    scan_name(band_name1);
    char stage_name2[MAX_SIZE];
    scan_name(stage_name2);
    char band_name2[MAX_SIZE];
    scan_name(band_name2);
    struct stage *stage_1 = find_stage(festival, stage_name1);
    struct stage *stage_2 = find_stage(festival, stage_name2);
    struct performance *perform_1 = find_performance(stage_1, band_name1);
    struct performance *perform_2 = find_performance(stage_2, band_name2);
    if (stage_1 == NULL) {
        printf("ERROR: No stage with name '%s' found.\n", stage_name1);
        return;
    }
    if (stage_2 == NULL) {
        printf("ERROR: No stage with name '%s' found.\n", stage_name2);
        return;
    }
    if (perform_1 == NULL) {
        printf("ERROR: No performance for band '%s' found on stage '%s'.\n", 
            band_name1, stage_name1);
        return;
    }
    if (perform_2 == NULL) {
        printf("ERROR: No performance for band '%s' found on stage '%s'.\n", 
            band_name2, stage_name2);
        return;
    }
    if (perform_1->genre != perform_2->genre) {
        printf("ERROR: Joint performances can only be of the same genre.\n");
        return;
    }

}

// Stage 4.2 - Scheduling Conflicts
void view_conflicts (struct festival *festival) {
    char band_name[MAX_SIZE];
    scan_name(band_name);
    printf("ERROR: No performances found for band '%s'.\n", band_name);
    return;
}

void resolve_conflicts (struct festival *festival) {
    char band_name[MAX_SIZE];
    scan_name(band_name);
    printf("ERROR: No performances found for band '%s'.\n", band_name);
    return;
}
///////////////////////////////////////////////////////////////////////////////
// Provided Function Definitions
///////////////////////////////////////////////////////////////////////////////

// Scans a name string into the provided buffer, placing '\0' at the end.
//
// Params:
//      name - a char array of length MAX_SIZE, where the name should be stored
// Returns: None
// Usage example:
// ```
//      char name[MAX_SIZE];
//      scan_name(name);
// ```
void scan_name(char name[MAX_SIZE]) {
    scan_token(name, MAX_SIZE);
}

// Scans a string and converts it to a genre enum.
//
// Params: None
// Returns:
//      The corresponding genre enum, if the string was valid,
//      Otherwise, returns INVALID_GENRE.
//
// Usage example:
// ```
//      enum genre genre = scan_genre();
// ```
enum genre scan_genre() {
    char genre[MAX_SIZE];
    scan_token(genre, MAX_SIZE);
    return string_to_genre(genre);
}

// Function to print a stage title banner
// Params: None
// Returns: None
// Usage example:
// ```
//      print_stage_title(stage);
// ```
void print_stage_title(struct stage *stage) {
    printf("==========================================\n[%s]\n", stage->name);
}

// Function to print a performance header, including its start and end time 
// (formatted as hours:minutes from the start of the festival),
// band name, and genre
// Params: None
// Returns: None
// Usage example:
// ```
//      print_performance_header(performance);
// ```
void print_performance_header(struct performance *performance) {
    char start_buf[MAX_SIZE];
    char end_buf[MAX_SIZE];
    char genre_buf[MAX_SIZE];
    get_genre_name(performance->genre, genre_buf);
    format_time(performance->start_time, start_buf);
    format_time(performance->end_time, end_buf);

    printf("%-5s - %-5s | %s (%s)\n",
        start_buf,
        end_buf,
        performance->band_name,
        genre_buf
    );
}


// Function to print a single song for a setlist
// Params: None
// Returns: None
// Usage example:
// ```
//      print_song(song, 1);
// ```
void print_song(struct song *song, int song_num) {
    printf("  %d. %s (%.1f mins, %d)\n",
        song_num,
        song->title,
        song->duration,
        song->popularity
    );
}

// Function to print a conflict group header
// Params:
//      conflict_time - the start time of the conflict group 
//                      (in minutes from the start of the festival)
// Returns: None
// Usage example:
// ```
//      print_conflict_group_header(conflict_time);
// ```
void print_conflict_group_header(int conflict_time) {
    char buffer[MAX_SIZE];
    format_time(conflict_time, buffer);
    printf(
        "\nConflicts found from %s:\n-------------------------------------"
        "-----\n", buffer
    );
}

// Function to print a conflicting performance with its stage name
// Params:
//      stage_name  - the name of the stage the performance is on
//      performance - a pointer to the performance
// Returns: None
// Usage example:
// ```
//      print_conflict_performance(stage_name, performance);
// ```
void print_conflict_performance(
    char stage_name[MAX_SIZE], 
    struct performance *performance
) {
    printf("    - [%s] ", stage_name);
    print_performance_header(performance);
}

// Function to print help information
// '?' command
// Params: None
// Returns: None
// Usage:
// ```
//      print_help();
// ```
void print_help(void) {
    printf(
        "*********************[     Usage Info     ]********************* \n"
        "  a s [name]                                                     \n"
        "    Add a stage with unique name                                 \n"
        "  a p [stage_name] [band_name] [genre] [max_duration]            \n"
        "    Append a performance to a stage                              \n"
        "  i g [stage_name] [band_name] [title] [duration] [popularity] "
        "[position]\n"
        "    Insert a song at [position] in a performance                 \n"
        "  p                                                              \n"
        "    Print the festival with all stages and performances          \n"
        "  *                                                              \n"
        "    Print the most popular performance(s)                        \n"
        "  P [band_name]                                                  \n"
        "    Print schedule and setlists for [band_name], plus summary    \n"
        "  x s [stage_name]                                               \n"
        "    Close a stage and free all its performances                  \n"
        "  x p [stage_name] [band_name]                                   \n"
        "    Remove a performance from a stage                            \n"
        "  x g [stage_name] [band_name] [position]                        \n"
        "    Remove the nth song from a performance                       \n"
        "  u [stage_name] [band_name] [new_duration]                      \n"
        "    Update performance max duration, trimming songs if shorter   \n"
        "  m [src_stage] [dest_stage] [band_name] [position]              \n"
        "    Move a performance to [dest_stage] at [position]             \n"
        "  j [stage_name1] [band_name1] [stage_name2] [band_name2]        \n"
        "    Join two performances                                        \n"
        "  c v [band_name]                                                \n"
        "    View all conflicting performances for [band_name]            \n"
        "  c r [band_name]                                                \n"
        "    Resolve all conflicting performances for [band_name]         \n"
        "  ?                                                              \n"
        "    Show this help information                                   \n"
        "  q                                                              \n"
        "    Quit the program                                             \n"
        "**************************************************************** \n"
    );
}

///////////////////////////////////////////////////////////////////////////////
// Additional Provided Functions
///////////////////////////////////////////////////////////////////////////////

// You don't need to use any of these, or understand how they work!
// We use them to implement some of the provided helper functions.

void print_welcome_banner() {
    printf("Welcome to CS Festival!\n");
    printf("♪┏(・o･)┛┗ ( ･o･) ┓♪\n");
}

void format_time(int minutes, char buffer[MAX_SIZE]) {
    int hours = minutes / 60;
    int mins  = minutes % 60;
    sprintf(buffer, "%02d:%02d", hours, mins);
}

enum genre string_to_genre(char genre_str[MAX_SIZE]) {
    if (strcmp(genre_str, "ROCK") == 0) {
        return ROCK;
    } else if (strcmp(genre_str, "POP") == 0) {
        return POP;
    } else if (strcmp(genre_str, "COUNTRY") == 0) {
        return COUNTRY;
    } else if (strcmp(genre_str, "JAZZ") == 0) {
        return JAZZ;
    } else if (strcmp(genre_str, "METAL") == 0) {
        return METAL;
    } else if (strcmp(genre_str, "HIPHOP") == 0) {
        return HIPHOP;
    }
    return INVALID_GENRE;
}

void get_genre_name(enum genre genre, char buffer[MAX_SIZE]) {
    if (genre == ROCK) {
        strcpy(buffer, "Rock");
    } else if (genre == POP) {
        strcpy(buffer, "Pop");
    } else if (genre == COUNTRY) {
        strcpy(buffer, "Country");
    } else if (genre == JAZZ) {
        strcpy(buffer, "Jazz");
    } else if (genre == METAL) {
        strcpy(buffer, "Metal");
    } else if (genre == HIPHOP) {
        strcpy(buffer, "Hip Hop");
    } else {
        strcpy(buffer, "Invalid Genre");
    }
}

int scan_token(char buffer[MAX_SIZE], int buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    char c;
    int i = 0;
    int num_scanned = 0;

    scanf(" ");

    while (i < buffer_size - 1 && (num_scanned = scanf("%c", &c)) == 1 &&
           !isspace(c)) {
        buffer[i++] = c;
    }

    if (i > 0) {
        buffer[i] = '\0';
    }

    return num_scanned;
}
