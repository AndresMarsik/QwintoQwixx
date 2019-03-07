//includes
#include <stdexcept>
#include "Scoresheet.h";

//use std
using namespace std;
 

//Definitions for QwintoRow attributes-------------------------------------------------

//overloaded subscript operator
template<Colour C>
int &QwintoRow<C>::operator[](int idx) {
	if (idx<0 || idx >= rowData.size())throw out_of_range("index out of range.");
	return rowData[idx];
}

//define validation implementations
//NOTE: cannot verify column condition (i.e. cannot have repeated value in a column);
//must require player-level logic to do so
//specialize validation for RED row
template<>
bool QwintoRow<Colour::RED>::validate(const int score, const int idx) {
	if (idx == 3) return false; // return false if cell is not allowed to be filled
	return commonValidation(score, idx);
}
//specialize validation for YELLOW row
template<>
bool QwintoRow<Colour::YELLOW>::validate(const int score, const int idx) {
	if (idx == 5) return false; // return false if cell is not allowed to be filled
	return commonValidation(score, idx);
}
//specialize validation for BLUE row
template<>
bool QwintoRow<Colour::BLUE>::validate(const int score, const int idx) {
	if (idx == 4) return false; // return false if cell is not allowed to be filled
	return commonValidation(score, idx);
}
//helper for common validation code
template<Colour C>
bool QwintoRow<C>::commonValidation(const int score, const int idx) {
	//return false if out of range or if cell already filled
	if (idx >= rowData.size() || idx<0 || rowData[idx] != 0) return false;
	//verify numbers in increasing order up to cell to be filled and in decreasing order from end of row
	int flag = 0;
	
	//Check that numbers before idx are smaller
	if (idx == 0) 
		flag++;

	else {
		int max = 0;
		for (int i = (idx - 1); i >= 0; --i) {
			if (rowData[i] != 0) {
				max = rowData[i];
				break;
			}
		}
		if (score > max)
			flag++;
	}

	//check that numbers after idx are larger
	if (idx == 9)
		flag++;

	else {
		int min = 19;
		for (int i = (idx + 1); i < 10; i++) {
			if (rowData[i] != 0) {
				min = rowData[i];
				break;
			}
		}
		if (score < min)
			flag++;
	}

	return (flag==2);
}

//setup values used by ostream overload
template<>
char QwintoRow<Colour::RED>::rowChars[] = { '|', '%', '%', '|', '|', '%', '%', '|', '|', '|', '|' };
template<>
int QwintoRow<Colour::RED>::noIdx = 3;
template<>
char QwintoRow<Colour::YELLOW>::rowChars[] = { '|', '|', '|', '|', '|', '|', '|', '%', '%', '|', '|' };
template<>
int QwintoRow<Colour::YELLOW>::noIdx = 5;
template<>
char QwintoRow<Colour::BLUE>::rowChars[] = { '|', '|', '%', '%', '|', '|', '|', '|', '|', '%', '%' };
template<>
int QwintoRow<Colour::BLUE>::noIdx = 4;

//ostream overload - generic overload that used templated attributes
template<Colour C>
ostream &operator<<(ostream &stream, QwintoRow<C> &row) {
	for (int i = 0; i < 10; i++) {
		stream << row.rowChars[i];
		if (i == row.noIdx) {
			stream << "XX";
		}
		else {
			if (row[i]<10) stream << " ";
			stream << row[i];
		}
	}
	stream << row.rowChars[9];

	return stream;
}

//for checking if a specific row is full
template<Colour C>
bool QwintoRow<C>::isFull() const {
	for (int i = 0; i<rowData.size(); ++i) {
		if (!rowData[i] && i != noIdx) return false;
	}
	return true;
}

//Definitions for QwixxRow attributes-------------------------------------------------
//define row lock (static - shared between all scoresheets)
template<class T, Colour C> bool QwixxRow<T, C>::isLocked=false;

//validation for scoring in a qwixx row
template<class T, Colour C>
bool QwixxRow<T, C>::validation(int score) {
	//verify row is currently unlocked
	if (isLocked)
		return false;

	//find the last "filled" index
	//if the target index is "before" this index, the selection is invalid
	int idx = 0;
	int lastIdx = 0;
	for (auto i = rowData.begin(); i != rowData.end(); ++i) {
		if (*i == 0) {
			lastIdx = idx;
		}
		idx++;
	}

	if (C == Colour::RED || C == Colour::YELLOW) {
		return (((lastIdx + 2) < score) || ((lastIdx == 0 && (lastIdx+2) == score)));
	}

	return (((lastIdx - 12) < (-1 * score)) || (lastIdx == 0 && ((lastIdx - 12) == (-1 * score))));
}

