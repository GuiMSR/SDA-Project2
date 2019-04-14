/* ========================================================================= *
 * Implements Maze functions:
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Maze.h"
#include "UnionFind.h"


/* Structure that informs about the walls state of the cell
     1 => door is open
     0 => door is closed*/

typedef struct wall_t{
    short int right;
    short int down;
}Wall;

struct maze_t{
    size_t size;
    Wall **cellValue;
};


Maze* mzCreate(size_t size){
    Maze *new_maze;
    new_maze = (Maze *)malloc(sizeof(Maze));
    if(new_maze == NULL){
       fprintf(stderr,"New maze couldn't be allocated\n");
        return NULL;
    }
    
    new_maze->cellValue = malloc(sizeof(Wall*) * size);
    if(new_maze->cellValue == NULL){
        printf("Coordinates couldn't be allocated\n");
        return NULL;
    }
    for (int nbis = 0; nbis < size; nbis ++){
        new_maze->cellValue[nbis] = malloc(sizeof(Wall) * size);
        if(new_maze->cellValue == NULL){
            fprintf(stderr,"Memory allocation denied : Error");
            return NULL;
        }
    }
    //creation of the closed maze
    for(int i = 0; i < size; i++){
        for (int j = 0; j < size ;j++){
            new_maze->cellValue[i][j].down = 0;
            new_maze->cellValue[i][j].right = 0;
        }
    }
    new_maze->size = size;
    return new_maze;
}

void mzMakeRandom(Maze* maze){
    if(maze == NULL){
        fprintf(stderr,"Maze required doesn't exist");
        return;
    }
    UnionFind* unionSet = ufCreate((maze->size)^2);
    Coord cell1;
    Coord cell2;
    cell1.row = 0;
    cell1.col = 0;
    cell2.row = 0;
    cell2.col = 0;
    // Randomizing maze
    for(int i = 0; i < maze->size; i++){
        for (int j = 0; j < maze->size ; j++){
            short int rndRight = rand()%1;
            short int rndDown = rand()%1;
            maze->cellValue[i][j].down = rndDown;
            maze->cellValue[i][j].right = rndRight;
        }
    }
    // Transforming the random maze into a valid one
    if (mzIsValid(maze) == true)        //checks if the maze is valid, and modifies the Union list with the connected cells
        return;
    else{
        for (size_t nbr = 1; nbr <= (maze->size^2); nbr++){
            ufStatus check1 = ufUnion(unionSet, nbr, nbr + 1);
            ufStatus check2 = ufUnion(unionSet, nbr, nbr + maze->size);
            if( check1 != UF_SAME){
                mzSetWall(maze, cell1, cell2, false);
                //change cells coords
                if(nbr % maze->size == 0){
                    cell1.row = cell1.row + 1;
                    cell1.col = 0;
                    cell2.row = cell1.row;
                    cell2.col = cell1.col + 1;
                }
                else{
                    cell1.col= cell1.col + 1;
                    cell2.col = cell1.col + 1;
                }
            }
            if(check2 != UF_SAME){
                mzSetWall(maze, cell1, cell2, false);
                //change cells coords
                if(nbr % maze->size == 0){
                    cell1.row = cell1.row + 1;
                    cell1.col = 0;
                    cell2.row = cell1.row;
                    cell2.col = cell1.col + 1;
                }
                else{
                    cell1.col= cell1.col + 1;
                    cell2.col = cell1.col + 1;
                }
            }
        }
    }
    ufFree(unionSet);
}


void mzFree(Maze* maze){
    if(maze == NULL){
        fprintf(stderr,"Maze asked doesn't exist");
    }
    else{
        for(int i = 0; i < maze->size; i++){
            free(maze->cellValue[i]);
        }
        free(maze->cellValue);
        free(maze);
        return;
    }
}

size_t mzSize(const Maze* maze){
    if(maze == NULL){
        fprintf(stderr,"Maze required doesn't exist");
        exit(-1);
    }
    else
        return maze->size;
}

bool mzIsWallClosed(const Maze* maze, Coord cell1, Coord cell2){
    if(maze == NULL){
        fprintf(stderr,"Maze required doesn't exist");
        return false;
    }
    size_t cell1Row = cell1.row;
    size_t cell1Col = cell1.col;
    size_t cell2Row = cell2.row;
    size_t cell2Col = cell2.col;
    
    if(cell1Row > maze->size || cell2Row > maze->size || cell1Col > maze->size || cell2Col > maze->size){
        fprintf(stderr,"Cell(s) out of bounds");
        return false;
    }
// Case cell1 at the left of cell2
    if(cell1Col < cell2Col){
        if(maze->cellValue[cell1Row][cell1Col].right == 0)
            return false;
        else
            return true;
    }
// Case cell1 at the right of cell2
    else if(cell1Col > cell2Col){
        if(maze->cellValue[cell2Row][cell2Col].right == 0)
            return false;
        else
            return true;
    }
// Case cell1 above cell2
    else if(cell1Row < cell2Row){
        if(maze->cellValue[cell1Row][cell1Col].down == 0)
            return false;
        else
            return true;
    }
// Case cell2 above cell1
    else if(cell1Row > cell2Row){
        if(maze->cellValue[cell2Row][cell2Col].down == 0)
            return false;
        else
            return true;
    }
    else{
        fprintf(stderr,"Cells are not neighbours");
        return false;
    }
}
    

