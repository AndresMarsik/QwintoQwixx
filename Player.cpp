//includes
#include "Player.h"

//use std namespace
using namespace std;

// ------------------------------ QWINTO PLAYER ----------------------------------------------

//Constructor
QwintoPlayer::QwintoPlayer(string& name) {
	sheet = new QwintoScoreSheet(name);
}

//method is run for the active player at the beginning of a term
void QwintoPlayer::inputBeforeRoll(RollOfDice& roll) {

	//Activate current rolling player
	isActive = true;

	//initialize
	string input = "";
	Dice red(Colour::RED);
	Dice yellow(Colour::YELLOW);
	Dice blue(Colour::BLUE);

	cout << *sheet << endl;

	//Add red  die
	while (true) {
		cout << "Would you like to roll a Red dice? (Y/N) ";
		getline(cin, input);
		if (input.length() == 1 && (input[0] == 'Y' || input[0] == 'y' || input[0] == 'N' || input[0] == 'n')) {
			if (input[0] == 'Y' || input[0] == 'y') {
				roll.addDie(red);
				break;
			}
			else
				break;

		}
		else
			cout << "Invalid input, please enter Y (yes) or N (no)" << endl;
	}

	//Add yellow
	while (true) {
		cout << "Would you like to roll a Yellow dice? (Y/N) ";
		getline(cin, input);
		if (input.length() == 1 && (input[0] == 'Y' || input[0] == 'y' || input[0] == 'N' || input[0] == 'n')) {
			if (input[0] == 'Y' || input[0] == 'y') {
				roll.addDie(yellow);
				break;
			}
			else
				break;

		}
		else
			cout << "Invalid input, please enter Y (yes) or N (no)" << endl;
	}

	//Add blue
	while (true) {
		cout << "Would you like to roll a Blue dice? (Y/N) ";
		getline(cin, input);
		if (input.length() == 1 && (input[0] == 'Y' || input[0] == 'y' || input[0] == 'N' || input[0] == 'n')) {
			if (input[0] == 'Y' || input[0] == 'y') {
				roll.addDie(blue);
				break;
			}
			else
				break;
		}
		else
			cout << "Invalid input, please enter Y (yes) or N (no)" << endl;
	}

	//roll selected dice
	roll.roll();
}

//Run for each user after a roll has been made
bool QwintoPlayer::inputAfterRoll(RollOfDice& roll) {
	//for storing position selection
	int pos = 0;
	//for storing colour selection
	char color = { 0 };
	string input = "";
	//for checking if the user decided to score
	bool flag = false;

	cout << "\n \n \n \n \n \n \n \n \n \n \n \n";
	cout << roll << endl;
		cout << *sheet << endl;

		while (true) {
			cout << "Would you like to enter a score? (Y/N)" << endl;
			getline(cin, input);
			if (input.length() == 1 && (input[0] == 'Y' || input[0] == 'y' || input[0] == 'N' || input[0] == 'n')) {
				if (input[0] == 'N' || input[0] == 'n') {
					if (isActive) {
						sheet->failed();
						cout << "Failed throw" << endl;
						isActive = false;
						return (!(*sheet));
					}
					else
						return (!(*sheet));
				}
				else
					break;
			}
			else
				cout << "Invalid input, please enter either Y (yes) or N (no)" << endl;
		}

		while (true) {
			cout << "What row would you like to score in? Red(R), Yellow(Y) or Blue(B)" << endl;
			getline(cin, input);
			if (input.length() == 1 && (input[0] == 'R' || input[0] == 'r' || input[0] == 'Y' || input[0] == 'y' || input[0] == 'B' || input[0] == 'b')) {
				color = input[0];
				cout << "In what position?" << endl;
				getline(cin, input);
				stringstream mystream(input);
				if (mystream >> pos && pos > 0 && pos < 11)
					break;
			}
			cout << "Invalid input, please try again" << endl;
		}

			if ((color == 'R' || color == 'r') && roll.hasColour(Colour::RED))
				flag = sheet->score(roll, Colour::RED, pos);
			if ((color == 'Y' || color == 'y')  && roll.hasColour(Colour::YELLOW))
				flag = sheet->score(roll, Colour::YELLOW, pos);
			if ((color == 'B' || color == 'b') && roll.hasColour(Colour::BLUE))
				flag = sheet->score(roll, Colour::BLUE, pos);

			//User already said whether they want to score or not, if scoreing fails add a failed throw if active
			if (!flag) {
				cout << "Failed to score" << endl;
				if (isActive) {
					sheet->failed();
					cout << "Failed throw" << endl;
				}
			}
			else
				cout << "Score entered" << endl;

			isActive = false;
			return (!(*sheet));
}