//place a score in a given row
template<class T, Colour C>
QwixxRow<T, C>& QwixxRow<T, C>::operator+=(const int score) {

	if (!validation(score))
		throw invalid_argument("Rule violation - cannot mark given score.");

	//convert score to idx
	int idx = 12 - score;
	if (C == Colour::RED || C == Colour::YELLOW) {
		idx = score - 2;
	}

	//Enter the score
	auto it = rowData.begin();
	for (int x = 0; x < idx; ++x) {
		it++;
	}
	it = rowData.erase(it);
	rowData.insert(it, 0);

	auto i = rowData.end();
	i--;
	//validate for row locking: require last score filled plus a minimum of four other scores
	if (*i == 0) { 
		int numsc = 0;
		for (auto it2 = rowData.begin(); it2 != rowData.end(); ++it2) {
			if (*it2 == 0) {
				++numsc;
			}
		}
		if (numsc > 4) {
			isLocked = true;
			wasLockedByUser = true;
		}			
	}

	return *this;
}

//overload subscript operator
template<class T, Colour C>
int &QwixxRow<T,C>::operator[](int idx) {
	
	if (idx<0 || idx >= rowData.size())
		throw out_of_range("index out of range.");
	auto it = rowData.begin();
	for (int x = 0; x < idx; x++) {
		it++;
	}
	return *it;
}

//overload insertion operator
template<class T, Colour C>
ostream &operator<<(ostream &stream, const QwixxRow<T, C> &row) {
	int sc = 12;
	int inc = -1;
	if (C == Colour::RED || C == Colour::YELLOW) {
		sc = 2;
		inc = 1;
	}
	for (auto i = row.rowData.begin(); i != row.rowData.end(); i++) {

		stream << "|" << ((sc < 10) &&  (*i != 0)  ? " " : "") << (*i == 0 ? "XX" : to_string(sc));
		sc += inc;
	}
	stream << "|" << (row.isLocked ? " L" : " U");

	return stream;
}

//check if row is locked
template<class T, Colour C>
bool QwixxRow<T, C>::locked() const {
	return wasLockedByUser;
}

//"clear" a row (used to start a sheet)
//0 is a marker for score and non-zero represents unmarked
template<class T, Colour C>
void QwixxRow<T, C>::clear() {

	if (!rowData.empty()) {
		auto it = rowData.begin();
		for (int i = 0; i < 11; i++) {
			it = rowData.erase(it);
		}
	}
	for (int i = 0; i < 11; i++) {
		rowData.push_back(1);
	}
}

//--------------------------  SCORESHEET -------------------------------------------------------
//Constructor 
ScoreSheet::ScoreSheet(string& name) : playerName{ name } {}

//score the throw
bool ScoreSheet::score(RollOfDice& roll, Colour color, int pos) {

	if (!validate(roll, color, pos))
		return false;
	else
		return true;
}

//if throw failed, add a failed throw to the scoresheet
void ScoreSheet::failed() {
	failedThrow.push_back(failedThrow.size() + 1);
}
//Set and return final score
int ScoreSheet::setTotal() {
	finalScore = calcTotal();
	finalScore = finalScore - failedThrow.size() * 5;
	return finalScore;
}

//overload insertion operator
ostream& operator<< (ostream& stream, ScoreSheet& sheet) {
	stream << "Player: " << sheet.playerName << "\t \t \t Points: " << sheet.finalScore << endl;
	sheet.print(stream);
	return stream;
}

//------------------------------------ QWINTO SCORESHEET ---------------------------------------------------------------

//Constructor
QwintoScoreSheet::QwintoScoreSheet(string& name) : ScoreSheet(name) {}

//validate the dice roll and enter the score if valid
bool QwintoScoreSheet::validate(RollOfDice& roll, Colour color, int pos) {

	switch (color) {
	case Colour::RED:
		try {
			if (yellowRow[pos] != roll && blueRow[pos+1] != roll && redRow.validate(roll, pos - 1)) {
				redRow[pos - 1] = roll;
				return true;
			}
			else
				return false;
		}
		catch(...) {
			if (redRow.validate(roll, pos - 1)) {
				redRow[pos - 1] = roll;
				return true;
			}
			else 
				return false;
		}

	case Colour::YELLOW:
		try {
			if (redRow[pos-2] != roll && blueRow[pos] != roll && yellowRow.validate(roll, pos - 1)) {
				yellowRow[pos - 1] = roll;
				return true;
			}
			else
				return false;
		}
		catch (...) {
			if (yellowRow.validate(roll, pos - 1)) {
				yellowRow[pos - 1] = roll;
				return true;
			}
			else
				return false;
		}
	case Colour::BLUE:
		try {
			if (yellowRow[pos-2] != roll && redRow[pos-3] != roll && blueRow.validate(roll, pos - 1)) {
				blueRow[pos - 1] = roll;
				return true;;
			}
			else
				return false;
		}
		catch (...) {
			if (blueRow.validate(roll, pos - 1)) {
				blueRow[pos - 1] = roll;
				return true;
			}
			else
				return false;
		}
	}
}