void mzSetWall(Maze* maze, Coord cell1, Coord cell2, bool close){
    if(maze == NULL){
        fprintf(stderr,"Maze required doesn't exist");
        return;
    }
    size_t cell1Row = cell1.row;
    size_t cell1Col = cell1.col;
    size_t cell2Row = cell2.row;
    size_t cell2Col = cell2.col;
    
    if(cell1Row > maze->size || cell2Row > maze->size || cell1Col > maze->size || cell2Col > maze->size){
        fprintf(stderr,"Cell(s) out of bounds");
        return;
    }
    // Case cell1 at the left of cell2
    if(cell1Col < cell2Col){
        if(close == true)
            maze->cellValue[cell1Row][cell1Col].right = 0;
        else
            maze->cellValue[cell1Row][cell1Col].right = 1;
    }
    // Case cell1 at the right of cell2
    else if(cell1Col > cell2Col){
        if(close == true)
            maze->cellValue[cell2Row][cell2Col].right = 0;
        else
            maze->cellValue[cell2Row][cell2Col].right = 1;
    }
    // Case cell1 above cell2
    else if(cell1Row < cell2Row){
        if(close == true)
            maze->cellValue[cell1Row][cell1Col].down = 0;
        else
            maze->cellValue[cell1Row][cell1Col].down = 1;
    }
    // Case cell2 above cell1
    else if(cell1Row > cell2Row){
        if(close == true)
            maze->cellValue[cell2Row][cell2Col].down = 0;
        else
            maze->cellValue[cell2Row][cell2Col].down = 1;
    }
    else{
        fprintf(stderr,"Cells are not neighbours");
    }
}

bool mzIsValid(const Maze* maze){
    if(maze == NULL){
        fprintf(stderr,"Maze required doesn't exist");
        return false;
    }
    UnionFind* unionSet = ufCreate((maze->size)^2);
    size_t nbrComponents;
    short int nbr = 1;
    for(int row = 0; row < maze->size; row++){
        for(int col = 0; col < maze->size; col++){
            if(maze->cellValue[row][col].right == 1 && maze->cellValue[row][col].down == 1){
                ufUnion(unionSet, nbr, nbr+1);
                ufUnion(unionSet, nbr, nbr+maze->size);
                nbr++;
            }
            
            else if(maze->cellValue[row][col].right == 1){
                ufUnion(unionSet, nbr, nbr + 1);
                nbr++;
            }
            else if (maze->cellValue[row][col].down == 1){
                ufUnion(unionSet, nbr, nbr+maze->size);
                nbr++;
            }
        }
    }
    nbrComponents = ufComponentsCount(unionSet);
    if(nbrComponents == 1){
        ufFree(unionSet);
        return true;
    }
    else{
    ufFree(unionSet);
    return false;
    }
}

void mzPrint(const Maze* maze, FILE* out){
    if(out == NULL)
    {
        printf("Error! File doesn't exist");
        return;
    }
    
    for(int i=0 ; i < maze->size; i++)    // First line
        fprintf(out,"+--+");
    fprintf(out,"\n ");
    
    for(int row = 0; row < maze->size; row++){
        for(int col = 0; col < maze->size; row++){
            if(maze->cellValue[row][col].right == 0 && maze->cellValue[row][col].down == 0){        // Down and rigth closed
                    if(col != maze->size - 1){
                        fprintf(out,"  |");
                        fseeko(out, 0, SEEK_END);
                        fprintf(out,"+--");
                        fseeko(out, -3-(3*col), SEEK_END);
                    }
                    else{
                        fprintf(out,"  |");
                        fseeko(out, 0, SEEK_END);
                        fprintf(out,"+--+\n|");
                    }
            }
            else if(maze->cellValue[row][col].right == 0){      // Rigth closed
                if(col != maze->size - 1){
                    fprintf(out,"  |");
                    fseeko(out, 0, SEEK_END);
                    fprintf(out,"+  ");
                    fseeko(out, -3-(3*col), SEEK_END);
                }
                else{
                    fprintf(out,"  |");
                    fseeko(out, 0, SEEK_END);
                    fprintf(out, "+  \n|");
                }
            }
            else if(maze->cellValue[row][col].down == 0){       // Down closed
                if(col != maze->size - 1){
                    fprintf(out,"   ");
                    fseeko(out, 0, SEEK_END);
                    fprintf(out, "+--");
                    fseeko(out, -3-(3*col), SEEK_END);
                }
                else{
                    fprintf(out,"   ");
                    fseeko(out, 0, SEEK_END);
                    fprintf(out, "+--\n|");
                }
            }
            else{                               // Everything opened
                if(col != maze->size - 1){
                    fprintf(out,"   ");
                    fseeko(out, 0, SEEK_END);
                    fprintf(out, "+  ");
                    fseeko(out, -3-(3*col), SEEK_END);
                }
                else{
                    fprintf(out,"   ");
                    fseeko(out, 0, SEEK_END);
                    fprintf(out, "+  \n|");
                }
            }
        }
    }
}