//For computing the total score for a single user
int QwintoPlayer::total() {
	int tot = sheet->setTotal();
	cout << *sheet << endl;
	return tot;
}

//--------------------------------QWIXX PLAYER ------------------------------------------------

//Initialize static members for Dice to be used in game
Dice* QwixxPlayer::white1 = new Dice(Colour::WHITE);
Dice* QwixxPlayer::white2 = new Dice(Colour::WHITE);
Dice* QwixxPlayer::red = new Dice(Colour::RED);
Dice* QwixxPlayer::yellow = new Dice(Colour::YELLOW);
Dice* QwixxPlayer::green = new Dice(Colour::GREEN);
Dice* QwixxPlayer::blue = new Dice(Colour::BLUE);

//Constructor (overrides the default constructor - require name to create user)
QwixxPlayer::QwixxPlayer(string& name) {
	sheet = new QwixxScoreSheet(name);
}

//Run at the beginning of each turn - dice are shared between all users
void QwixxPlayer::inputBeforeRoll(RollOfDice& roll) {
	//roll all dice that correspond to currently unlocked rows
	roll.addDie(*QwixxPlayer::white1);
	roll.addDie(*QwixxPlayer::white2);
	if (QwixxPlayer::red != nullptr)
		roll.addDie(*QwixxPlayer::red);
	if (QwixxPlayer::yellow != nullptr)
		roll.addDie(*QwixxPlayer::yellow);
	if (QwixxPlayer::green != nullptr)
		roll.addDie(*QwixxPlayer::green);
	if (QwixxPlayer::blue != nullptr)
		roll.addDie(*QwixxPlayer::blue);

	//roll selected dice
	roll.roll();

	//activate current user
	isActive = true;
	return;
}

