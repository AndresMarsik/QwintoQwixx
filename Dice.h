//only define once
#ifndef CSI2372_PROJECT_DICE_H
#define CSI2372_PROJECT_DICE_H

//includes
#include <random>
#include <vector>
#include <iostream>

//use std namespace
using namespace std;

//enum class for colour
enum class Colour { RED, YELLOW, GREEN, BLUE, WHITE };

//RandomDice static helper class
class RandomDice {
	static default_random_engine rdGenerator;
	static uniform_int_distribution<int> distribution;
public:
	static int getRoll();
};

//Class for a single dice of a certain colour
class Dice {
	//overload insertion operator for ost output
	friend ostream& operator<< (ostream& stream, Dice const& d);
public:
	Colour colour;
	int faceValue;
	int roll();
	explicit Dice(Colour colour);
};

//container to store multiple Dice objects for ease of use
class RollOfDice {
	//overload insertion operator for ost output
	friend ostream& operator<<(ostream& stream, RollOfDice const& rod);
	vector<Dice> dice;
public:
	void addDie(Dice d);
	void roll();
	RollOfDice pair(Colour diceColour1, int numInSeq1, Colour diceColour2, int numInSeq2);
	//overload integer conversion operator
	operator int() const;
	//required for range looping
	vector<Dice>::iterator begin();
	vector<Dice>::iterator end();
	vector<Dice>::const_iterator begin() const;
	vector<Dice>::const_iterator end() const;
	bool hasColour(Colour c);
	void empty();
};

#endif //CSI2372_PROJECT_DICE_H
