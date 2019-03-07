//includes
#include "Dice.h"
#include <random>
#include <iostream>
#include <chrono>

//use standard namespace
using namespace std;

//define members for RandomDice (static and private)
default_random_engine RandomDice::rdGenerator(chrono::system_clock::now().time_since_epoch().count());
uniform_int_distribution<int> RandomDice::distribution(1, 6);

//public method for generating a random roll (in range 1 - 6)
int RandomDice::getRoll() {
	return (distribution(rdGenerator));
}

//Rolls a single dice and returns the result of the roll
int Dice::roll() {
	faceValue = RandomDice::getRoll();
	return faceValue;
}

//Constructor for Dice object - overrides default: Dice must be
//created with an enum-specified colour
Dice::Dice(Colour colour) : colour(colour) { faceValue = 0; }

//Overload insertion operator for a single Dice object
ostream &operator<<(ostream &stream, Dice const &d) {
	switch (d.colour) {
	case Colour::RED:
		stream << "RED: ";
		break;
	case Colour::BLUE:
		stream << "BLUE: ";
		break;
	case Colour::GREEN:
		stream << "GREEN: ";
		break;
	case Colour::WHITE:
		stream << "WHITE: ";
		break;
	case Colour::YELLOW:
		stream << "YELLOW: ";
		break;
	}

	stream << d.faceValue;
	return stream;
}

//Adds a single dice to a given RollOfDice container
void RollOfDice::addDie(Dice d) {
	dice.push_back(d);
}

//Rolls all the Dice in the container
void RollOfDice::roll() {
	for (auto &iter : dice) {
		iter.roll();
	}
}

//Dice in roll of dice are printed in order to the user
//therefore the dice will be selected by colour and order:
//e.g. for the output (RED,3),(WHITE,5),(WHITE,1),...
//the user may specify to pair the second white dice with the red
//and RollOfDice::pair(Colour::RED,1,Colour::RED,2) may be called accordingly.
//Assumes that both colour dice selected in the arguments exist in the current roll
RollOfDice RollOfDice::pair(Colour diceColour1, int numInSeq1, Colour diceColour2, int numInSeq2) {
	//error check counters:
	if (numInSeq1 < 1) numInSeq1 = 1;
	if (numInSeq2 < 1) numInSeq2 = 1;

	//construct empty rod to return
	RollOfDice rd;

	//search for specified dice in current roll
	Dice* d1 = nullptr;
	for (auto &d : dice) {
		if (d.colour == diceColour1 && numInSeq1>0) {
			d1 = &d;
			--numInSeq1;
		}
	}
	Dice* d2 = nullptr;
	for (auto &d : dice) {
		if (d.colour == diceColour2 && numInSeq2>0) {
			d2 = &d;
			--numInSeq2;
		}
	}

	//add the chosen dice to the new RollOfDice object
	rd.addDie(*d1);
	rd.addDie(*d2);
	return rd;
}

//check if a roll of dice has a certain colour (mainly for debugging)
bool RollOfDice::hasColour(Colour c) {
	for (Dice d : dice) {
		if (d.colour == c) return true;
	}
	return false;
}

//Remove all the Dice from this RollOfDice container
void RollOfDice::empty() {
	dice.clear();
}

//Overload integer conversion operator
//Intentionally not specified as explicit to allow syntax:
//row += rd (implicit conversions)
RollOfDice::operator int() const {
	int tot = 0;
	for (Dice d : dice) {
		tot += d.faceValue;
	}
	return tot;
}

//begin() and end() (const and non-const)
//overloaded to allow looping over object with ranged loop
vector<Dice>::iterator RollOfDice::begin() {
	return dice.begin();
}
vector<Dice>::iterator RollOfDice::end() {
	return dice.end();
}
vector<Dice>::const_iterator RollOfDice::begin() const {
	return dice.begin();
}
vector<Dice>::const_iterator RollOfDice::end() const {
	return dice.end();
}

//Overload insertion operator for a RollOfDice object
ostream &operator<<(ostream &stream, RollOfDice const &rod) {
	stream << "Roll of dice: "<< endl;
	for (Dice d : rod)
		stream << d << "\t " << endl;
	return stream;
}



