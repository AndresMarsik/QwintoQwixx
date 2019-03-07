//only define once
#ifndef CSI2372_PROJECT_PLAYER_H
#define CSI2372_PROJECT_PLAYER_H

//includes
#include <iostream>
#include <string>
#include <sstream>
#include "Scoresheet.h""

//use std namespace
using namespace std;

//-------------------------------------- PLAYER ----------------------------------------------
class Player {
protected:
	//flag for active user
	bool isActive = false;
public:
	//abstract methods to implement in child classes
	virtual void inputBeforeRoll(RollOfDice& roll) = 0;
	virtual bool inputAfterRoll(RollOfDice& roll) = 0;
	virtual int total() = 0;

};

//----------------------------------------- QWINTO PLAYER ---------------------------------------

class QwintoPlayer : public Player {
	//pointer to hold sheet
	QwintoScoreSheet* sheet;
public:
	QwintoPlayer(string& name);
	void inputBeforeRoll(RollOfDice& roll);
	bool inputAfterRoll(RollOfDice& roll);
	int total();
};

//--------------------------------------------------QUIXX PLAYER ---------------------------------------------

class QwixxPlayer : public Player {
	//pointer to hold sheet
	QwixxScoreSheet* sheet;
protected:
	//static dice to use for each user
	static Dice* white1;
	static Dice* white2;
	static Dice* red;
	static Dice* yellow;
	static Dice* green;
	static Dice* blue;
public:
	QwixxPlayer(string& name);
	void inputBeforeRoll(RollOfDice& roll);
	bool inputAfterRoll(RollOfDice& roll);
	int total();
};

#endif //CSI2372_PROJECT_PLAYER_H