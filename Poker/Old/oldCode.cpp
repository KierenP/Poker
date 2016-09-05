#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

class Players {
public:
	Players(string n, int b)
	{
		name = n;
		balance = b;
		folded = false;
		dealer = false;
		bBlind = false;
		sBlind = false;
		toCall = 0;
	}

	string name;
	int balance;
	int toCall;
	bool folded, dealer, bBlind, sBlind;
};

int totalPlayers;
int initialBalance;
int pot;
int playerInput;
int bBlind;
int sBlind;
int playerTurn;
bool winner = false;
int dealer;
vector<Players> vectorOfPlayers;

void renderBoard();
void playerToCall(int raise);
void processInput();
void playerTurnChange();
void newHand();
string turnIndicate(int i);
void handleBlinds();
string blindIndicte(int i);
string foldedIndicate(int i);
bool checkBalance(int i, int n);
void allIn(int i);

void allIn(int i) //transfers all of a players balance into the pot
{
	pot += vectorOfPlayers.at(i).balance;
	vectorOfPlayers.at(i).balance = 0;
}

bool checkBalance(int i, int n)
{
	if (vectorOfPlayers.at(i).balance >= n + vectorOfPlayers.at(i).toCall)
	{
		return true;
	}
	return false;
}

string foldedIndicate(int i)
{
	if (vectorOfPlayers.at(i).folded == true)
	{
		return " Folded";
	}
	else { return ""; };
}

void playerToCall(int raise)
{
	for (int i = 0; i <= totalPlayers - 1;i++)
	{
		vectorOfPlayers.at(i).toCall += raise;
	}
}

string blindIndicate(int i)
{
	if (vectorOfPlayers.at(i).dealer == true && vectorOfPlayers.at(i).bBlind == true)
	{
		return "    Dealer, Big Blind";
	}
	else if (vectorOfPlayers.at(i).dealer == true)
	{
		return "    Dealer";
	}
	else if (vectorOfPlayers.at(i).sBlind == true)
	{
		return "    Small Blind";
	}
	else if (vectorOfPlayers.at(i).bBlind == true)
	{
		return "    Big Blind";
	}
	else { return ""; }
}

void handleBlinds()  //hardcoded handelling for looping through array while relatively assigning blinds from dealer location
{
	if (winner == false)
	{
		for (int i = 0; i <= totalPlayers - 1;i++)
		{
			vectorOfPlayers.at(i).dealer = false;
			vectorOfPlayers.at(i).bBlind = false;
			vectorOfPlayers.at(i).sBlind = false;
		}

		vectorOfPlayers.at(dealer).dealer = true;
		if (dealer == totalPlayers - 1)
		{
			vectorOfPlayers.at(0).sBlind = true;
			vectorOfPlayers.at(1).bBlind = true;
		}
		else if (dealer == totalPlayers - 2)
		{
			vectorOfPlayers.at(dealer + 1).sBlind = true;
			vectorOfPlayers.at(0).bBlind = true;
		}
		else
		{
			vectorOfPlayers.at(dealer + 1).sBlind = true;
			if (totalPlayers == 2)
			{
				vectorOfPlayers.at(0).bBlind = true;
			}
			else { vectorOfPlayers.at(dealer + 2).bBlind = true; }
		}

		if (dealer == totalPlayers - 1)
		{
			dealer = 0;
		}
		else { dealer += 1; }

		for (int i = 0; i <= totalPlayers - 1;i++) //deduct blinds from players
		{
			if (vectorOfPlayers.at(i).sBlind == true || vectorOfPlayers.at(i).bBlind == true)
			{
				if (vectorOfPlayers.at(i).bBlind == true)
				{
					if (vectorOfPlayers.at(i).balance < bBlind) //check if player can afford blind to prevent negative balance
					{
						allIn(i);
					}
					else
					{
						pot += bBlind;
						vectorOfPlayers.at(i).balance -= bBlind;
					}
				}
				else
				{
					if (vectorOfPlayers.at(i).balance < sBlind) //check if player can afford blind to prevent negative balance
					{
						allIn(i);
					}
					else
					{
						pot += sBlind;
						vectorOfPlayers.at(i).balance -= sBlind;
					}
				}
			}
		}
	}
}

string turnIndicate(int i) //Displays a # next to the name of the player whos turn it is
{
	if (vectorOfPlayers.at(i).name == vectorOfPlayers.at(playerTurn).name)
	{
		return " ~";
	}
	else { return ""; };
}

