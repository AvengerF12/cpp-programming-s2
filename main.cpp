#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip> //precision settings
#include <limits>
//#include <conio.h> //for kbhit()
//#include <Windows.h>

#include "player.h"

using namespace std;

int optimal_throw(Player& current, vector<int> doublesLeaveDoubles, vector<int> dartboard);
int start_simulation(vector<Player>& player, vector<int>& scoreCombinations, vector<int>& dartboard, vector<int>& doublesLeaveDoubles, vector<Player>::iterator& modIter);

const int TOTAL_MATCHES = 10, MAX_THROWS_TURN = 3;

int main()
{
	unsigned int bAccuracy = 0, sAccuracy = 0;
	string playerName = "";
	vector<int> dartboard { 20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5 };
	vector<int>::const_iterator bIter;
	vector<int> doublesLeaveDoubles { 40, 36, 32, 28, 24, 20, 16, 12, 8, 4 }; 
	vector<int>::const_iterator dLd;

	vector<int> scoreCombinations(14); //stores all the possible Joe:Sid set winning combinations: 7:6, 6:7, 5:7, 4:7, 3:7, 2:7, 1:7 and viceversa
	vector<string> stringCombinations {"7:0", "7:1", "7:2", "7:3", "7:4", "7:5", "7:6", \
						"6:7", "5:7", "4:7", "3:7", "2:7", "1:7", "0:7"};

	vector<Player> player(2);
	vector<Player>::iterator modIter;

	for (unsigned int i = 0; i < player.size(); i++) //setting players' names
	{
		cout << "Player " << (i + 1) << " name is: ";
		cin >> playerName;
		player[i].setName(playerName);
	}

	cout << "Would you prefer to manually set the accuracy values, set a master accuracy, or choose it from a randomization? (1, 2) ";
	int accChoice = 0;
	while (true) {
		cin >> accChoice;
		
		switch (accChoice) {
			case 1:
				cout << "\nSet the players' bull accuracy:" << endl;
				for (modIter = player.begin(); modIter != player.end(); modIter++){
					cout << modIter->getName() << ": ";
					cin >> bAccuracy;
					modIter->setBullAccuracy(bAccuracy);
				}

				cout << "\nSet the players' single hit accuracy: " << endl;
				for (modIter = player.begin(); modIter != player.end(); modIter++) {
					cout << modIter->getName() << ": ";
					cin >> sAccuracy;
					modIter->setSingleAccuracy(sAccuracy);
				}
				break;

			case 2:
				cout << "\nSet the players' master accuracy" << endl;
				for (modIter = player.begin(); modIter != player.end(); modIter++) {
					cout << modIter->getName() << ": ";
					cin >> bAccuracy;
					modIter->setAllAccuracy(bAccuracy);
				}
				break;

			default:
				cout << "Wrong number inserted. Please try again: ";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				accChoice = 0;
				break;
			}

		if(accChoice == 1 || accChoice == 2) {
			break;
		}
		
	}
	
	//decision of player that begins game
	cout << "\nThe players will now attempt a 50:50 game to decide who will start first: " << endl;
	while (true)
	{
		modIter = player.begin();
		int tempResult1 = modIter->bull(dartboard);
		modIter++;
		int tempResult2 = modIter->bull(dartboard);

		if (tempResult1 == tempResult2)
		{
			continue;
		}
		else if (tempResult1 > tempResult2)
		{
			modIter = player.begin();
			cout << modIter->getName() << " will start. " << endl;
			break;
		}
		else
		{
			cout << modIter->getName() << " will start. " << endl;
			break;
		}
	}

	char simYN = '\0';
	while (true)
	{
		cout << "Start simulation? (y/n) ";
		cin >> simYN;
		if (simYN == 'y' || simYN == 'Y')
		{
			start_simulation(player, scoreCombinations, dartboard, doublesLeaveDoubles, modIter);
			break; //stops the simulation while(true) loop, after all matches are concluded
		}
		else if (simYN == 'n' || simYN == 'N')
		{
			cout << "Ok then, not yet. Have some time to prepare for it." << endl;
		}
		else
		{
			cout << "Wrong key pressed." << endl;
		}
	}

	//results and percentages output
	cout << "\nSimulation ended. Players' results:" << endl;

	for(unsigned int i=0;i<player.size();i++){
		cout << player[i].getName() << ": " << player[i].matchesWon << " matches won." << endl;
	}

	cout << "\nJoe:Sid\t\tFrequency" << endl;

	for(unsigned int i=0;i<scoreCombinations.size();i++){
		cout << string(2, ' ') << stringCombinations[i] << "\t\t" << string(3, ' ') << setprecision(5) << (((float)scoreCombinations[i]) / TOTAL_MATCHES) * 100 << "%" << endl;
	}

	return 0;
}


