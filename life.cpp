#include "life.h"

// On creation allocate apmatrix grid and set all to false
LifeGrid::LifeGrid(){
	cell.resize(GRID_HEIGHT, GRID_WIDTH);
    for (int y = 0; y < GRID_HEIGHT; ++y){
        for (int x = 0; x < GRID_WIDTH; ++x){
        	cell[y][x] = false;
        }
    }
}
// Load from a file
void LifeGrid::load(const char* fname){
	std::ifstream input(fname);
	char c;
	if (!input.good()){
		throw 101;
	}
	// Read and validate input for grid
    for (int y = 0; y < GRID_HEIGHT; ++y){
        for (int x = 0; x < GRID_WIDTH; ++x){
        	c = input.get();
        	if (input.good()){
	        	if (c == '.'){
	        		cell[y][x] = false;
	        	}else if (c == 'x'){
	        		cell[y][x] = true;
	        	}else{
					throw 102;
	        	}
			}else{
				throw 103;
			}
        }
        input.get();// newline
    }
}
// Save to file
void LifeGrid::save(const char* fname){
	std::ofstream output(fname);
	if (!output.good()){
		throw 105;
	}
	// Save as x's and .'s
	// Newline seperated rows
    for (int y = 0; y < GRID_HEIGHT; ++y){
        for (int x = 0; x < GRID_WIDTH; ++x){
        	if (cell[y][x]){
        		output.put('x');
        	}else{
        		output.put('.');
        	}
        }
    	output.put('\n');
    }
	output.put('\n');
	output.close();
}
// Randomize based off a density value (0-100)
void LifeGrid::random(int density){
    for (int y = 0; y < GRID_HEIGHT; ++y){
        for (int x = 0; x < GRID_WIDTH; ++x){
        	cell[y][x] = (rand() % 100) < density;
        }
    }
}
// Based off the algorithm such that when counting a square, and the 8 surrounding it,
// 3 will always be alive
// 4 will remain the same
// all others will be dead
void LifeGrid::update(){
	apmatrix<bool> nextCell(GRID_HEIGHT, GRID_WIDTH);
	int life;
    for (int y = 0; y < GRID_HEIGHT; ++y){
        for (int x = 0; x < GRID_WIDTH; ++x){
            life = count9(x, y);
            if (life == 3){
                nextCell[y][x] = true;
            }else if (life == 4){
                nextCell[y][x] = cell[y][x];
            }else{
            	nextCell[y][x] = false;
            }
        }
    }
    cell = nextCell;
}
// Count all 9 squares, making sure not to count edges
int LifeGrid::count9(int x, int y){
    int counter = 0;
    for (int i = max(y - 1, 0); i <= min(y + 1, GRID_HEIGHT - 1); ++i){
        for (int j = max(x - 1, 0); j <= min(x + 1, GRID_WIDTH - 1); ++j){
        	if (cell[i][j]){
	        	++counter;
	        }
        }
    }
    return counter;
}
// Accessor for cell at x, y
bool LifeGrid::get(int x, int y){
	return cell[y][x];
}
// Setter for cell at x, y
void LifeGrid::set(int x, int y, bool state){
	cell[y][x] = state;
}
