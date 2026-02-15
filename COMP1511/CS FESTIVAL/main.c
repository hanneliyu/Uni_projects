// CS Festival
// main.c
//
// This program was written by <Hanneli Yu> 
// on <11/14/2025>
//
// <A comprehensive festival planning system>

#include <stdio.h>

#include "cs_festival.h"

int main(void) {
    // Welcome banner
    print_welcome_banner();

    // Festival plan setup
    printf("Enter the name of your festival: ");
    char festival_name[MAX_SIZE];
    scan_name(festival_name);
    struct festival *my_festival = create_festival(festival_name);

    // Command loop
    command_loop(my_festival);

    free_festival(my_festival);

    // End message
    printf("Goodbye!\n");
    
    return 0;
}