int optimal_throw(Player& current, vector<int> doublesLeaveDoubles, vector<int> dartboard)
{
	vector<int>::const_iterator dLd;
	vector<int>::const_iterator bIter;

	int tempCalc = 0;
	unsigned int targetNo = 0;

	for (dLd = doublesLeaveDoubles.begin(); dLd != doublesLeaveDoubles.end(); dLd++) //if score is equal to a double-leaving-double, goes for THAT first
	{
		if (current.getScore() == (*dLd)) {
			targetNo = (*dLd) / 2;
			tempCalc = (current.getScore() - current.doubleThrow(dartboard, bIter, targetNo));
			current.setScore(tempCalc);
			cout << "Double throw attempted on double leaving double to end" << endl;
			
			return targetNo;
		}
	}

	for (bIter = dartboard.begin(); bIter != dartboard.end(); bIter++)
	{
									

		if (current.getScore() == ((*bIter) * 2)) //if not then check if score is equal to ANY of the doubles, go for THAT shot to end the game
		{
			targetNo = *bIter;
			tempCalc = (current.getScore() - current.doubleThrow(dartboard, bIter, targetNo));
			current.setScore(tempCalc);
			cout << "Normal double throw attempted to end" << endl;

			return targetNo;
		}
	}

	if (current.getScore() == 50) //if able to win with a bull, go for that shot
	{
		tempCalc = (current.getScore() - current.bull(dartboard));
		current.setScore(tempCalc);
		cout << "Bull attempted" << endl;
	}
	
	if (current.getScore() > 50)
	{
		for (bIter = dartboard.begin(); bIter != dartboard.end(); bIter++) 
		{
			for (dLd = doublesLeaveDoubles.begin(); dLd != doublesLeaveDoubles.end(); dLd++) //checks if there's any double of treble possible that will leave
			{	 //a double-leaving-double (most efficient even in case of fail) to end
				if (current.getScore() == (((*bIter) * 3) + (*dLd)))
				{
					targetNo = *bIter;
					tempCalc = (current.getScore() - current.trebleThrow(dartboard, bIter, targetNo));
					current.setScore(tempCalc);
					cout << "Triple throw attempted to leave double leaving double" << endl;

					return targetNo;
				}
				else if (current.getScore() == (((*bIter) * 2) + (*dLd)))
				{
					targetNo = *bIter;
					tempCalc = (current.getScore() - current.doubleThrow(dartboard, bIter, targetNo));
					current.setScore(tempCalc);
					cout << "Double throw attempted to leave double leaving double" << endl;

					return targetNo;
				}
			}

			if (current.getScore() == (((*bIter) * 3) + 50)) //checks then if there's any double or treble possible that will leave
			{  //a bull to win the game, and goes for that (for question 2, 3 & 4 in adv AI)
				targetNo = *bIter;
				tempCalc = (current.getScore() - current.trebleThrow(dartboard, bIter, targetNo));
				current.setScore(tempCalc);
				cout << "Triple throw attempted to leave a bull" << endl;

				return targetNo;
			}
			else if (current.getScore() == (((*bIter) * 2) + 50))
			{
				targetNo = *bIter;
				tempCalc = (current.getScore() - current.doubleThrow(dartboard, bIter, targetNo));
				current.setScore(tempCalc);
				cout << "Double throw attempted to leave a bull" << endl;
				return targetNo;
			}
		}
	}
	
	if (current.getScore() >= 62) //in all the other cases, until down to this score, go only for the highest treble (3x20 - 60)
	{
		targetNo = 20; //additional step; because of reference, function argument needs to be a variable, not just literal "20"
		tempCalc = (current.getScore() - current.trebleThrow(dartboard, bIter, targetNo));
		current.setScore(tempCalc);
		cout << "Normal triple throw attempted" << endl;
	}
	else if (current.getScore() < 62 && current.getScore() > 50) //goes for the single throw equal to the score difference, 
	{	//thus it can try to end with a bull at next throw
		targetNo = current.getScore() - 50;
		tempCalc = (current.getScore() - current.singleThrow(dartboard, bIter, targetNo));
		current.setScore(tempCalc);
		cout << "Attempted throw difference (62 - 50) from bull" << endl;
	}
	else if (current.getScore() < 50)
	{
		for (dLd = doublesLeaveDoubles.begin(); dLd != doublesLeaveDoubles.end(); dLd++) //whenever under 50, whatever number of throws left, always better to get close
		{	 //to a double that leaves a double (in case of failure), and go for that one
			for (int j = 1; j <= 9; j++)
			{
				if (current.getScore() == ((*dLd) + j)) //calculates score difference from every double and aims for 
				{ 	//that difference in order to have afterwards a score equal to a double
					targetNo = j;
					tempCalc = (current.getScore() - current.singleThrow(dartboard, bIter, targetNo));
					current.setScore(tempCalc);
					cout << "Attempted difference (1-9) to leave double leaving double" << endl;
					dLd = doublesLeaveDoubles.end() - 1; //in order to end outer loop immediately
					break;
				}
			}
		}
								
		if (current.getScore() == 3) //when score is under 4, no more doubles leaving doubles, must only end with double 1
		{
			targetNo = 1;
			tempCalc = (current.getScore() - current.singleThrow(dartboard, bIter, targetNo));
			current.setScore(tempCalc);
			cout << "Attempted throwing 1 because score under 4" << endl;
		}
	}

	return 0;
}


