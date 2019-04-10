// Applicatoin.cpp : 
// O(n^2)
// Memory N^2 Could also be done with an array[N] instead of array[N*N] but then performance O(N^3)
// Christina Kylili
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>      
using namespace std;
const int N = 8;

enum Directions {
	HorizontalLeft = 0,
	HorizontalRight = 1,
	VerticalUp = 2,
	VerticalDown = 3,
	DiagonalLeftUp = 4,
	DiagonalLeftDown = 5,
	DiagonalRightUp = 6,
	DiagonalRightDown = 7
};

struct Bowlies {
	int x = -1;
	int y = -1;
	bool isHappy = true;
	bool isOut = true;
	int hitMap[8] = { -1, -1, -1,-1, -1, -1 ,-1, -1 };
};

//Returns the position of the ball with cordinates (x,y) to the array
int ArrayPosition(int x, int y)
{
	return (x*N + y);
}

//Checks input of user based on arguments different things need to be checked
bool IsInvalidInput(int x, int y, Bowlies* array, bool checkFreePosition, bool checkUnhappyPosition)
{
	if ((x < 0) || (x >= N) || (y < 0) || (y >= N))
	{
		cout << "Position out of bounds. Try again" << endl;
		return true;
	}
	if (checkFreePosition)
		if (!array[ArrayPosition(x, y)].isOut)
		{
			cout << "Position already taken. Try again" << endl;
			return true;
		}
	if (checkUnhappyPosition)
		if (array[ArrayPosition(x, y)].isHappy)
		{
			cout << "Ball is not unhappy. Try again" << endl;
			return true;
		}

	return false;
}

bool IsInvalidDirection(int x, int y, Bowlies* array, int direction)
{
	if (direction < HorizontalLeft || direction > DiagonalRightDown)
	{
		cout << "Give one of the possible directions" << endl;
		return true;
	}

	if (array[ArrayPosition(x, y)].hitMap[direction] == -1)
	{
		cout << "You can hit any ball in that direction. Choose direction to move:" << endl;
		return true;
	}
	return false;
}

//Show board map
void ShowBoard(Bowlies* array)
{
	cout << "    ";
	for (int i = 0; i <= N - 1; i++) {
		cout << i << "    ";
	}
	cout << endl;
	for (int r = 0; r <= N - 1; r++)
	{
		cout << r << " :";
		for (int c = 0; c <= N - 1; c++)
		{
			if (array[ArrayPosition(r, c)].isOut)
			{
				cout << " --  ";
			}
			else if (!array[ArrayPosition(r, c)].isHappy)
				cout << " x   ";
			else
				cout << " 0   ";
		}
		cout << endl;
	}
}

//After an attack we need to discover again the unhappy balls cause other balls have probably also affected by the attack, the ones that they had in their hitMap the attacker
void ResetBallProperties(Bowlies* array)
{
	for (int i = 0; i <= N*N - 1; i++)
	{
		array[i].isHappy = true;
		for (int k = 0; k < 8; k++)
			array[i].hitMap[k] = -1;
	}
}

