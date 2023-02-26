#include "minesweeper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_game(struct cell * game, int dim, int * dim_sizes, int num_mines, int ** mined_cells) {
 

    int num_of_cells = 1; 
    for (int i = 0; i < dim; i += 1) {
        num_of_cells *= dim_sizes[i]; 
    }

    int final_dim = dim - 1; 
    for (int i = 0; i < num_of_cells; i++) { 
        int dim_interval = 1;
        /* Assign Coordinates to cell*/
        for (int j = final_dim; j>= 0; j--) {
            int dim_coord = i / dim_interval;  
            dim_coord = dim_coord % dim_sizes[j];
            game[i].coords[j] = dim_coord;
            dim_interval *= dim_sizes[j]; 
        }

        game[i].mined = 0; 
        game[i].selected = 0; 
        game[i].hint = 0; 

        /* Flag mined cells */
        for (int j = 0; j < num_mines; j++) {
            int is_mine = 1; 
            for (int k = 0; k < dim; k++) {
                if (mined_cells[j][k] != game[i].coords[k]) {
                    is_mine = 0; 
                }
            }
            if (is_mine) {
                game[i].mined = 1; 
            }   
        }
    } 

    /* Flag adjacent cells */
    for (int i = 0; i < num_of_cells; i++) {
        int adj_index = 0;    
        for (int j = 0; j < num_of_cells; j++) {
            if (j != i) {
                int is_adjacent = 1; 
                for (int k = 0; k < dim; k++) {
                    int diff = game[i].coords[k] - game[j].coords[k]; 
                    if (diff > 1 || diff < -1) {
                        is_adjacent = 0; 
                    }
                }
                if (is_adjacent) {
                    if (game[j].mined) {
                        game[i].hint++; 
                    } else {
                        game[i].adjacent[adj_index] = &(game[j]); 
                        adj_index++; 
                    }
                }
            }
        }
        game[i].num_adjacent = adj_index; 
    }

    return;
}

int select_recursion(struct cell *game_cell) {

    if (game_cell->selected) {
        return 0; 
    } 
    game_cell->selected = 1; 

    // printf("Coords %d %d\n", game_cell->coords[0], game_cell->coords[1]);

    if (game_cell->mined) {
        return 1; 
    }

    if (!(game_cell->hint)) {
        int i = 0; 
        for (i = 0; i < game_cell->num_adjacent; i++) {
            if (!(game_cell->adjacent[i]->selected)) {
                select_recursion(game_cell->adjacent[i]); 
            }
        }
        return 2; 
    } 
    return 0; 
}

int completion_check(struct cell *game, int grid_size) {
    for (int i = 0; i < grid_size; i++) {
        if (!(game[i].selected) && !(game[i].mined)) {
            return 0; 
        }
    }
    return 2; 
}

int select_cell(struct cell * game, int dim, int * dim_sizes, int * coords) {
    int i = 0; 
    int j = dim - 1; 
    int dim_interval = 1; 
    for (; j >= 0; j--) {
        i += dim_interval*coords[j]; 
        dim_interval *= dim_sizes[j];
    }
    
    int response = select_recursion(&(game[i]));
    if (response == 2) {
        return completion_check(game, dim_interval);
    }
    return response; 
}

int main() {

    char dim_input[NUM_DIM]; 

    printf("Enter number of dimensions: "); 
    fgets(dim_input, NUM_DIM, stdin); 

    int dim = (int) strtol(dim_input, NULL, 10);  
    int *dim_sizes = (int*) malloc(dim * sizeof(int));
    char input_buf[NUM_DIM]; 
    int num_of_cells = 1; 
    printf("\n\nEnter dimension sizes: \n"); 
     for (int i = 0; i < dim; i++) {
        printf("\nEnter size for Dimension %d (1-n): ", i+1);
        fgets(input_buf, NUM_DIM, stdin); 
        dim_sizes[i] = (int) strtol(input_buf, NULL, 10);  
        num_of_cells *= dim_sizes[i];
    }  

    struct cell *game = (struct cell*) malloc(num_of_cells * sizeof(struct cell)); 
    char mines_in[NUM_DIM]; 
    printf("\n\nEnter number of mines: "); 
    fgets(mines_in, NUM_DIM, stdin); 
    int num_mines = (int) strtol(mines_in, NULL, 10);  
    int **mined_cells = (int**) malloc(num_mines * sizeof(int*)); 
    for (int i = 0; i < num_mines; i++) {
        mined_cells[i] = (int*) malloc(dim * sizeof(int)); 
        for (int j = 0; j < dim; j++) {
            mined_cells[i][j] = rand() % dim_sizes[j]; 
        }
    }

    init_game(game, dim, dim_sizes, num_mines, mined_cells);

    printf("\n\nStart Game!\n");
    int *player_coords = (int*) malloc(dim * sizeof(int)); 
    int response = 0; 
    while (response != 1 && response != 2) {
        for (int i = 0; i < dim; i++) {
            printf("\nEnter coordinate for Dimension %d (%d-%d): ", i+1, 0, dim_sizes[i]-1);
            fgets(input_buf, NUM_DIM, stdin); 
            player_coords[i] = (int) strtol(input_buf, NULL, 10);  
            while (player_coords[i] >= dim_sizes[i]) {
                printf("\nPlease enter a value within the defined range: "); 
                fgets(input_buf, NUM_DIM, stdin); 
                player_coords[i] = (int) strtol(input_buf, NULL, 10);  
            }
        }

        response = select_cell(game, dim, dim_sizes, player_coords); 
        if (response == 0) {
            printf("\nYou have dodged all mines!\n"); 
        } else if (response == 1) {
            printf("\nYou have stepped on a mine. Game over!\n"); 
        } else if (response == 2) {
            printf("\nAll non-mined cells have been explored. You win!\n"); 
        }
    } 

    free(dim_sizes);
    free(game); 
    free(player_coords); 
}