int start_simulation(vector<Player>& player, vector<int>& scoreCombinations, vector<int>& dartboard, vector<int>& doublesLeaveDoubles, vector<Player>::iterator& modIter) {

	int tempScore = 0;
	unsigned int targetNo = 0;

	for (int i = 0; i < TOTAL_MATCHES; i++)
	{
		player[0].setsWon = 0;
		player[1].setsWon = 0;
		while (player[0].setsWon < 7 && player[1].setsWon < 7) // When any player reaches 7 sets won in one match (can't lose the match anymore), the loop breaks
		{
			player[0].gamesWon = 0;
			player[1].gamesWon = 0;
			while (player[0].gamesWon < 3 && player[1].gamesWon < 3) //when any player reaches 3 games won in a set of 5 (can't lose the set anymore), the loop breaks
			{
				//real game begins
				for (; modIter <= player.end(); modIter++) //iterator position doesen't change, so same player who won the 50:50 will start, then keep alternating
				{
					if (modIter == player.end()) //if the end of the vector was reached, start again
					{
						modIter = player.begin(); 
					}
							
					tempScore = modIter->getScore(); //saves current score in case any of the 3 shots is invalid
					while (modIter->getScore() >= 2 && modIter->nTurns != 0)
					{
						targetNo = optimal_throw(*modIter, doublesLeaveDoubles, dartboard);	
															
						cout << modIter->getName() << "'s just scored " << targetNo << endl;
						cout << modIter->getName() << "'s score is: " << modIter->getScore() << endl;
						cout << modIter->getName() << "'s throws left: " << (modIter->nTurns) - 1 << endl;
						cout << endl;

						if (modIter->getScore() == 0) //if current player won, update games won, exits the game, next player will start
						{
							modIter->gamesWon++;
							break;
						}
								modIter->nTurns--;
					}
					if (modIter->getScore() == 0)
					{
						int resetScore = 501; //scores get restored
						player[0].setScore(resetScore);
						player[1].setScore(resetScore);
						break;
					}
					else if (modIter->getScore() < 2 && modIter->getScore() != 0)
					{
						modIter->setScore(tempScore); //resets the previously held score because score got under 2
						modIter->nTurns = MAX_THROWS_TURN;
					}
					else
					{
						modIter->nTurns = MAX_THROWS_TURN;
					}
				}
				modIter++;
			}
			if (player[0].gamesWon == 3)
			{
				player[0].setsWon++;
			}
			else
			{
				player[1].setsWon++;
			}

			if (modIter == player.end())
			{
				modIter = player.begin(); //if the end of the vector was reached, start again
			}
		}
		if (player[0].setsWon == 7)
		{
			player[0].matchesWon++;
		}
		else
		{
			player[1].matchesWon++;
		}


		//counters for the sets winning percentage
		if (player[0].setsWon == 7 && player[1].setsWon == 0){
			scoreCombinations[0]++;
		}
		else if (player[0].setsWon == 7 && player[1].setsWon == 1){
			scoreCombinations[1]++;
		}
		else if (player[0].setsWon == 7 && player[1].setsWon == 2){
			scoreCombinations[2]++;
		}
		else if (player[0].setsWon == 7 && player[1].setsWon == 3){
			scoreCombinations[3]++;
		}
		else if (player[0].setsWon == 7 && player[1].setsWon == 4){
			scoreCombinations[4]++;
		}
		else if (player[0].setsWon == 7 && player[1].setsWon == 5){
			scoreCombinations[5]++;
				}
		else if (player[0].setsWon == 7 && player[1].setsWon == 6){
			scoreCombinations[6]++;
		}
		else if (player[0].setsWon == 6 && player[1].setsWon == 7){
			scoreCombinations[7]++;
		}
		else if (player[0].setsWon == 5 && player[1].setsWon == 7){
			scoreCombinations[8]++;
		}
		else if (player[0].setsWon == 4 && player[1].setsWon == 7){
			scoreCombinations[9]++;
		}
		else if (player[0].setsWon == 3 && player[1].setsWon == 7){
			scoreCombinations[10]++;
		}
		else if (player[0].setsWon == 2 && player[1].setsWon == 7){
			scoreCombinations[11]++;
		}
		else if (player[0].setsWon == 1 && player[1].setsWon == 7){
			scoreCombinations[12]++;
		}
		else if (player[0].setsWon == 0 && player[1].setsWon == 7){
			scoreCombinations[13]++;
		}
	}

	return 0;
}