//Check if the bowlie in array[position] is unhappy.Also identify all the balls that the ball array[position] can hit.A ball can hit in maximum 8 balls
bool IsBowlieUnhappy(int position, Bowlies* array)
{
	bool retValue = false;
	for (int i = 0; i <= N - 1; i++)
	{
		int elementSameRow = ArrayPosition(array[position].x, i);	//positions for balls in the same row
		//Detect if the ball array[i] is on the horizontal left or horizontal right of the ball that we check
		if ((!array[elementSameRow].isOut) && (elementSameRow != position))
		{
			retValue = true;
			array[position].isHappy = false;
			if (position > elementSameRow)
			{
				array[position].hitMap[HorizontalLeft] = elementSameRow; //The last ball we find in the left of the 'position' ball is the one that we need to save
			}
			else
			{
				if (array[position].hitMap[HorizontalRight] == -1)	//The first ball we find in the right of the 'position' ball is the one that we need to save
				{
					array[position].hitMap[HorizontalRight] = elementSameRow;
				}
			}
		}

		int elementSameColumn = ArrayPosition(i, array[position].y); //positions for balls in the same column
		if ((!array[elementSameColumn].isOut) && (elementSameColumn != position))
		{
			retValue = true;
			array[position].isHappy = false;
			if (position > elementSameColumn)
			{
				array[position].hitMap[VerticalUp] = elementSameColumn;
			}
			if (position < elementSameColumn)
			{
				if (array[position].hitMap[VerticalDown] == -1)
				{
					array[position].hitMap[VerticalDown] = elementSameColumn;
				}
			}
		}

		//move in the right down direction (x+i,y+i)
		int elementRightDiagonalDown = ArrayPosition(array[position].x + i, array[position].y + i);
		if ((array[position].x + i <= N - 1) && (array[position].y + i <= N - 1))
		{
			if ((!array[elementRightDiagonalDown].isOut) && (i != 0))
			{
				array[position].isHappy = false;
				retValue = true;
				if (array[position].hitMap[DiagonalRightDown] == -1)
				{
					array[position].hitMap[DiagonalRightDown] = elementRightDiagonalDown;
				}
			}
		}

		//move in the left up direction (x-i,y-i)
		int elementLeftUp = ArrayPosition(array[position].x - i, array[position].y - i);
		if ((0 <= array[position].x - i) && (array[position].x - i <= N - 1) && (0 <= array[position].y - i) && (array[position].y - i <= N - 1))
		{
			if ((!array[elementLeftUp].isOut) && (i != 0))
			{
				array[position].isHappy = false;
				retValue = true;
				if (array[position].hitMap[DiagonalLeftUp] == -1)
				{
					array[position].hitMap[DiagonalLeftUp] = elementLeftUp;
				}
			}
		}

		//move in the left down direction (x+i,y-i)
		int elementLeftDiagonalDown = ArrayPosition(array[position].x + i, array[position].y - i);
		if ((array[position].x + i <= N - 1) && (0 <= array[position].y - i) && (array[position].y - i <= N - 1))
		{
			if ((!array[elementLeftDiagonalDown].isOut) && (i != 0))
			{
				array[position].isHappy = false;
				retValue = true;
				if (array[position].hitMap[DiagonalLeftDown] == -1) //only want to store the first one cause thats the closest
				{
					array[position].hitMap[DiagonalLeftDown] = elementLeftDiagonalDown;
				}
			}
		}

		//move in the left down direction (x-i,y+i)
		int elementRightUp = ArrayPosition(array[position].x - i, array[position].y + i);
		if ((0 <= array[position].x - i) && (array[position].x - i <= N - 1) && (0 <= array[position].y + i) && (array[position].y + i <= N - 1))
		{
			if ((!array[elementRightUp].isOut) && (i != 0))
			{
				retValue = true;
				array[position].isHappy = false;
				if (array[position].hitMap[DiagonalRightUp] == -1) //only want to store the first one cause thats the closest
				{
					array[position].hitMap[DiagonalRightUp] = elementRightUp;
				}
			}
		}
	}
	return retValue;
}

//One hit can cause a series of many hits:In the end of one attack there is always one ball out
void Attack(int attacker, Directions attackDirection, Bowlies* array)
{
	int victim;
	victim = array[attacker].hitMap[attackDirection];
	array[attacker].isOut = true;	//Clear this position

	if (victim != -1)
	{
		cout << "Ball (" << array[victim].x << "," << array[victim].y << ") was hit by (" << array[attacker].x << "," << array[attacker].y << ")"<<endl;
		Attack(victim, attackDirection, array);
		array[victim].isOut = false;			//The attacker takes the position of the victim
		//maybe this is not needed cause i do need to do identify unhappy but in case i dont do it then i will remove this
	}
	else //ball goes out of world because it can hit anyone and it has been hitted
	{
		return;
	}
}