//calculate final score
int QwintoScoreSheet::calcTotal() {

	int total = 0;

	//go through all colours and if it's full add rightmost score, if not add number of scored entered
	//RED
	if (redRow.isFull())
		total += redRow[9];
	else {
		for (int i = 0; i < 10; i++) {
			if (redRow[i] != 0)
				total++;
		}
	}

	//YELLOW
	if (yellowRow.isFull())
		total += yellowRow[9];
	else {
		for (int i = 0; i < 10; i++) {
			if (yellowRow[i] != 0)
				total++;
		}
	}

	//BLUE
	if (blueRow.isFull())
		total += blueRow[9];
	else {
		for (int i = 0; i < 10; i++) {
			if (blueRow[i] != 0)
				total++;
		}
	}

	//Check the special columns and if they are full add the score in the bonus space
	if (redRow[0] != 0 && yellowRow[1] != 0 && blueRow[2] != 0)
		total += blueRow[2];

	if (redRow[1] != 0 && yellowRow[2] != 0 && blueRow[3] != 0)
		total += redRow[1];

	if (redRow[5] != 0 && yellowRow[6] != 0 && blueRow[7] != 0)
		total += redRow[5];

	if (redRow[6] != 0 && yellowRow[7] != 0 && blueRow[8] != 0)
		total += yellowRow[7];

	if (redRow[7] != 0 && yellowRow[8] != 0 && blueRow[9] != 0)
		total += blueRow[9];

	return total;
}

//return true if game is over, false otherwise
bool QwintoScoreSheet::operator!() const {
	
	if (failedThrow.size() == 4)
		return true;

	int counter = 0;
	if (redRow.isFull())
		counter++;
	if (yellowRow.isFull())
		counter++;
	if (blueRow.isFull())
		counter++;

	if (counter >= 2)
		return true;

	return false;

}

void QwintoScoreSheet::print(ostream& o) {

	o << "                     -------------------------------" << endl;
	o << "Red          " << "       " << redRow << endl;
	o << "             " << "    " << "-----------------------------------" << endl;
	o << "Yellow       " << "    " << yellowRow << endl;
	o << "             " << "-----------------------------------" << endl;
	o << "Blue          " << blueRow << endl;
	o << "             " << "---------------------------------" << endl;
	o << "Failed throws: ";
	for (int i = 0; i < failedThrow.size(); i++) {
		o << failedThrow[i] << " ";
	}
	o << endl;
	
	return;
}

//-------------------------------------- QUIXX SCORESHEET ---------------------------------------------------------------

//Constructor
QwixxScoreSheet::QwixxScoreSheet(string& name) : ScoreSheet(name) {
	//clear the rows for start of game
	redRow.clear();
	yellowRow.clear();
	greenRow.clear();
	blueRow.clear();
}

//validate the dice roll and enter the score if valid
bool QwixxScoreSheet::validate(RollOfDice& roll, Colour color, int pos) {

	try {
		if (color == Colour::RED)
			redRow += roll;

		if (color == Colour::YELLOW)
			yellowRow += roll;

		if (color == Colour::GREEN)
			greenRow += roll;

		if (color == Colour::BLUE)
			blueRow += roll;
	}
	catch (...) {
		return false;
	}
	return true;
}

//calculate final score
int QwixxScoreSheet::calcTotal() {

	int total = 0;
	int counter = 0;

	//declare point board
	int points[12] = { 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78 };

	//go through all rows and add up the number of scores + a bonus if locked and convert to the corresponding points
	//RED
	for (int i = 0; i < 11; i++) {
		if (redRow[i] == 0)
			counter++;
	}
	if (redRow.locked())
		counter++;
	if (counter != 0)
		total += points[counter - 1];
	counter = 0;

	//YELLOW
	for (int i = 0; i < 11; i++) {
		if (yellowRow[i] == 0)
			counter++;
	}
	if (yellowRow.locked())
		counter++;
	if (counter != 0)
		total += points[counter - 1];
	counter = 0;

	//GREEN
	for (int i = 0; i < 11; i++) {
		if (greenRow[i] == 0)
			counter++;
	}
	if (greenRow.locked())
		counter++;
	if (counter != 0)
		total += points[counter - 1];
	counter = 0;

	//BLUE
	for (int i = 0; i < 11; i++) {
		if (blueRow[i] == 0)
			counter++;
	}
	if (blueRow.locked())
		counter++;
	if (counter != 0)
		total += points[counter - 1];

	return total;
}

//test and return true if game is over, false otherwise
bool QwixxScoreSheet::operator!() const {

	if (failedThrow.size() == 4)
		return true;

	int counter = 0;
	if (redRow.locked())
		counter++;
	if (yellowRow.locked())
		counter++;
	if (greenRow.locked())
		counter++;
	if (blueRow.locked())
		counter++;

	if (counter >= 2)
		return true;;

	return false;
}

//printing method for scoresheet
void QwixxScoreSheet::print(ostream& o){

	o << "             ------------------------------------" << endl;
	o << "Red          " << redRow << endl;
	o << "             ------------------------------------" << endl;
	o << "Yellow       " << yellowRow << endl;
	o << "             ------------------------------------" << endl;
	o << "Green        " << greenRow << endl;
	o << "             ------------------------------------" << endl;
	o << "Blue         " << blueRow << endl;
	o << "             ------------------------------------" << endl;
	o << "Failed throws: ";
	for (int i = 0; i < failedThrow.size(); i++) {
		o << failedThrow[i] << " ";
	}
	o << endl;

	return;

}