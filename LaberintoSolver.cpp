#include <stdio.h>

#define FALSE 0
#define TRUE 1

#define NROWS 6
#define MCOLS 6

// Symbols:
// '.' = open
// '#' = blocked
// 'S' = start
// 'G' = goal
// '+' = path
// 'x' = bad path

char maze[NROWS][MCOLS] = {
	{'S','.','#','.','#','#'},
	{'#','.','#','.','.','.'},
	{'#','.','.','#','.','#'},
	{'.','.','#','.','#','#'},
	{'#','.','.','.','.','G'},
	{'#','#','#','#','#','#'}
};

/*
char maze[NROWS][MCOLS] = {{'#','#','#','#','G','#'},{'#','.','#','.','.','#'},{'#','.','#','.','#','#'},{'#','.','#','.','.','#'},{'#','.','.','.','#','#'},{'#','#','#','#','#','#'}};
*/
void display_maze(void);
int find_path(int x, int y);


int
main(void)
{
	display_maze();

	if ( find_path(1, 1) == true )
		printf("Success!\n");
	else
		printf("Failed\n");

	display_maze();

	return 0;
}
// main()


void
display_maze(void)
{
	int i;

	printf("MAZE:\n");
	for ( i = 0; i < NROWS; i++ )
		printf("%.*s\n", MCOLS, maze[i]);
	printf("\n");

	return;
}


int
find_path(int x, int y)
{
	// If x,y is outside maze, return false.
	if ( x > MCOLS - 1 || y > NROWS - 1 ) return false;

	// If x,y is the goal, return true.
	if ( maze[y][x] == 'G' ) return true;

	// If x,y is not open, return false.
	if ( maze[y][x] != '.' && maze[y][x] != 'S' ) return false;

	// Mark x,y part of solution path.
	maze[y][x] = '+';

	// If find_path North of x,y is true, return true.
	if ( find_path(x, y - 1) == true ) return true;

	// If find_path East of x,y is true, return true.
	if ( find_path(x + 1, y) == true ) return true;

	// If find_path South of x,y is true, return true.
	if ( find_path(x, y + 1) == true ) return true;

	// If find_path West of x,y is true, return true.
	if ( find_path(x - 1, y) == true ) return true;

	// Unmark x,y as part of solution path.
	maze[y][x] = 'x';

	return false;
}
// find_path()
