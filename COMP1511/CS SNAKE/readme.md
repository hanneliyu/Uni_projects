# CS Snake 🐍

A modified version of the classic Snake game implemented in C, where players control a snake that grows from its starting position while collecting apples and navigating obstacles.

## Project Overview

CS Snake reimagines the traditional Snake game with unique mechanics. Unlike the classic version, the snake's tail remains fixed at its starting position and uncoils as it moves. Players must strategically navigate walls, portals, and one-way passages while collecting apples to unlock exits and escape the board.

## Features

### Stage 1: Setup Phase
- **1.1**: Basic Setup Loop - Place walls, exits, and apples on the board.
- **1.2**: Error Checking - Validate positions and tile occupancy.
- **1.3**: Adding Long Walls - Create horizontal and vertical wall structures.
- **1.4**: Spawn Snake - Place the player's snake on the board.

### Stage 2: Gameplay Phase (Basic Mechanics)
- **2.1**: Basic Slithering - Move the snake in four directions with body elongation.
- **2.2**: Eating Apples and Unlocking Exits - Consume apples to unlock escape routes.
- **2.3**: Points and Statistics - Track score, moves, and completion percentage.
- **2.4**: Winning and Losing - End game conditions for success and failure.

### Stage 3: Gameplay Phase (Advanced Mechanics)
- **3.1**: Snake Reverse - Reverse apple flips the snake's direction.
- **3.2**: One-Way Passage - Directional tiles restricting movement.
- **3.3**: Snake Split - Split apple removes half the snake's body.
- **3.4**: Reset - Restart the game from initial setup.
- **3.5**: Portals - Teleport between paired portal locations.

## Game Entities

### Apple Types
| Entity | Symbol | Points | Special Effect |
|--------|--------|--------|----------------|
| Normal Apple | `(`)` | 5 | None |
| Reverse Apple | `(R)` | 10 | Reverses snake direction |
| Split Apple | `(S)` | 20 | Removes half the body segments |

### Board Elements
| Entity | Symbol | Description |
|--------|--------|-------------|
| Snake Head | `^~^` | Player's snake head |
| Rival Snake | `^v^` | Second player's snake |
| Body Segment | `###` | Snake body parts |
| Wall | `\|\|\|` | Impassable obstacle |
| Exit (Locked) | `[X]` | Requires all apples to unlock |
| Exit (Unlocked) | `[ ]` | Victory condition |
| Portal | `~O~` | Teleportation point |
| Passage Up | `^^^` | One-way upward movement |
| Passage Down | `vvv` | One-way downward movement |
| Passage Left | `<<<` | One-way leftward movement |
| Passage Right | `>>>` | One-way rightward movement |

## Commands

### Setup Phase Commands
| Command | Description | Format |
|---------|-------------|--------|
| `w` | Place single wall | `w <row> <col>` |
| `W` | Place long wall | `W <v\|h> <row> <col> <length>` |
| `e` | Place locked exit | `e <row> <col>` |
| `a` | Place apple | `a <n\|r\|s> <row> <col>` |
| `p` | Place one-way passage | `p <^\|v\|<\|>> <row> <col>` |
| `t` | Place portal pair | `t <row1> <col1> <row2> <col2>` |
| `s` | Start single-player game | `s` |
| `S` | Start multiplayer game | `S` |

### Gameplay Phase Commands
| Command | Description |
|---------|-------------|
| `w` | Move snake up |
| `a` | Move snake left |
| `s` | Move snake down |
| `d` | Move snake right |
| `p` | Print game statistics |
| `r` | Reset game to initial state |

## Data Structures

The game uses efficient structures to manage the board state:
```c
struct tile {
    enum entity entity;      // Type of entity on this tile
    int segment_split;       // Timestamp for body segment tracking
    int portal_number;       // Portal pair identifier (0-4)
};

struct portal_pair {
    int row1, col1;         // First portal coordinates
    int row2, col2;         // Second portal coordinates
    int paired;             // Pairing status flag
};
```

## Compilation
```bash
gcc -Wall -Werror -std=c11 -o cs_snake cs_snake.c
```

## Usage Example
```
Welcome to CS Snake!

--- Map Setup ---
Enter command: w 6 3
Enter command: e 7 5
Enter command: a n 2 8
Enter command: a r 3 1
Enter command: p ^ 3 8
Enter command: s
+---+---+---+---+---+---+---+---+---+---+
|            C S _ S N A K E            |
+---+---+---+---+---+---+---+---+---+---+
+   +   +   +   +   +   +   +   +   +   +
+   +   +   +   +   +   +   +   +   +   +
+   +   +   +   +   +   +   +   + (`) +
+   + (R) +   +   +   +   +   +   +   +
+   +   +   +   +   +   +   +   +   +   +
+   +   +   +   +   +   +   +   +   +   +
+   +   + ||| +   +   +   +   +   +   +
+   +   +   +   +   + [X] +   +   +   +
+   +   +   +   +   +   +   +   +   +   +
+   +   +   +   +   +   +   +   +   +   +

--- Spawning Snake ---
Enter the snake's starting position: 4 5
--- Gameplay Phase ---
Enter command: d
Enter command: d
Enter command: w
```

## Technical Constraints

- **Fixed Grid Size**: 10×10 game board
- **Maximum Portals**: 5 portal pairs
- **2D Array Management**: Efficient tile state tracking
- **No Dynamic Resizing**: Static board dimensions
- **Input Validation**: Comprehensive error checking for all commands

## Project Structure
```
.
├── cs_snake.c           # Complete game implementation
└── README.md            # This file
```

## Key Implementation Highlights

- **Snake Body Tracking**: Timestamp-based segment system for reverse and split mechanics
- **Portal System**: Linked portal pairs with automatic teleportation
- **Collision Detection**: Comprehensive boundary and entity checking
- **State Management**: Board reset functionality preserving initial setup
- **Distance Algorithms**: Manhattan distance for tail finding in reverse operations
- **Segment Sorting**: Bubble sort for identifying oldest body segments in split operations

## Game Statistics Tracking

The system monitors:
- **Points**: Cumulative score from eaten apples
- **Moves Made**: Total number of movements
- **Apples Eaten**: Count of consumed apples
- **Completion Percentage**: Progress toward clearing all apples
- **Maximum Points Remaining**: Potential score from uneaten apples

## Author

**Hanneli Yu** 
Written on October 14, 2025  
COMP1511 - UNSW Sydney

## Academic Context

This project was completed as part of COMP1511 coursework at UNSW Sydney, demonstrating proficiency in:
- Multi-dimensional array manipulation
- Game loop architecture
- State machine implementation
- Command parsing and validation
- Modular function design
- Complex game logic implementation


**Note**: This is an educational project following COMP1511 coding standards and constraints. The implementation showcases fundamental C programming concepts including structs, enums, 2D arrays, and procedural programming patterns.
