#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>

#include "helpers.h"
#include "player.h"

using namespace std;


// Getters section
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


// Setters section
void Player::setName(string& name)
{
	playerName = name;
}


void Player::setScore(int score)
{
	playerScore = score;
}


void Player::setBullAccuracy(unsigned int bull_accuracy)
{
	bullAccuracy = bull_accuracy;;
}


void Player::setSingleAccuracy(unsigned int single_accuracy)
{
	singleAccuracy = single_accuracy;
}


void Player::setAllAccuracy(unsigned int all_accuracy)
{
	/*This function takes a single value for the
 	 * accuracy as an argument and from that calculates
 	 * a fair and balanced value for both the single and
 	 * bull accuracy.*/

	if (all_accuracy <= 99 && all_accuracy > 89) {
		bullAccuracy = all_accuracy - 10;
		singleAccuracy = all_accuracy - 4;

	} else if (all_accuracy <= 89 && all_accuracy > 79) {
		bullAccuracy = all_accuracy - 7;
		singleAccuracy = all_accuracy - 3;

	} else if (all_accuracy <= 79 && all_accuracy > 64) {
		bullAccuracy = all_accuracy - 5;
		singleAccuracy = all_accuracy - 2;

	} else if (all_accuracy <= 64 && all_accuracy > 50) {
		bullAccuracy = all_accuracy - 3;
		singleAccuracy = all_accuracy -1;

	} else {
		bullAccuracy = all_accuracy;
		singleAccuracy = all_accuracy;

	}
}


// Throw functions
int Player::bull(const vector<int>& theDartboard)
{
	int randomization = (rand() % 100);
	
	// Sets the range between which it is possible to hit the outer bull
	int forTheOuter = getBullAccuracy() + 4; 

	if (randomization <= getBullAccuracy()) {
		return 50;

	} else if (randomization > getBullAccuracy() && randomization <= (getBullAccuracy() + forTheOuter)) {

		/* Split the remaining possibilities, depending on the accuracy, in two:
 		 * outer-bull (25) or a random target on the dartboard */
 		 
		return 25;

	} else {
		int dartPosRand = (rand() % theDartboard.size());
		return theDartboard[dartPosRand];

	}
}


int Player::singleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int target)
{
	int randomization = (rand() % 100);

	/* Only the outer-bull is considered in this case because if the player/AI wants to score 50/bull
	 * it has to use the appropriate function = bullThrow() */
	if (target == 25){
		if (randomization <= getSingleAccuracy() + 4){
			return 25;

		} else {
			int half = (100 - getSingleAccuracy()) / 2; 

			/* For the remaining hit cases:
 			 * Take the number of values between 100 (max accuracy) and the single accuracy,
 			 * split it in half to have an equal probability of hitting bull or another target */
			if (randomization > getSingleAccuracy() && randomization <= (getSingleAccuracy() + half)) { 
				return 50;

			} else { 
				//and the other interval for the rest of board numbers
				int dartPosRand = (rand() % theDartboard.size());
				return theDartboard[dartPosRand];

			}
		}

	} else if (randomization <= getSingleAccuracy()) {
		return target;
	} else {
		int aFourth = (100 - getSingleAccuracy()) / 4; 

		/* Equally split the probabilities of success among the 4 target options remaining:
 		 * Single on the right, on the left, double above and treble below.*/

		if (randomization > getSingleAccuracy() && randomization <= (getSingleAccuracy() + aFourth)) {
			// First alternative possibility: hit the area on the left of the target
			bIter = move_pos(theDartboard, target, 'l');

			return *bIter;
		} else if (randomization > (getSingleAccuracy() + aFourth) && randomization <= (getSingleAccuracy() + 2 * aFourth)) {
			// Second alternative possibility: hit the target on the right.
			bIter = move_pos(theDartboard, target, 'r');
			
			return *bIter;
		} else if (randomization > (getSingleAccuracy() + 2 * aFourth) && randomization <= (getSingleAccuracy() + 3 * aFourth)) {
			// Third alternative possibility: hit the target above (double the initial target)
			return target * 2;
		} else {
			// Fourth alternative possibility: hit the target below (treble the initial target)
			return target * 3;
		}
	}
}

int Player::doubleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int target)
{
	int randomization = (rand() % 100);

	// Used to determine the remaining hit cases
	int aThird = (100 - getSingleAccuracy()) / 3; 

	if (randomization <= getSingleAccuracy()) {
		return target * 2; // The requested target has been hit

	} else if (randomization > getSingleAccuracy() && randomization <= (getSingleAccuracy() + aThird)) {
		return target; // Hits single

	}
	else if (randomization > (getSingleAccuracy() + aThird) && randomization <= (getSingleAccuracy() + 2 * aThird)) {
		bIter = move_pos(theDartboard, target, 'l');

		return *bIter * 2; // Hits the double on the left

	} else {
		bIter = move_pos(theDartboard, target, 'r');
		
		return *bIter * 2; // Hits double on the right
	}
}


int Player::trebleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int target)
{
	int randomization = (rand() % 100);
	int aFourth = (100 - getSingleAccuracy()) / 4; // Determines the 4 remaining hit cases

	if (randomization <= getSingleAccuracy()) {
		return target * 3; // Hits correct treble target

	} else if (randomization > getSingleAccuracy() && randomization <= (getSingleAccuracy() + 2 * aFourth)) { 
		// This hit case is bigger because there are two possibilities to hit a single
		return target; // Hits single

	} else if (randomization > (getSingleAccuracy() + 2 * aFourth) \
		&& randomization <= (getSingleAccuracy() + 3 * aFourth)) {
		bIter = move_pos(theDartboard, target, 'l');

		return *bIter * 3; //hits the treble at the left

	} else {
		bIter = move_pos(theDartboard, target, 'r');

		return *bIter * 3; //hits treble at the right

	}
}

Player::Player()	
{
	// Player stats
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
