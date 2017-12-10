////////////////////////
//Nathanael Fixx
//CSS-342 - Project 6
//13 hours
//Cygwin
///////////////////////


#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <iomanip>


using namespace std;

//maze array is global
string maze[8][8];

//_____________________________________________
//Forward declaration of functions
void printMaze();
void fillMaze();
void addEnemies(int &wRow, int &wCol);
bool checkIfIn(int i, int j, char c);
bool isDead(int &row, int &col);
void move(int &row, int &col);
void remove(int row, int col, char c);
void getRoomNum(int row, int col);
void checkClues(int row, int col);
void moveWumpus(int &wRow, int &wCol);
//_____________________________________________


int main()
{
	int wRow, wCol; //for Wumpus
	fillMaze();
	addEnemies(wRow, wCol);
	move(wRow, wCol);
	
	//prints out the final location of player and enemies
	//once the player dies or kills the Wumpus
	cout << "Final Map: " << endl;
	printMaze();

    return 0;
}

//______________________________________________________
//printMaze
//______________________________________________________
void printMaze()
{
	cout << endl;

	cout << "*** LEGEND ***" << endl;
	cout << "X = footsteps" << endl;
	cout << "P = pit" << endl;
	cout << "B = bats" << endl;
	cout << "W = wumpus" << endl << endl;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << setw(9) << maze[i][j];
		}
		cout << endl << endl << endl;
	}
	cout << endl;
}

//______________________________________________________
//fillMaze
//
//Fills maze with numbers 1-64
//______________________________________________________
void fillMaze()
{
	string temp;
	int count = 1;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			temp = to_string(count); //stores the ints as strings
			maze[i][j] = temp;
			count++;
		}
	}
}

//______________________________________________________
//addEnemies
//
//Uses rand() to add 4 pits, 4 bats and a Wumpus to the
//maze
//______________________________________________________
void addEnemies(int &wRow, int &wCol) 
{
	srand(time(0));
	int num;
	int num2;

	for (int i = 0; i < 4; i++) {
		num = rand() % 8;
		num2 = rand() % 8;
		
		while (checkIfIn(num, num2, 'B') == true) { //checks if a B is not already in 
			num = rand() % 8;                       //that location.
			num2 = rand() % 8;
		}

		maze[num][num2] += "B"; //B is for Bats
	}

	for (int i = 0; i < 4; i++) {
		num = rand() % 8;
		num2 = rand() % 8;

		while (checkIfIn(num, num2, 'P') == true) {
			num = rand() % 8;
			num2 = rand() % 8;
		}

		maze[num][num2] += "P"; //P is for pit
	}

	wRow = rand() % 8;
	wCol = rand() % 8;

	maze[wRow][wCol] += "W"; //W is for Wumpus

}

//______________________________________________________
//checkIfIn
//
//checks if a specific char is in a string (room)
//______________________________________________________
bool checkIfIn(int i, int j, char c)
{
	int check = 0;
	string subject = maze[i][j];

	while (subject[check] != NULL) {
		if (subject[check] != c) {
			check++;
		}
		else
			return true;
	}
	return false;
}

//______________________________________________________
//isDead
//
//Uses the checkIfIn function to see if a player has
//died. If the player has been picked up by bats, it
//moves the bats and player to a different random
//location and checks once again if the player has died
//______________________________________________________
bool isDead(int & row, int & col)
{
	int batRow;
	int batCol;

	if (checkIfIn(row, col, 'W') == true) {
		cout << "You have been eaten by the Wumpus " << endl;
		return true;
	}

	if (checkIfIn(row, col, 'P') == true) {
		cout << "You fell in a pit and died " << endl;
		return true;
	}
	if (checkIfIn(row, col, 'B') == true) {
		cout << "You have been picked up by bats " << endl;
		remove(row, col, 'B');

		row = rand() % 8;
		col = rand() % 8;

		checkClues(row, col);
		maze[row][col] += "X";
		

		batRow = rand() % 8;
		batCol = rand() % 8;

		while (checkIfIn(batRow, batCol, 'B') == true) { 
			batRow = rand() % 8;
			batCol = rand() % 8;
		}

		maze[batRow][batCol] += "B";
		if (checkIfIn(row, col, 'B') == true) {
			cout << "You hear bats in the distance " << endl;
		}

		if (isDead(row, col) == true) {
			return true;
		}

	}
	
	return false;
}

