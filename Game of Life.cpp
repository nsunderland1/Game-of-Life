/* Nicholas Sunderland
  
   ICS4U - Period 2
   September 18 2014
   
   This program is an implementation of Conway's Game of Life, an automated 
   simulation of cellular reproduction. It reads an initial configuration from
   a file, and runs through each generation until the user chooses to quit.
*/

#include <iostream>
#include <fstream>
#include <allegro.h>
using namespace std;
#include <apmatrix.h>

#define SCREEN_WIDTH 1000 
#define SCREEN_HEIGHT 440
#define ROWS 20
#define COLUMNS 50
#define GREEN makecol(0,255,0)
#define RED makecol(255,0,0)
#define WHITE makecol(255,255,255)

const char ALIVE = 'x', DEAD = '.';

//Grid of all cells used in simulation
apmatrix<char> grid(ROWS, COLUMNS);

void load_grid(char*);
void display_grid(int);
void next_gen();

int main() {
    string filename;
    cout << "Select a file: ";
    cin >> filename;
	
	//Reading initial configuration from file
    load_grid(filename.c_str());
    
    //Setting up graphics upon succesful file open
    allegro_init();
    install_keyboard();   
    set_color_depth(desktop_color_depth());
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0); 
    
    int generation = 1;
    
    //Running program until user chooses to exit
    while (!key[KEY_ESC]) {
        display_grid(generation);        
        next_gen();
        rest(125);   
        generation++;
    }

    return 0;
}
END_OF_MAIN();

//Reads initial configuration of game from a file
void load_grid(char*fileName) {
    ifstream inFile(fileName);
    
    //Checking for succesful file open
    if (!inFile.is_open()) {
        cout << "Failed to open " << fileName 
             << endl << endl;
        system("pause");
        exit(EXIT_FAILURE);       
    }
    
    //Reading grid
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLUMNS; j++)
            inFile >> grid[i][j];    
    
    inFile.close();
}

//Displays current configuration of grid
void display_grid(int generation) {
    BITMAP*buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    short x, y; //Stores location of current cell
     
    //Looping through entire grid 
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            //Updating cell location
            x = j*20;
            y = i*20;
            
            //Blitting green square for living cell, red for dead
            if (grid[i][j] == ALIVE)
               rectfill(buffer, x+1, y+1, x+20-1, y+20-1, GREEN);
            else 
               rectfill(buffer, x+1, y+1, x+20-1, y+20-1, RED);
        }
    }
    //Displaying current generation          
    textprintf_ex(buffer, font, 450, 420, WHITE, -1, "Generation: %d", generation);
    
    blit(buffer, screen, 0,0,0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    destroy_bitmap(buffer);
}

//Produces next generation of cells
void next_gen() {
     //Grid for next generation
     apmatrix<char> newgrid(ROWS, COLUMNS);
     
     //Counts number of adjacent living cells
     int neighbours; 
     
     //Setting initial state of next grid
     newgrid = grid;
     
     //Used to indicate direction of neighbouring cells
     short up, down;
     short left, right;
     
     //Checking every cell
     for (int i = 0; i < ROWS; i++) {
         for (int j = 0; j < COLUMNS; j++) {
             //Resetting neighbour count for current cell
             neighbours = 0;
             
             //Updating positions 
             up = i - 1;
             down = i + 1;
             left = j - 1;
             right = j + 1;
             
             //Checking status of each neighbouring cell
             if (j > 0 && grid[i][left] == ALIVE)
                neighbours++;
             if (j < COLUMNS-1 && grid[i][right] == ALIVE)
                neighbours++;
             if (i > 0 && grid[up][j] == ALIVE)
                neighbours++;
             if (i < ROWS-1 && grid[down][j] == ALIVE)
                neighbours++;
             if (i > 0 && j > 0 && grid[up][left] == ALIVE)
                neighbours++;
             if (i > 0 && j < COLUMNS-1 && grid[up][right] == ALIVE)
                neighbours++;
             if (i < ROWS-1 && j > 0 && grid[down][left] == ALIVE)
                neighbours++;
             if (i < ROWS-1 && j < COLUMNS-1 && grid[down][right] == ALIVE)
                neighbours++;   
             
             //Determining updated status of cell based on neighbour count
             if (grid[i][j] == ALIVE) {
                if (neighbours < 2)
                   newgrid[i][j] = DEAD;
                else if (neighbours == 2 || neighbours == 3)
                   newgrid[i][j] = ALIVE;
                else if (neighbours > 3)
                   newgrid[i][j] = DEAD;               
             }    
             else if (grid[i][j] == DEAD && neighbours == 3)
                  newgrid[i][j] = ALIVE;
         }    
     }   
     
     //Updating grid to current generation's configuration
     grid = newgrid; 
}
