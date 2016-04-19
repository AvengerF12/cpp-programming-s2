#include "player.h"
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

// getters
string& Player::getName()
{
	return playerName;
}

int& Player::getScore()
{
	return playerScore;
}

int& Player::getBullAccuracy()
{
	return bullAccuracy;
}

int& Player::getSingleAccuracy() 
{
	return singleAccuracy;
}

// setters
void Player::setName(string& name)
{
	playerName = name;
}

void Player::setScore(int& score)
{
	playerScore = score;
}

void Player::setBullAccuracy(unsigned int& bull_accuracy)
{
	bullAccuracy = bull_accuracy;
}

void Player::setSingleAccuracy(unsigned int& single_accuracy)
{
	singleAccuracy = single_accuracy;
}


void Player::setAllAccuracy(unsigned int & all_accuracy)
{
	if (all_accuracy <= 99 && all_accuracy > 89)
	{
		bullAccuracy = all_accuracy - 10;
		singleAccuracy = all_accuracy - 4;
	}
	else if (all_accuracy <= 89 && all_accuracy > 79)
	{
		bullAccuracy = all_accuracy - 7;
		singleAccuracy = all_accuracy - 3;
	}
	else if (all_accuracy <= 79 && all_accuracy > 64)
	{
		bullAccuracy = all_accuracy - 5;
		singleAccuracy = all_accuracy - 2;
	}
	else if (all_accuracy <= 64 && all_accuracy > 50)
	{
		bullAccuracy = all_accuracy - 3;
		singleAccuracy = all_accuracy -1;
	}
	else
	{
		bullAccuracy = all_accuracy;
		singleAccuracy = all_accuracy;
	}
}

//throw functions
int Player::bull(const vector<int>& theDartboard)
{
	int randomization = (rand() % 100);
	int forTheOuter = getBullAccuracy() + 4; //interval setter for outer and thus remaining hit cases

	if (randomization <= getBullAccuracy())
	{
		return 50;
	}
	else if (randomization > getBullAccuracy() && randomization <= (getBullAccuracy() + forTheOuter)) 
		//remaining accuracy % is split and considered as an one interval
	{	//for hitting outer bull																					  
		return 25;
	}
	else //and the other interval for the rest of board numbers
	{
		int dartPosRand = (rand() % theDartboard.size());
		return theDartboard[dartPosRand];
	}
}

int Player::singleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int& target)
{
	int randomization = (rand() % 100);

	if (target == 25)
	{
		if (randomization <= getSingleAccuracy() + 4)
		{
			return 25;
		}
		else
		{
			int half = (100 - getSingleAccuracy()) / 2; //for remaining hit cases
			if (randomization > getSingleAccuracy() && randomization <= (getSingleAccuracy() + half)) //remainder accuracy % split and considered as an one interval
			{	//for hitting the bull
				return 50;
			}
			else //and the other interval for the rest of board numbers
			{
				int dartPosRand = (rand() % theDartboard.size());
				return theDartboard[dartPosRand];
			}
		}
	}
	else if (randomization <= getSingleAccuracy())
	{
		return target;
	}
	else
	{
		int aFourth = (100 - getSingleAccuracy()) / 4; //for remaining hit cases
		if (randomization > getSingleAccuracy() && randomization <= (getSingleAccuracy() + aFourth)) //first alternative interval
		{
			bIter = find(theDartboard.begin(), theDartboard.end(), target);
			if (bIter == theDartboard.begin())
			{
				bIter = theDartboard.end() - 1; //exception if the chosen number is the first of the vector
			}
			else
			{
				bIter--; //makes sure to hit the dartboard number left of chosen one
			}
			return *bIter;
		}
		else if (randomization > (getSingleAccuracy() + aFourth) && randomization <= (getSingleAccuracy() + 2 * aFourth)) //second alternative interval
		{
			bIter = find(theDartboard.begin(), theDartboard.end(), target);
			if (bIter == (theDartboard.end() - 1))
			{
				bIter = theDartboard.begin(); //exception if the chosen number is the last of the vector
			}
			else
			{
				bIter++; //makes sure to hit the dartboard number right of chosen one
			}
			return *bIter;
		}
		else if (randomization > (getSingleAccuracy() + 2 * aFourth) && randomization <= (getSingleAccuracy() + 3 * aFourth)) //third alternative interval
		{
			return target * 2; //hits the double of the intended number
		}
		else
		{
			return target * 3; //hits the treble of the intended number
		}
	}
}

int Player::doubleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int & target)
{
	int randomization = (rand() % 100);
	int aThird = (100 - getSingleAccuracy()) / 3; //for remaining hit cases

	if (randomization <= getSingleAccuracy())
	{
		return target * 2; //hits correct double
	}
	else if (randomization > getSingleAccuracy() && randomization <= (getSingleAccuracy() + aThird)) //first alternative interval
	{
		return target; //hits single
	}
	else if (randomization > (getSingleAccuracy() + aThird) && randomization <= (getSingleAccuracy() + 2 * aThird)) //second alternative interval
	{
		bIter = find(theDartboard.begin(), theDartboard.end(), target);
		if (bIter == theDartboard.begin())
		{
			bIter = theDartboard.end() - 1; //exception if the chosen number is the first of the vector
		}
		else
		{
			bIter--; //makes sure to hit the dartboard number left of chosen one
		}
		return *bIter * 2; //hits the double at the left
	}
	else
	{
		bIter = find(theDartboard.begin(), theDartboard.end(), target); //third alternative interval
		if (bIter == (theDartboard.end() - 1))
		{
			bIter = theDartboard.begin();
		}
		else
		{
			bIter++; //makes sure to hit the dartboard number right of chosen one
		}
		return *bIter * 2; //hits double at the right
	}
}

int Player::trebleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int & target)
{
	int randomization = (rand() % 100);
	int aFourth = (100 - getSingleAccuracy()) / 4; //for remaining hit cases

	if (randomization <= getSingleAccuracy()) //check later if maybe some other accuracy coefficient
	{
		return target * 3; //hits correct treble
	}
	else if (randomization > getSingleAccuracy() && randomization <= (getSingleAccuracy() + 2 * aFourth)) //first alternative interval, bigger than usual
	{																									  //because double occasion to hit single
		return target; //hits single
	}
	else if (randomization > (getSingleAccuracy() + 2 * aFourth) && randomization <= (getSingleAccuracy() + 3 * aFourth)) //second alternative interval
	{
		bIter = find(theDartboard.begin(), theDartboard.end(), target);
		if (bIter == theDartboard.begin())
		{
			bIter = theDartboard.end() - 1; //exception if the chosen number is the first of the vector
		}
		else
		{
			bIter--; //makes sure to hit the dartboard number left of chosen one
		}
		return *bIter * 3; //hits the treble at the left
	}
	else
	{
		bIter = find(theDartboard.begin(), theDartboard.end(), target); //third alternative interval
		if (bIter == (theDartboard.end() - 1))
		{
			bIter = theDartboard.begin(); //exception if the chosen number is the last of the vector
		}
		else
		{
			bIter++; //makes sure to hit the dartboard number right of chosen one
		}
		return *bIter * 3; //hits treble at the right
	}
}

Player::Player()	
{
	playerScore = 501;
	bullAccuracy = 0;
	singleAccuracy = 0;
	gamesWon = 0;
	setsWon = 0;
	matchesWon = 0;
	nTurns = 3;
}

Player::~Player()
{

}
