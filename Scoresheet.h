//only define once
#ifndef CSI2372_PROJECT_SCORESHEET_H
#define CSI2372_PROJECT_SCORESHEET_H

//includes
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <list>
#include <array>
#include "Dice.h"

//use std
using namespace std;

//------------------------------------------------- QWINTO ROW ---------------------------------------------
//Templated Qwinto row for a given colour
template<Colour C>
class QwintoRow {
	//template friend ost overload to use << operator
	template<Colour C>friend ostream& operator<<(ostream& stream, QwintoRow<C>& row);
	//container for scores
	array<int, 10> rowData;
	bool commonValidation(int score, int idx);
public:
	static char rowChars[];
	static int noIdx;
	int& operator[] (int index);
	bool validate(int score, int idx);
	bool isFull() const;
};

//---------------------------------------------------- QWIXX ROW -------------------------------------------
//Templated Qwixx row for a given colour and specified type for container
template<class T, Colour C>
class QwixxRow {
	//template friend ost overload to use << operator
	template<class T, Colour C>friend ostream& operator<<(ostream& stream, const QwixxRow<T, C>& row);
	//container for scores (type is template specified)
	T rowData;
	bool validation(int score);
	static bool isLocked;//row of colour is locked
	bool wasLockedByUser;//row was locked by the owner of this row object
public:
	QwixxRow& operator+=(int score);
	bool locked() const;
	int& operator[] (int index);
	void clear();
};

//---------------------------------------- SCORESHEET ------------------------------------------
//Parent class to be extended for each scoresheet type
class ScoreSheet {
	string playerName;
	int finalScore = 0;
	friend ostream& operator<< (ostream& stream, ScoreSheet& sheet);

public:
	ScoreSheet(string& name);
	bool score(RollOfDice& roll, Colour color, int pos = -1);
	int setTotal();
	virtual bool operator!() const = 0;
	void failed();

protected:
	vector<int> failedThrow;
	virtual bool validate(RollOfDice& roll, Colour color, int pos) = 0;
	virtual int calcTotal() = 0;
	virtual void print(ostream& o) = 0;
};

//---------------------------------------- QWINTO SCORESHEET ---------------------------------------------------------
//scoresheet extended for qwinto
class QwintoScoreSheet :public ScoreSheet {
	QwintoRow<Colour::RED> redRow;
	QwintoRow<Colour::YELLOW> yellowRow;
	QwintoRow<Colour::BLUE> blueRow;

protected:
	bool validate(RollOfDice& roll, Colour color, int pos);
	int calcTotal();
	void print(ostream& o);
public:
	QwintoScoreSheet(string& name);
	bool operator!() const;
};

//---------------------------------------------------- QWIXX SCORESHEET --------------------------------------------------------
//scoresheet extended for qwixx
class QwixxScoreSheet : public ScoreSheet {
protected:
	bool validate(RollOfDice& roll, Colour color, int pos);
	int calcTotal();
	void print(ostream& o);

public:
	//specify construction for each row from templated row class
	QwixxRow<vector<int>, Colour::RED> redRow;
	QwixxRow<vector<int>, Colour::YELLOW> yellowRow;
	QwixxRow<list<int>, Colour::GREEN> greenRow;
	QwixxRow<list<int>, Colour::BLUE> blueRow;

	QwixxScoreSheet(string& name);
	bool operator!() const;
};

#endif //CSI2372_PROJECT_SCORESHEET_H