# CS Festival Management System

A comprehensive C-based festival planning and management system that allows users to organize stages, performances, and setlists for music festivals.

## Project Overview

This project implements a festival management system using linked lists to handle multiple stages, band performances, and song setlists. Users can interactively add, remove, modify, and view festival data through a command-line interface.

## Features

### Stage 1: Core Setup
- **1.1**: Creating and Initialising Structs.
- **1.2**: Command Loop.
- **1.3**: Add Stage.
- **1.4**: Printing the Festival.

### Stage 2: Advanced Operations
- **2.1**: Append Performance.
- **2.2**: Insert Song.
- **2.3**: Print Most Popular Performance.
- **2.4**: Print Band Summary.

### Stage 3: Modifications & Memory Management
- **3.1**: Cancel Festival Plan.
- **3.2**: Remove Stage and Performance.
- **3.3**: Remove Song.
- **3.4**: Update Performance Duration.
- **3.5**: Move Performance.

## Data Structures

The system uses four main linked list structures:
```c
struct festival      // Top-level festival container
struct stage         // Individual performance stages
struct performance   // Band performances with timing
struct song          // Individual songs in setlists
```

## Supported Music Genres

- Rock
- Pop
- Country
- Jazz
- Metal
- Hip Hop

## Commands

| Command | Description |
|---------|-------------|
| `a s [name]` | Add a new stage |
| `a p [stage] [band] [genre] [duration]` | Add performance to stage |
| `i g [stage] [band] [title] [duration] [popularity] [position]` | Insert song into setlist |
| `p` | Print entire festival |
| `*` | Show most popular performances |
| `P [band]` | Print band summary |
| `x s [stage]` | Remove stage |
| `x p [stage] [band]` | Remove performance |
| `x g [stage] [band] [position]` | Remove song |
| `u [stage] [band] [new_duration]` | Update performance duration |
| `m [src_stage] [dest_stage] [band] [position]` | Move performance |
| `?` | Show help |
| `q` | Quit program |

## Compilation
```bash
gcc -Wall -Werror -std=c11 -o festival main.c cs_festival_stage3.c
```

## Usage Example
```
Welcome to CS Festival!
♪┏(・o･)┛♪ ( ･o･) ┗♪

Enter the name of your festival: Summer Beats 2026
Enter command: a s MainStage
Stage 'MainStage' added.
Enter command: a p MainStage ACDC ROCK 60
Performance for band 'ACDC' added to stage 'MainStage'.
Enter command: p
Festival: Summer Beats 2026
==========================================
[MainStage]

00:00 - 01:00 | ACDC (Rock)
  (no songs yet)
```

## Technical Constraints

- **No Arrays**: All data structures implemented using linked lists only (except for strings)
- **Dynamic Memory**: All structures allocated dynamically using `malloc()`
- **Memory Safety**: Complete memory cleanup with proper `free()` calls
- **Style Guidelines**: Adheres to COMP1511 coding standards

## Project Structure
```
.
├── main.c                    # Program entry point
├── cs_festival.h             # Header file with declarations
├── cs_festival_stage3.c      # Implementation file
└── README.md                 # This file
```

## Key Implementation Highlights

- **Linked List Operations**: Insert, delete, and traverse operations across multiple linked structures
- **Time Management**: Automatic scheduling with 10-minute breaks between performances
- **Error Handling**: Comprehensive validation for all user inputs
- **Memory Efficiency**: Proper allocation and deallocation of all dynamic structures

## Author

**Hanneli Yu** 
COMP1511 - 25T3 Assignment 2

## Academic Context

This project was completed as Assignment 2 for COMP1511/1911 at UNSW Sydney, demonstrating proficiency in:
- Linked list data structures
- Dynamic memory management
- Modular C programming
- Command-line interface design
- Complex data manipulation and validation

---

**Note**: This is an educational project. The code follows specific style guidelines and constraints as outlined in the course requirements.

