#include "Pathfinder.h"

	Pathfinder::Pathfinder() {
		stringstream ss;
		for (int i = 0; i < ROW_SIZE; ++i) {
            for (int j = 0; j < COL_SIZE; ++j) {
                for (int k = 0; k < LAYER_SIZE; ++k) {
                    maze_grid[i][j][k] = 1;
					ss << maze_grid[i][j][k] << ' ';
                }
                ss << endl;
            }
            ss << endl;
        }
	}
	Pathfinder:: ~Pathfinder() {
		solution.clear();
	}


	string Pathfinder::toString() const
	{
	    if (maze_grid != 0){
        stringstream ss;
	    for(int row = 0; row < ROW_SIZE; row++) {
			for(int col = 0; col < COL_SIZE; col++) {
                for(int layer = 0; layer < LAYER_SIZE; layer++){
                    ss <<  maze_grid[row][col][layer];
                    if(layer < LAYER_SIZE-1){
                    ss << ' ';
                    }
                }
            ss << endl;
			}
            if(row < ROW_SIZE-1){
                ss << endl;
            }
	    }
	    return ss.str();
        }
        else {
        stringstream ss;
	    for(int row = 0; row < ROW_SIZE; row++) {
			for(int col = 0; col < COL_SIZE; col++) {
                for(int layer = 0; layer < LAYER_SIZE; layer++){
                    int maze_grid[row][col][layer] = {1};
                    ss <<  maze_grid[row][col][layer];
                    if(layer < LAYER_SIZE-1){
                    ss << ' ';
                    }
                }
            ss << endl;
			}
            if(row < ROW_SIZE-1){
                ss << endl;
            }
	    }
	    return ss.str();
        }
	}

	/*
	* createRandomMaze
	*
	* Generates a random maze and stores it as the current maze.
	*
	* The generated maze must contain a roughly equal number of 1s and 0s and must have a 1
	* in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).  The generated maze may be
	* solvable or unsolvable, and this method should be able to produce both kinds of mazes.
	*/
	void Pathfinder::createRandomMaze()
	{
	        for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    for (int k = 0; k < 5; ++k) {
                        maze_grid[i][j][k] = rand() % 2;
                }
            }
        }
        maze_grid[0][0][0] = 1;
        maze_grid[4][4][4] = 1;
	}
	//-----------------------------------------------------------------------------------------

	//Part 2-----------------------------------------------------------------------------------
	/*
	* importMaze
	*
	* Reads in a maze from a file with the given file name and stores it as the current maze.
	* Does nothing if the file does not exist or if the file's data does not represent a valid
	* maze.
	*
	* The file's contents must be of the format described above to be considered valid.
	*
	* Parameter:	file_name
	*				The name of the file containing a maze
	* Returns:		bool
	*				True if the maze is imported correctly; false otherwise
	*/
	bool Pathfinder::importMaze(string file_name)
	{
		cout << "importMaze from "<<file_name<<endl;
		ifstream file (file_name.c_str());
		char counter;
    	int count = 0;
    	if(file.is_open()){
        	while(file >> counter){
            	if(counter == '1' || counter == '0'){
                	count++;
            	} 
				else {
                	return false;
            	}
        	}
        	file.close();
    	}
    	else{
        	return false;
    	}
    	if(count!=125){
        	return false;
    	}
        file.open(file_name);
		if (file.is_open()) {
			string line;
			for(int row = 0; row < ROW_SIZE; row++) {
				getline(file, line);
				stringstream ss(line);
				for(int col = 0; col < COL_SIZE; col++) {
                    stringstream ss(line);
                    for(int layer = 0; layer < LAYER_SIZE; layer++){
                        int value;
					    ss >> value;
                    	if(layer == 0 && col == 0 && row == 0){
                        	if(value == 0){
                            	return false;
                        	}
                    	}
                    	if(layer == LAYER_SIZE-1 && col == COL_SIZE-1 && row == ROW_SIZE-1){
                        	if(value == 0){
                            	return false;
                        	}					    
                    	}
						maze_grid[row][col][layer] = value;
					}
                    getline(file, line);
				}
			}
			file.close();
		}
	    return true;
	}
	//-----------------------------------------------------------------------------------------

	
	
	bool Pathfinder::find_maze_path(int r, int c, int z) {
	  if (r < 0 || c < 0 || z < 0 || r > ROW_SIZE-1 || c > COL_SIZE-1 || z > LAYER_SIZE-1){
		return false;
		}      // Cell is out of bounds.
	  else if (maze_grid[r][c][z] == 0 || maze_grid[r][c][z] == 2) {
	    return false;
		}      // Cell is on barrier or dead end.
	  else if (r == ROW_SIZE-1 && c == COL_SIZE-1 && z == LAYER_SIZE-1) {
	    solution.push_back("(4, 4, 4)");
	    return true;               // and is maze exit.
	  }
	  solution.push_back("("+to_string(z)+", "+to_string(c)+", "+to_string(r)+")");
	  maze_grid[r][c][z] = 2;
	    // Recursive case.
	    // Attempt to find a path from each neighbor.
	    // Tentatively mark cell as on path.
	    if (find_maze_path(r - 1, c, z)
	        || find_maze_path(r + 1, c, z)
	        || find_maze_path(r, c - 1, z)
	        || find_maze_path(r, c + 1, z)
            || find_maze_path(r, c, z - 1)
            || find_maze_path(r, c, z + 1) ) {
	      return true;
	    }
	    else {
		  solution.pop_back();
	      return false;
	    }
		return false;
	  }
	
	  //Part 3-----------------------------------------------------------------------------------
	/*
	* solveMaze
	*
	* Attempts to solve the current maze and returns a solution if one is found.
	*
	* A solution to a maze is a list of coordinates for the path from the entrance to the exit
	* (or an empty vector if no solution is found). This list cannot contain duplicates, and
	* any two consecutive coordinates in the list can only differ by 1 for only one
	* coordinate. The entrance cell (0, 0, 0) and the exit cell (4, 4, 4) should be included
	* in the solution. Each string in the solution vector must be of the format "(x, y, z)",
	* where x, y, and z are the integer coordinates of a cell.
	*
	* Understand that most mazes will contain multiple solutions
	*
	* Returns:		vector<string>
	*				A solution to the current maze, or an empty vector if none exists
	*/
	
	vector<string> Pathfinder::solveMaze()
	{
		solution.clear();
		find_maze_path(0,0,0);
		for (int k = 0; k < 5; k++)
		{
			for (int j = 0; j < 5; j++)
			{
				for (int i = 0; i < 5; i++)
				{
					if (maze_grid[i][j][k] == 2)
					{
						maze_grid[i][j][k] = 1;
					}
				}
			}
		}
//				cout << "OUR MAZE 2" << endl << toString() << endl << endl;
		return solution;
	}