int main()
{ 
	Bowlies *bowliesArray;
	bowliesArray = new Bowlies[N*N];

	srand(time(NULL));

	//Put N-1 balls in random positions
	for (int i = 0; i < N - 1; i++)
	{
		int x = rand() % N;
		int y = rand() % N;
		while (!bowliesArray[ArrayPosition(x, y)].isOut)//empty position so it can be added to the board i can also check if is false then it means it can be added
		{
			x = rand() % (N);
			y = rand() % (N);
		};
		std::cout << "x:" << x << " y:" << y << "-->" << "Added" << "\n\n";
		bowliesArray[ArrayPosition(x, y)].x = x;
		bowliesArray[ArrayPosition(x, y)].y = y;
		bowliesArray[ArrayPosition(x, y)].isOut = false;
	};

	//Print balls (na to kamo me ena loop)
	ShowBoard(bowliesArray);

	//User places a new ball
	int x, y;
	cout << "Give x position of new bowlie(0 to N-1):";
	cin >> x;
	cout << "Give y position of new bowlie(0 to N-1):";
	cin >> y;
	while (!cin || IsInvalidInput(x, y, bowliesArray, true, false))
	{
		cin.clear(); // reset failbit
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
		cout << "Give x position of new bowlie(0 to N-1):";
		cin >> x;
		cout << "Give y position of new bowlie(0 to N-1):";
		cin >> y;
	}
	bowliesArray[ArrayPosition(x, y)].x = x;
	bowliesArray[ArrayPosition(x, y)].y = y;
	bowliesArray[ArrayPosition(x, y)].isOut = false;

	ShowBoard(bowliesArray);

	//Identify not happy bowlies
	int unhappyCount = 0;
	for (int i = 0; i <= N*N; i++)
	{
		if (!bowliesArray[i].isOut)
		{
			if (IsBowlieUnhappy(i, bowliesArray))
				unhappyCount++;
		}
	}


	cout << "Unhappy balls = X" << endl;
	ShowBoard(bowliesArray);

	int direction;
	while (unhappyCount != 0)
	{
		unhappyCount = 0;

		//Choose unhappy ball
		cout << "Give x position of unhappy ball (0 to N-1):";
		cin >> x;
		cout << "Give y position of unhappy ball (0 to N-1):";
		cin >> y;

		while (!cin || IsInvalidInput(x, y, bowliesArray, false, true))
		{
			cin.clear(); // reset failbit
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
			cout << "Give x position of unhappy ball (0 to N-1):";
			cin >> x;
			cout << "Give y position of unhappy ball (0 to N-1):";
			cin >> y;
		}

		cout << "Choose direction to move:" << endl;
		cout << "0: Horizontal Left" << endl;
		cout << "1: Horizontal Right" << endl;
		cout << "2: Vertical Up " << endl;
		cout << "3: Vertical Down " << endl;
		cout << "4: Diagonal Left Up" << endl;
		cout << "5: Diagonal Left Down" << endl;
		cout << "6: Diagonal Right Up" << endl;
		cout << "7: Diagonal Right Down" << endl;
		cin >> direction;

		while (!cin || IsInvalidDirection(x, y, bowliesArray, direction))
		{
			cin.clear(); // reset failbit
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
			cout << "Give one of the possible directions" << endl;
			cin >> direction;
		}

		Attack(ArrayPosition(x, y), (Directions)direction, bowliesArray);
		ResetBallProperties(bowliesArray);
		for (int r = 0; r <= N*N; r++)
		{
			if (!bowliesArray[r].isOut)
			{
				if (IsBowlieUnhappy(r, bowliesArray))
					unhappyCount++;
			}
		}
		ShowBoard(bowliesArray);
	}
	cout << "================================= GAME OVER ===================================================";
	int i;
	cin >> i;
	return 0;
}
