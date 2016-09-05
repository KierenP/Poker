#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <string>
#include "Player.h"

int pot;
int bBlindBet;
int sBlindBet;
bool running = true;

vector<Player> Players;

vector<Player>::iterator dealer;
vector<Player>::iterator sBlind;
vector<Player>::iterator bBlind;
vector<Player>::iterator Current;

void renderBoard();
void PlayersAddToCall(int raise);
void processInput();
void playerTurnChange();
void newHand();
void DeductBlinds();
bool CheckAllFold();
void GetWinner();
void GetRaise();
string turnIndicate(vector<Player>::iterator iter);
string blindIndicate(vector<Player>::iterator iter);
string foldedIndicate(vector<Player>::iterator iter);
string GetNewName(int number);
void PopulateList(unsigned int num, int balance);

//Returns the next iterator, checking for wrap arond
vector<Player>::iterator Next(vector<Player>::iterator iter)
{
	iter++;
	if (iter == Players.end())
	{
		return Players.begin();
	}
	return iter;
}

//Returns the previous iterator, checking for wrap arond
vector<Player>::iterator Prev(vector<Player>::iterator iter)
{
	if (iter == Players.begin())
	{
		return Players.end();
	}
	return iter--;
}

string foldedIndicate(vector<Player>::iterator iter)
{
	if (iter->GetIsFolded())
	{
		return " Folded";
	}
	return "";
}

//Returns true of all people have folded, false if not
bool CheckAllFold()
{
	for (vector<Player>::iterator iter = Players.begin(); iter != Players.end(); iter++)
	{
		if (!iter->GetIsFolded())
			return false;
	}
	return true;
}

//Add's raise to all players 'To Call'
void PlayersAddToCall(int raise)
{
	for (vector<Player>::iterator iter = Players.begin(); iter != Players.end(); iter++)
	{
		iter->AddToCall(raise);
	} 
}

string blindIndicate(vector<Player>::iterator iter)
{
	if (iter == dealer && iter == bBlind)
	{
		return "    Dealer, Big Blind";
	}
	if (iter == dealer)
	{
		return "    Dealer";
	}
	if (iter == sBlind)
	{
		return "    Small Blind";
	}
	if (iter == bBlind)
	{
		return "    Big Blind";
	}
	return "";
}

void DeductBlinds()
{
	pot += sBlind->Call(sBlindBet);
	pot += bBlind->Call(bBlindBet);

	PlayersAddToCall(bBlindBet);
}

string turnIndicate(vector<Player>::iterator iter) //Displays a ~ next to the name of the player whos turn it is
{
	if (iter == Current)
	{
		return " ~";
	}
	return "";
}

void newHand()
{
	/*
		I couldn't find a work around for invalidating vectors, so I save dealer from a iterator to a index, then back from a index to a iterator after deletion of elements
	*/

	unsigned int dealerIndex = 0;

	for (vector<Player>::iterator iter = Players.begin(); iter != Players.end(); iter++)
	{
		if (dealer == iter)
		{
			dealerIndex = Players.begin() - iter;				//Save the index of the dealer once we find it
		}

		if (iter->GetBalance() <= 0)
		{
			if (dealerIndex > Players.begin() - iter)			//This will only happen after the dealer has been saved so its safe (dealer = iter before dealer > iter)
				dealerIndex--;									//If dealer is to be shifted, move it so it keeps track of the same person

			iter = Players.erase(iter);							//iter will now point to the element after the one it erased
		}
		else													//Else is important as we dont want to edit what iter points to after its erased what it pointed to
		{
			iter->SetIsFolded(false);
			iter->SetToCall(0);
		}
	}

	dealer = Players.begin() + dealerIndex;						//Turn the index back into the iterator

	sBlind = Next(dealer);
	bBlind = Next(sBlind);

	Current = dealer;											//will be moved to left of dealer in PlayerTurnChange()

	if (Next(Current) == Current)
	{
		running = false;
	}

	DeductBlinds();
}

void playerTurnChange()
{
	if (CheckAllFold())
	{
		return;							//Saftey check to avoid infinite loop
	}

	do
	{
		Current = Next(Current);
	} while (Current->GetIsFolded());
}

void GetRaise()
{
	int raise;
	cout << "Raise by what amount? : ";
	cin >> raise;

	int bet = Current->Call(raise);
	pot += bet;
	PlayersAddToCall(bet - Current->GetToCall()); //Increase all players current call amount by amount raised
}

void processInput()
{
	int playerInput;
	cin >> playerInput;
	switch (playerInput)
	{
	case 1: //Call
		pot += Current->Call();
		break;
	case 2: //Raise
		GetRaise();
		break;
	case 3: //Fold
		Current->SetIsFolded(true);
		break;
	case 4: //Outcome
		GetWinner();
		break;
	case 5: //End
		running = false;
		break;
	default:
		renderBoard();
		processInput();
		break;
	}

	if (CheckAllFold())
		GetWinner();
	playerTurnChange();
}

void GetWinner()
{
	bool FoundWinner = false;
	do
	{
		string winningPlayer;
		cout << "Who won the hand? : ";
		cin >> winningPlayer;

		for (vector<Player>::iterator iter = Players.begin(); iter != Players.end(); iter++)
		{
			if (winningPlayer == iter->GetName())
			{
				FoundWinner = true;
				iter->AddBalance(pot);
				pot = 0;
			}
		}
	} while (FoundWinner == false);	//if you enter a name that isnt there, it will ask again

	newHand();
}

void renderBoard()
{
	system("CLS");
	cout << "----------------------" << endl << "Pot : $" << pot << endl << "----------------------" << endl;
	
	for (vector<Player>::iterator iter = Players.begin(); iter != Players.end(); iter++)
	{
		if (!iter->RemovedFromGame())
		{
			cout << "Name : " << iter->GetName() << turnIndicate(iter) << foldedIndicate(iter) << endl;
			cout << "Balance : $" << iter->GetBalance() << endl;
			cout << blindIndicate(iter) << endl << endl;
		}
	}

	cout << "1) check/call" << " <$" << Current->GetToCall() << ">" << "  2) raise  3)fold  4)outcome  5)exit" << endl << "Input : ";
}

int main()
{
	int PlayerCount;
	int StartBalance;

	cout << "Players at the table (2-9) : ";
	cin >> PlayerCount;
	cout << "Initial individual player Balance : ";
	cin >> StartBalance;
	cout << "Stakes? (bBlind) : ";
	cin >> bBlindBet;
	sBlindBet = ceil(bBlindBet / 2); //Set small Blind as half of big Blind (rounded up)

	PopulateList(PlayerCount, StartBalance);

	dealer = Players.begin();
	Current = Next(dealer);		//Make the first to play the guy after next
	sBlind = Next(dealer);
	bBlind = Next(sBlind);

	DeductBlinds();

	while (running)
	{
		renderBoard();
		processInput();
	}
}

void PopulateList(unsigned int num, int balance)
{
	for (unsigned int i = 0; i < num; i++)
	{
		Player temp(GetNewName(i), balance);
		Players.push_back(temp);
	}
}

string GetNewName(int number)
{
	string name;
	cout << "Enter Player " << number + 1 << "'s name : ";
	cin >> name;

	return name;
}