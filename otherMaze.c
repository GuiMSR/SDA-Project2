/* ========================================================================= *
 * Maze Implementation:
 * =========================================================================
*/
#include "Maze.h"
#include <stdio.h>
#include <stdlib.h>
#include "UnionFind.h"

/*	Linear indexing conversion to Matrix indexing: 
	Matrix(i, j) = Vector(SIZE*i + j)
	Vector(k) = Matrix(k/SIZE, k%SIZE)

	Coord:	/!\ Check extremity of Matrix
	coordUp    = (i-1, j  )
	coordDown  = (i+1, j  )
	coordLeft  = (i  , j-1)
	coordRight = (i  , j+1)
	*/



struct maze_t{
	bool rightWallIsClosed;
	bool downWallIsClosed;
	UnionFind* set; // un Set
	Coord coord;
	int mazeSize;
};

Maze* mzCreate(size_t size){
	UnionFind * newSet = ufCreate(size*size);
	Maze * maze = malloc(size * size * sizeof(Maze));
	int cellsCnt = 0;
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			maze[cellsCnt].rightWallIsClosed = true;
			maze[cellsCnt].downWallIsClosed = true;
			maze[cellsCnt].set = newSet;
			maze[cellsCnt].coord.row = i;
			maze[cellsCnt].coord.col = j;
			maze[cellsCnt].mazeSize = size;
			cellsCnt++;
		}
	}
	return maze;
}

void mzFree(Maze* maze)
{
	ufFree(maze[0].set);
	free(maze);
	return;
}


bool mzIsWallClosed(const Maze* maze, Coord cell1, Coord cell2)
{
	// index
	int x1 = cell1.row, y1 = cell1.col;
	int x2 = cell2.row, y2 = cell2.col;
	int size = maze[0].mazeSize;
	int dx = x2 - x1;
	int dy = y2 - y1;
	//cases
	if(dx == 1 && dy == 0)
		return maze[size*x1+y1].downWallIsClosed;
	else if(dx == 0 && dy == 1)
		return maze[size*x1+y1].rightWallIsClosed;
	else if(dx == -1 && dy == 0)
		return maze[size*x2+y2].downWallIsClosed;
	else if(dx == 0 && dy == -1)
		return maze[size*x2+y2].rightWallIsClosed;
	else
		return true;
}

void mzSetWall(Maze* maze, Coord cell1, Coord cell2, bool close)
{
	// index
	int x1 = cell1.row, y1 = cell1.col;
	int x2 = cell2.row, y2 = cell2.col;
	int size = maze[0].mazeSize;
	int dx = x2 - x1;
	int dy = y2 - y1;
	// cases
	if(dx == 1 && dy == 0)
		maze[size*x1+y1].downWallIsClosed = close;
	else if(dx == 0 && dy == 1)
		maze[size*x1+y1].rightWallIsClosed = close;
	else if(dx == -1 && dy == 0)
		maze[size*x2+y2].downWallIsClosed = close;
	else if(dx == 0 && dy == -1)
		maze[size*x2+y2].rightWallIsClosed = close;
	else
		return;
	if(!close)
		ufUnion(maze[size*x1+y1].set, size*x1+y1, size*x2+y2);
	return;
}

bool mzIsValid(const Maze* maze)
{
	return (ufComponentsCount(maze[0].set) == 1); //not engough;
}

void mzPrint(const Maze* maze, FILE* out)
{
	return;
}

size_t mzSize(const Maze* maze)
{
	return maze[0].mazeSize;
}

void mzMakeRandom(Maze* maze){
	int SIZE = mzSize(maze);
	int indexes[SIZE*SIZE];
	for(int i = 0; i < SIZE*SIZE;i++)
		indexes[i] = i;
	//Mix indexes
	//printf("indexes was created succesfully\n");
	int walls[4] = {0, 1, 2, 3};
	bool firstRun = true;
	int runs = 0;
	while(ufComponentsCount(maze[0].set) != 1)
	{
		//mix indexes
		int max = SIZE*SIZE;
		for(int j = max; j > 1; j--)
		{
			int id = rand()%j;
			int tmp = indexes[id];
			indexes[id] = indexes[j-1];
			indexes[j-1] = tmp;
		}
		// complete the maze
		for(int i = 0; i < SIZE*SIZE; i++)
		{
			int index = indexes[i];
			int x = index/SIZE, y = index%SIZE;
			Coord coord;
			coord.row = x;
			coord.col= y;
			int nbrWallsToOpen = (rand()%4)+1;
			if(!firstRun)
				nbrWallsToOpen = 4;
			int nbrWalls = 4;
			//printf("x:%d, y:%d, toOpen:%d\n", x, y, nbrWallsToOpen);
			while(nbrWallsToOpen > 0)
			{
				int wallCase =  walls[rand()%nbrWalls];
				switch (wallCase)
				{
					case 0:{
						Coord coordUp;
						coordUp.row = x-1; coordUp.col = y;
						if(x-1 < 0)
							break;
						if(ufFind(maze[0].set, index) != ufFind(maze[0].set, index - SIZE))
							mzSetWall(maze, coord, coordUp, false);
						break;}
					case 1:{
						Coord coordLeft;
						coordLeft.row = x; coordLeft.col = y-1;
						if(y-1 < 0)
							break;
						if(ufFind(maze[0].set, index) != ufFind(maze[0].set, index - 1))
							mzSetWall(maze, coord, coordLeft, false);
						break;}
					case 2:{
						Coord coordDown;
						coordDown.row = x+1; coordDown.col = y;
						if(x+1 > SIZE-1)
							break;
						if(ufFind(maze[0].set, index) != ufFind(maze[0].set, index + SIZE))
							mzSetWall(maze, coord, coordDown, false);
						break;}
					case 3:{
						Coord coordRight;
						coordRight.row = x; coordRight.col = y+1;
						if(y+1 > SIZE-1)
							break;
						if(ufFind(maze[0].set, index) != ufFind(maze[0].set, index + 1))
							mzSetWall(maze, coord, coordRight, false);
						break;}
					default:
						break;
				}
				//swap
				int tmp = walls[wallCase];
				walls[wallCase]  = walls[nbrWalls - 1];
				walls[nbrWalls - 1] = tmp;
				nbrWalls--;
				nbrWallsToOpen--;
				
			}
			if (!firstRun && ufComponentsCount(maze[0].set) == 1)
				break;
			
		}
		if(firstRun)
			firstRun = false;

		//printf("Components after run %d : %d\n", runs+1, (int) ufComponentsCount(maze[0].set));
		runs++;
	}
	
	
	return;
}