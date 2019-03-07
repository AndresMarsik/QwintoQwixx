#include "Player.h"

using namespace std;

int main() {
	string input = "";
	vector<QwintoPlayer> QwintoPlayers;
	vector<QwixxPlayer> QwixxPlayers;
	int game = 0;
	int number = 0;
	RollOfDice& roll = RollOfDice();
	int turn = 0; //keep track of number of turns

	//Ask which game to play, how many players and player names
	while (true) {
		cout << "Which game would you like to play? Qwinto(1) or Qwixx(2)" << endl;
		getline(cin, input);
		stringstream myStream(input);
		if (myStream >> game && (game==1 || game==2))
			break;
		else
			cout << "Invalid input, please try again" << endl;
	}

	//Ask how many players
	while (true) {
		cout << "How many players? (between 1 and 5)" << endl;
		getline(cin, input);
		stringstream myStream(input);
		if (myStream >> number && (number > 0|| number < 6))
			break;
		else
			cout << "Invalid input, please try again" << endl;
	}

	//Initialize players with their names
	//Qwinto
	if (game == 1) {
		for (int i = 0; i < number; i++) {
			cout << "Please enter player name: ";
			getline(cin, input);
			QwintoPlayers.push_back(QwintoPlayer(input));
		}
	}
	//Qwixx
	else {
		for (int i = 0; i < number; i++) {
			cout << "Please enter player name: ";
			getline(cin, input);
			QwixxPlayers.push_back(QwixxPlayer(input));
		}
	}

	//Play game until end condition is met
	//Qwinto
	if (game == 1) {
		int activePlayer = 0;
		while (true) {
			turn++;
			cout << "\n \n \n \n \n \n \n \n \n \n \n \n";
			cout << "-------------------------------------------------- TURN: " << turn << " --------------------------------------------------" << endl;
			cout << endl;
			QwintoPlayers[activePlayer].inputBeforeRoll(roll);
			if (QwintoPlayers[activePlayer].inputAfterRoll(roll)) //if game is over, break of loop
				break;
			for (int i = 0; i < number; i++) {
				if (i != activePlayer) {
					if (QwintoPlayers[i].inputAfterRoll(roll))
						break;    //if game is over, break of loop
				}
			}
			roll.empty();
			activePlayer = (activePlayer + 1) % number;
		}

		//Calculate and display final score
		int highscore = -21;     //highscore 
		int currentScore = 0;  //temporary counter
		int winner = 0;        //position of winner in vector
		for (int i = 0; i < number; i++) {
			currentScore = QwintoPlayers[i].total();
			if (currentScore > highscore) {
				highscore = currentScore;
				winner = i;
			}
		}

		//Display winner
		cout << "\n \n \n \n \n \n \n \n \n \n \n \n " << "THE WINNER IS: " << endl;
		QwintoPlayers[winner].total();
	}

	//Qwixx
	if (game == 2) {
		int activePlayer = 0;
		while (true) {
			turn++;
			cout << "\n \n \n \n \n \n \n \n \n \n \n \n";
			cout << "-------------------------------------------------- TURN: " << turn << " --------------------------------------------------" << endl;
			cout << endl;
			QwixxPlayers[activePlayer].inputBeforeRoll(roll);
			if (QwixxPlayers[activePlayer].inputAfterRoll(roll)) //if game is over, break of loop
				break;
			for (int i = 0; i < number; i++) {
				if (i != activePlayer) {
					if (QwixxPlayers[i].inputAfterRoll(roll))
						break;    //if game is over, break of loop
				}
			}
			roll.empty();
			activePlayer = (activePlayer + 1) % number;
		}

		//Calculate and display final score
		int highscore = -21;     //highscore 
		int currentScore = 0;  //temporary counter
		int winner = 0;        //position of winner in vector
		for (int i = 0; i < number; i++) {
			currentScore = QwixxPlayers[i].total();
			if (currentScore > highscore) {
				highscore = currentScore;
				winner = i;
			}
		}

		//Display winner
		cout << "\n \n \n \n \n \n \n \n \n \n \n \n " << "THE WINNER IS: " << endl;
		QwixxPlayers[winner].total();
	}

	//press 3 times to confirm exit
	cout << "Press enter 3 times to exit console";
	getline(cin, input);
	getline(cin, input);
	getline(cin, input);

	return 0;
}