//______________________________________________________
//move
//
//Places player into the maze and takes user input to 
//move him. 
//______________________________________________________
void move(int &wRow, int &wCol)
{
	int arrowCount = 4;
	int row = rand() % 8;
	int col = rand() % 8;
                                     //choosing a location for the player
	while (checkIfIn(row, col, 'B') == true || checkIfIn(row, col, 'P') == true || checkIfIn(row, col, 'W') == true) {
		row = rand() % 8; 
		col = rand() % 8;
	}
	
	cout << "Currently in room num: ";
	getRoomNum(row, col);
	checkClues(row, col);
	
	maze[row][col] += "X";

	string move;
	int win = 0;

	while (win == 0) { //loops until the Wumpus has been killed or
		           //until the player has died
		cin >> move;
		if (move == "i") {
			if (row - 1 >= 0) {
				row--;
				checkClues(row, col);
				maze[row][col] += "X";
				if (isDead(row, col) == true) {
					return;
				}
			}
			else {
				cout << "There is a wall there" << endl;
			}
		}

		if (move == "j") {
			if (col - 1 >= 0) {
				col--;
				checkClues(row, col);
				maze[row][col] += "X";
				if (isDead(row, col) == true) {
					return;
				}
			}
			else {
				cout << "There is a wall there" << endl;
			}
		}

		if (move == "l") {
			if (col + 1 < 8) {
				col++;
				checkClues(row, col);
				maze[row][col] += "X";
				if (isDead(row, col) == true) {
					return;
				}
			}
			else {
				cout << "There is a wall there" << endl;
			}
		}

		if (move == "m") {
			if (row + 1 < 8) {
				row++;
				checkClues(row, col);
				maze[row][col] += "X";
				if (isDead(row, col) == true) {
					return;
				}
			}
			else {
				cout << "There is a wall there" << endl;
			}
		}

		if (move == "I") {
			arrowCount--;

			if (checkIfIn(row - 1, col, 'W') == true) {
				cout << "You killed the Wumpus!! You Win!" << endl;
				return;
			}

			if (arrowCount == 0) {
				cout << "You are out of arrows. You lose." << endl;
				return;
			}

			else {
				moveWumpus(wRow, wCol);
				if (isDead(row, col) == true) {
					return;
				}
			}
		}

		if (move == "M") {
			arrowCount--;

			if (checkIfIn(row + 1, col, 'W') == true) {
				cout << "You killed the Wumpus!! You Win!" << endl;
				return;
			}

			if (arrowCount == 0) {
				cout << "You are out of arrows. You lose." << endl;
				return;
			}

			else {
				moveWumpus(wRow, wCol);
				if (isDead(row, col) == true) {
					return;
				}
			}
		}

		if (move == "J") {
			arrowCount--;

			if (checkIfIn(row, col - 1, 'W') == true) {
				cout << "You killed the Wumpus!! You Win!" << endl;
				return;
			}

			if (arrowCount == 0) {
				cout << "You are out of arrows. You lose." << endl;
				return;
			}

			else {
				moveWumpus(wRow, wCol);
				if (isDead(row, col) == true) {
					return;
				}
			}
		}

		if (move == "L") {
			arrowCount--;

			if (checkIfIn(row, col + 1, 'W') == true) {
				cout << "You killed the Wumpus!! You Win!" << endl;
				return;
			}

			if (arrowCount == 0) {
				cout << "You are out of arrows. You lose." << endl;
				return;
			}

			else {
				moveWumpus(wRow, wCol);
				if (isDead(row, col) == true) {
					return;
				}
			}
		}

		if (move == "map") {
			printMaze();
		}
		
		cout << "Currently in room num: ";
		getRoomNum(row, col);
		
	}
}