//Run for each player after the dice have been rolled
bool QwixxPlayer::inputAfterRoll(RollOfDice& roll) {

	int scored = 0;
	string input = "";

	//output the roll for the current turn
	cout << roll << endl;
	cout << "\n";
	cout << *sheet << endl;

	//Ask if and where to score white dice (all players)
	while (true) {
		cout << "Which row do you want to score the white dice in?: red(R), yellow(Y), green(G), blue(B), none(N)" << endl;
		getline(cin, input);
		if (input.length() == 1 && (input[0] == 'R' || input[0] == 'r' || input[0] == 'Y' || input[0] == 'y' || input[0] == 'G' || input[0] == 'g' || input[0] == 'B' || input[0] == 'b' || input[0] == 'N' || input[0] == 'n')) {
			//RED
			if (input[0] == 'R' || input[0] == 'r') {
				if (sheet->score(roll.pair(Colour::WHITE, 1, Colour::WHITE, 2), Colour::RED)) {
					scored++;
					cout << "White dice scored successfully" << endl;
					if (sheet->redRow.locked())
						QwixxPlayer::red = nullptr;
					break;
				}
				cout << "Scoring failed" << endl;
				break;
			}
			//YELLOW
			if (input[0] == 'Y' || input[0] == 'y') {
				if (sheet->score(roll.pair(Colour::WHITE, 1, Colour::WHITE, 2), Colour::YELLOW)) {
					scored++;
					cout << "White dice scored successfully" << endl;
					if (sheet->yellowRow.locked())
						QwixxPlayer::yellow = nullptr;
					break;
				}
				cout << "Scoring failed" << endl;
				break;
			}
			//GREEN
			if (input[0] == 'G' || input[0] == 'g') {
				if (sheet->score(roll.pair(Colour::WHITE, 1, Colour::WHITE, 2), Colour::GREEN)) {
					scored++;
					cout << "White dice scored successfully" << endl;
					if (sheet->greenRow.locked())
						QwixxPlayer::green = nullptr;
					break;
				}
				cout << "Scoring failed" << endl;
				break;
			}
			//BLUE
			if (input[0] == 'B' || input[0] == 'b') {
				if (sheet->score(roll.pair(Colour::WHITE, 1, Colour::WHITE, 2), Colour::BLUE)) {
					scored++;
					cout << "White dice scored successfully" << endl;
					if (sheet->blueRow.locked())
						QwixxPlayer::blue = nullptr;
					break;
				}
				cout << "Scoring failed" << endl;
				break;
			}
			break;
		}
		else
			cout << "Invalid input, please try again" << endl;
	}

	//Ask for white+color dice combination to score,
	//if no score at all add a failed throw (active player only)
	if (isActive) {
		while (true) {
			cout << "Would you like to score a combination of white and color dice? (Y/N)" << endl;
			getline(cin, input);
			if (input.length() == 1 && (input[0] == 'Y' || input[0] == 'y' || input[0] == 'N' || input[0] == 'n')) {
				if (input[0] == 'Y' || input[0] == 'y') {
					while (true) {
						cout << "What combination of dice would you like to score?: " << endl;
						cout << "White1 + red    (1) \t White2 + red    (2)" << endl;
						cout << "White1 + yellow (3) \t White2 + yellow (4)" << endl;
						cout << "White1 + green  (5) \t White2 + green  (6)" << endl;
						cout << "White1 + blue   (7) \t White2 + blue   (8)" << endl;
						getline(cin, input);
						if (input.length() == 1 && (input[0] == '1' || input[0] == '2' || input[0] == '3' || input[0] == '4' || input[0] == '5' || input[0] == '6' || input[0] == '7' || input[0] == '8')) {
							//White1+red
							if ((input[0] == '1') && QwixxPlayer::red) {
								if (sheet->score(roll.pair(Colour::WHITE, 1, Colour::RED, 1), Colour::RED)) {
									cout << "Scoring successful" << endl;
									if (sheet->redRow.locked())
										QwixxPlayer::red = nullptr;
									scored++;
									break;
								}
								else {
									cout << "Scoring failed" << endl;
									break;
								}
							}
							//White2+red
							if ((input[0] == '2') && QwixxPlayer::red) {
								if (sheet->score(roll.pair(Colour::WHITE, 2, Colour::RED, 1), Colour::RED)) {
									cout << "Scoring successful" << endl;
									if (sheet->redRow.locked())
										QwixxPlayer::red = nullptr;
									scored++;
									break;
								}
								else {
									cout << "Scoring failed" << endl;
									break;
								}
							}
							//White1+yellow
							if ((input[0] == '3') && QwixxPlayer::yellow) {
								if (sheet->score(roll.pair(Colour::WHITE, 1, Colour::YELLOW, 1), Colour::YELLOW)) {
									cout << "Scoring successful" << endl;
									if (sheet->yellowRow.locked())
										QwixxPlayer::yellow = nullptr;
									scored++;
									break;
								}
								else {
									cout << "Scoring failed" << endl;
									break;
								}
							}
							//White2+yellow
							if ((input[0] == '4') && QwixxPlayer::yellow) {
								if (sheet->score(roll.pair(Colour::WHITE, 2, Colour::YELLOW, 1), Colour::YELLOW)) {
									cout << "Scoring successful" << endl;
									if (sheet->yellowRow.locked())
										QwixxPlayer::yellow = nullptr;
									scored++;
									break;
								}
								else {
									cout << "Scoring failed" << endl;
									break;
								}
							}
							//White1+green
							if ((input[0] == '5') && QwixxPlayer::green) {
								if (sheet->score(roll.pair(Colour::WHITE, 1, Colour::GREEN, 1), Colour::GREEN)) {
									cout << "Scoring successful" << endl;
									if (sheet->greenRow.locked())
										QwixxPlayer::green = nullptr;
									scored++;
									break;
								}
								else {
									cout << "Scoring failed" << endl;
									break;
								}
							}
							//White2+green
							if ((input[0] == '6') && QwixxPlayer::green) {
								if (sheet->score(roll.pair(Colour::WHITE, 2, Colour::GREEN, 1), Colour::GREEN)) {
									cout << "Scoring successful" << endl;
									if (sheet->greenRow.locked())
										QwixxPlayer::green = nullptr;
									scored++;
									break;
								}
								else {
									cout << "Scoring failed" << endl;
									break;
								}
							}
							//White1+blue
							if ((input[0] == '7') && QwixxPlayer::blue) {
								if (sheet->score(roll.pair(Colour::WHITE, 1, Colour::BLUE, 1), Colour::BLUE)) {
									cout << "Scoring successful" << endl;
									if (sheet->blueRow.locked())
										QwixxPlayer::blue = nullptr;
									scored++;
									break;
								}
								else {
									cout << "Scoring failed" << endl;
									break;
								}
							}
							//White2+blue
							if ((input[0] == '8') && QwixxPlayer::blue) {
								if (sheet->score(roll.pair(Colour::WHITE, 2, Colour::BLUE, 1), Colour::BLUE)) {
									cout << "Scoring successful" << endl;
									if (sheet->blueRow.locked())
										QwixxPlayer::blue = nullptr;
									scored++;
									break;
								}
								else {
									cout << "Scoring failed" << endl;
									break;
								}
							}
						}
						else
							cout << "Invalid input, try  again" << endl;
					}
				}
				if (scored == 0) {
					cout << "Failed throw" << endl;
					sheet->failed();
					break;
				}
				break;
			}
			else
				cout << "Invalid input, please enter Y(yes) or N(no)" << endl;
		}
	}

	//deactivate user (in case user is currently active)
	isActive = false;
	return (!(*sheet));
}

//compute final score for each user
int QwixxPlayer::total() {
	int tot = sheet->setTotal();
	cout << *sheet << endl;
	return tot;
}