void newHand()
{

	for (int i = 0; i < totalPlayers; i++) //removes players without any money
	{

		if (vectorOfPlayers.at(i).balance <= 0)
		{
			vectorOfPlayers.erase(vectorOfPlayers.begin() + i);
			totalPlayers -= 1;
			dealer--;
			i--;
		}
	}

	if (vectorOfPlayers.size() == 1)
	{
		winner = true;
	}

	for (int i = 0; i <totalPlayers - 1; i++) //Resets all players folded states
	{
		vectorOfPlayers.at(i).folded = false;
	}
	for (int i = 0; i <totalPlayers - 1; i++) //Resets all players toCall values
	{
		vectorOfPlayers.at(i).toCall = 0;
	}

}

void playerTurnChange()
{
	if (playerTurn == totalPlayers - 1)
	{
		playerTurn = 0;
	}
	else { playerTurn += 1; }
	if (vectorOfPlayers.at(playerTurn).folded == true)
	{
		playerTurnChange();
	}
}

void processInput()
{
	cin >> playerInput;
	switch (playerInput)
	{
	case 1: //Call
		if (vectorOfPlayers.at(playerTurn).toCall > vectorOfPlayers.at(playerTurn).balance)
		{
			allIn(playerTurn);
			vectorOfPlayers.at(playerTurn).toCall = 0;
		}
		else
		{
			vectorOfPlayers.at(playerTurn).balance -= vectorOfPlayers.at(playerTurn).toCall;
			pot += vectorOfPlayers.at(playerTurn).toCall;
			vectorOfPlayers.at(playerTurn).toCall = 0;
		}
		break;

	case 2: //Raise
		int raise;
		cout << "Raise by what amount? : ";
		cin >> raise;

		while (!checkBalance(playerTurn, raise)) //prevent the player from raising more than they can afford
		{
			renderBoard();
			cout << endl << "Raise by what amount? : ";
			cin >> raise;
		}

		vectorOfPlayers.at(playerTurn).balance -= (raise + vectorOfPlayers.at(playerTurn).toCall); //deduct balance by amount raised and require call amount
		pot += (raise + vectorOfPlayers.at(playerTurn).toCall); //Increase pot by amount raised and current call amount
		playerToCall(raise); //Increase all players current call amount by amount raised
		vectorOfPlayers.at(playerTurn).toCall = 0; //set current players call amount to zero (since they just called/rasied)
		break;

	case 3: //Fold
		vectorOfPlayers.at(playerTurn).folded = true;
		break;

	case 4: //Outcome
	{
		string winningPlayer;
		cout << "Who won the hand? : ";
		cin >> winningPlayer;
		for (int i = 0; i <= totalPlayers - 1; i++)
		{
			if (vectorOfPlayers.at(i).name == winningPlayer)
			{
				vectorOfPlayers.at(i).balance += pot;
				pot = 0;

			}
		}

		newHand();
		handleBlinds();
	}
	break;

	case 5: //End
		winner = true;
		break;
	default:
		renderBoard();
		processInput();
		break;
	}
	playerTurnChange();
}

void renderBoard()
{
	system("CLS");
	cout << "----------------------" << endl << "Pot : $" << pot << endl << "----------------------" << endl;
	for (int i = 0; i<totalPlayers; i++)
	{
		cout << "Name : " << vectorOfPlayers.at(i).name << turnIndicate(i) << foldedIndicate(i) << endl;
		cout << "Balance : $" << vectorOfPlayers.at(i).balance << endl;
		cout << blindIndicate(i) << endl << endl;

	}
	cout << "1) check/call" << " <$" << vectorOfPlayers.at(playerTurn).toCall << ">" << "  2) raise  3)fold  4)outcome  5)exit" << endl << "Input : ";
}

int main()
{
	cout << "Players at the table (2-9) : ";
	cin >> totalPlayers;
	cout << "Initial individual player Balance : ";
	cin >> initialBalance;
	cout << "Stakes? (bBlind) : ";
	cin >> bBlind;
	sBlind = ceil(bBlind / 2); //Set small Blind as half of big Blind (rounded up)

	for (int i = 1; i <= totalPlayers; i++)
	{
		string n;

		cout << "Enter Player " << i << " name : ";
		cin >> n;
		Players player(n, initialBalance);
		vectorOfPlayers.push_back(player);
	}

	playerTurn = 0;
	dealer = 1;
	handleBlinds();

	while (winner == false)
	{
		renderBoard();
		processInput();
	}
	cout << "end";
}