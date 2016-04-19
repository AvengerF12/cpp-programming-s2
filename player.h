#pragma once
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Player
{
private:
	string playerName;
	int bullAccuracy;
	int singleAccuracy;
	int playerScore;

public:
	int gamesWon;
	int setsWon;
	int matchesWon;
	int nTurns;

	//getters
	string& getName();
	int& getScore();
	int& getBullAccuracy();
	int& getSingleAccuracy();
	
	//setters
	void setName(string& name);
	void setScore(int& score);
	void setBullAccuracy(unsigned int& bull_accuracy);
	void setSingleAccuracy(unsigned int& single_accuracy);
	void setAllAccuracy(unsigned int& all_accuracy);

	//throw functions
	int bull(const vector<int>& theDartboard);
	int singleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int& target);
	int doubleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int& target);
	int trebleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int& target);

	Player();
	~Player();
};