//______________________________________________________
//remove
//
//removes a specific char from a string. (Used to
//change locations of the bats and wumpus)
//______________________________________________________
void remove(int row, int col, char c)
{
	int check = 0;
	string subject = maze[row][col];

	while (subject[check] != NULL && subject[check] != c) {  //finds the character
		if (subject[check] != c) {
			check++;
		}
	}

		while (check != subject.length() - 1) {
			subject[check] = subject[check + 1];
			check++;
		}

		subject[check] = ' ';
		maze[row][col] = subject;
}

//______________________________________________________
//getRoomNum
//______________________________________________________
void getRoomNum(int row, int col)
{
	int i = 0;
	string subject = maze[row][col];

	while (isdigit(subject[i])) {
		cout << subject[i];
		i++;
	}
	cout << endl;
}

//______________________________________________________
//checkClues
//
//uses checkIfIn to see if the player is adjacent to a
//Wumpus, pit, or bats. Also checks for footprints
//______________________________________________________
void checkClues(int row, int col)
{
//____________________________________________________________
//Left
	if (col - 1 >= 0) {
		if (checkIfIn(row, col - 1, 'B') == true)
			cout << "You hear bats in the distance " << endl;

		if (checkIfIn(row, col - 1, 'P') == true)
			cout << "You feel a draft " << endl;

		if (checkIfIn(row, col - 1, 'W') == true)
			cout << "You smell something rank " << endl;
	}
//____________________________________________________________	

//____________________________________________________________
//Right
	if (col + 1 < 8) {
		if (checkIfIn(row, col + 1, 'B') == true)
			cout << "You hear bats in the distance " << endl;

		if (checkIfIn(row, col + 1, 'P') == true)
			cout << "You feel a draft " << endl;

		if (checkIfIn(row, col + 1, 'W') == true)
			cout << "You smell something rank " << endl;
	}
//____________________________________________________________

//____________________________________________________________
//Down
	if (row + 1 < 8) {
		if (checkIfIn(row + 1, col, 'B') == true)
			cout << "You hear bats in the distance " << endl;

		if (checkIfIn(row + 1, col, 'P') == true)
			cout << "You feel a draft " << endl;

		if (checkIfIn(row + 1, col, 'W') == true)
			cout << "You smell something rank " << endl;
	}
//____________________________________________________________

//____________________________________________________________
//Up
	if (row - 1 >= 0) {
		if (checkIfIn(row - 1, col, 'B') == true)
			cout << "You hear bats in the distance " << endl;

		if (checkIfIn(row - 1, col, 'P') == true)
			cout << "You feel a draft " << endl;

		if (checkIfIn(row - 1, col, 'W') == true)
		cout << "You smell something rank " << endl;
	}
//____________________________________________________________

	if (checkIfIn(row, col, 'X') == true)
		cout << "You see footprints in the dust " << endl;
}

//______________________________________________________
//moveWumpus
//
//Called when an arrow is shot and misses. Moves the 
//Wumpus to a random adjacent room, or leaves it where
//it is (random chance of each).
//______________________________________________________
void moveWumpus(int & wRow, int & wCol)
{
	remove(wRow, wCol, 'W');

	int num;
	int didMove = 0;
	

	while (didMove == 0) {
		num = rand() % 5; //random num decides which function is called
		
		if (num == 0) {
			if (wCol - 1 >= 0) {
				wCol--;
				maze[wRow][wCol] += "W";
				didMove = 1;
			}
		}
			if (num == 1) {
				if (wCol + 1 < 8) {
					wCol++;
					maze[wRow][wCol] += "W";
					didMove = 1;
				}
			}
			if (num == 2) {
				if (wRow - 1 >= 0) {
					wRow--;
					maze[wRow][wCol] += "W";
					didMove = 1;
				}
			}
			if (num == 3) {
				if (wRow + 1 < 8) {
					wRow++;
					maze[wRow][wCol] += "W";
					didMove = 1;
				}
			}
			if (num == 4) {
				maze[wRow][wCol] += "W";
				didMove = 1;
			}
		}
}

