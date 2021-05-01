//Escape Game Play CPP Source File

#include <iostream>
#include <cmath>
using namespace std;
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

//for randomizer
#include <ctime>

extern Console out;
/*
With graphics, screens are given an x,y coordinate system with the origin
in the upper left corner.  So it means the coordinate axes are:
---------------->   x direction
|
|
|
|
|
V

y direction 
*/
	
/*
    Print the game world
    
    The functions should draw characters to present features of the game
    board, e.g. win location, obstacles, power ups
*/
const int WINNER_Y = 4;
const int WINNER_X = 10;

const int GOOSE_INIT_X = 70;
const int GOOSE_INIT_Y = 20;

const int POWERUP_Y = 18;
const int POWERUP_X = 35;

int score = 0;


// print the game board function
void printBoard(int map[GAME_ROWS][GAME_COL])
{
    for(int rowCount = 0; rowCount < 21; rowCount++)
    {
    	for(int colCount = 0; colCount < 80; colCount++)
    	{
    		if(map[rowCount][colCount] == 1)
    		{
    			terminal_put(colCount, rowCount, WALL_CHAR);
			}
			if(map[rowCount][colCount] == 2)
			{
				terminal_put(colCount, rowCount, WIN_CHAR);
			}
			if(map[rowCount][colCount] == 3)
			{
				terminal_put(colCount, rowCount, POWERUP_CHAR);
			}
		}
	}
	
}

/*
    Do something when the goose captures the player
    
    If you want to attack or something else, this is the function you 
    need to change.  For example, maybe the two touch each other and
    then fight.  You could add a health to the Actor class that is
    updated.  Run, use weapons, it's up to you!
*/
//New Function for powerup..goose will get sent back to starting location
bool superPower(Actor & player)
{
	if((player.get_x() == POWERUP_X) && (player.get_y() == POWERUP_Y))
	{
		player.addPower();
	}
}

//function to check wehether captured
bool captured(Actor & player, Actor & goose)
{
	superPower(player);
	//if goose overlaps with powerup, goose will be sent back (no capture)
    if(player.get_x() == goose.get_x() 
	&& player.get_y() == goose.get_y() && player.get_status())
	{
		goose.set_x(GOOSE_INIT_X);
		goose.set_y(GOOSE_INIT_Y);
		player.resetPower();
		return false;
	}     	
	//if no powerup, the game will end (function will return true)
	else if (player.get_x() == goose.get_x() 
	&& player.get_y() == goose.get_y())
    {
    	return true;
	}
}

//return whether the player has gotten to the win sign
bool hasWon(Actor const & player, const int WINNER)
{
    return ((player.get_x() == WINNER_X)
         && player.get_y() == WINNER_Y);
}

void randomizer(int characterType, int map[GAME_ROWS][GAME_COL])
{
	
	//ramdomizer function and srand taken from an external source*
	srand((unsigned) time(0));
	
	
   	//to randomize walls        
   	if(characterType == SHALL_NOT_PASS)    
   	{
   	    //only places a wall randomly in one of 8 sections (
		//(to spread them out)
		//Code for horizontal wall randomizer
        for( int num =10; num <= 80; num += 10)
        {
            int ColWall = (rand() % num) + (num -10);
            int RowWall = (rand() % GAME_ROWS) + 1;

            for (int count = 0; count < 6; count ++)
            {
                map[RowWall][ColWall + count] = characterType;
            }
        }
        
        //Code for vertical wall randomizer
    	for( int num =10; num <= 80; num += 10)
    	{
       	    int ColWall = (rand() % num) + (num -10);
       	    int RowWall = (rand() % GAME_ROWS) + 1;


           	for (int count = 0; count < 6; count ++)
            {
            	map[RowWall + count][ColWall] = characterType;
            }
        }
        
    }
    
    //to randomize powerup and winner spot
    else
    {
        map[(rand() % GAME_ROWS) + 1][(rand() % GAME_COL) + 1] == characterType;
   	}

}

//if hasWon is true, DO NOT end game, but reset and add 1 to score (new level)
void newLevel(Actor & player, Actor & goose, int map[GAME_ROWS][GAME_COL])
{
	
	// clear previous level in array
	for(int rowCount = 0; rowCount < 21; rowCount++)
    {
    	for(int colCount = 0; colCount < 80; colCount++)
    	{
    		map[rowCount][colCount] == 0;
		}
	}
		
	//clear the whole board --> 80 x 21
	terminal_clear_area(0, 0, 80, 21);
	randomizer(SHALL_NOT_PASS, map);

	//randomize POWERUP location
	randomizer(POWERUP, map);	
		
		
	//add to the score
	score += 10;
	out.writeLine("Congrats on passing the level!");
	cout << "Score: " << score << endl;
	
	
	//print character and goose back to their initial positions
	randomizer(WINNER, map);
	player.set_x(10);
	player.set_y(10);
	
	goose.set_x(GOOSE_INIT_X);
	goose.set_y(GOOSE_INIT_Y);
		
	//print the randomized board
	printBoard(map);
	
	//reset status of powerup
	player.resetPower();
}

/*
    Move the player to a new location based on the user input.  You may want
    to modify this if there are extra controls you want to add.
    
    All key presses start with "TK_" then the character.  So "TK_A" is the a
    key being pressed.
    
    A look-up table might be useful.
    You could decide to learn about switch statements and use them here.
*/

void movePlayer(int key, Actor & player, int map[GAME_ROWS][GAME_COL])
{
    int yMove = 0, xMove = 0;
    if (key == TK_UP)
        yMove = -1;
    else if (key == TK_DOWN)
        yMove = 1;
    else if (key == TK_LEFT)
        xMove = -1;
    else if (key == TK_RIGHT)
        xMove = 1;
        
    if (player.can_move(xMove, yMove)  && 
	map[player.get_y() + yMove][player.get_x() + xMove] != SHALL_NOT_PASS)
        player.update_location(xMove, yMove);
}

/*
    What other functions do you need to make the game work?  What can you
    add to the basic functionality to make it more fun to play?
*/

void gooseChase(Actor & goose, Actor & player, int map[GAME_ROWS][GAME_COL])
{
	int distX = goose.get_x() - player.get_x();
	int distY = goose.get_y() - player.get_y();
	
	int moveX = 0;
	int moveY = 0;
	
	if(goose.get_x() - player.get_x() < 0)
	{
		moveX = 1;
		//jump through walls
		if(map[goose.get_y() + moveY][goose.get_x() + moveX] 
		== SHALL_NOT_PASS)
			moveX = 2;
	}
	else if(goose.get_x() - player.get_x() > 0)
	{
		moveX = -1;
		if(map[goose.get_y() + moveY][goose.get_x() + moveX] 
		== SHALL_NOT_PASS)
			moveX = -2;
	}
	
	if(goose.get_y() - player.get_y() < 0)
	{
		moveY = 1;
		if(map[goose.get_y() + moveY][goose.get_x() + moveX] 
		== SHALL_NOT_PASS)
			moveY = 2;
	}
	else if(goose.get_y() - player.get_y() > 0)
	{
		moveY = -1;
		if(map[goose.get_y() + moveY][goose.get_x() + moveX] 
		== SHALL_NOT_PASS)
			moveY = -2;
	}
    
    if (goose.can_move(moveX, moveY))
    	goose.update_location(moveX, moveY);
    
}












