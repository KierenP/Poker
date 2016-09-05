#include "Player.h"

Player::~Player()
{
}

Player::Player(string name, int balance)
{
	m_Name = name;
	m_Balance = balance;
	m_Folded = false;
	m_ToCall = 0;
	m_AllIn = false;
}

int Player::AllIn()
{
	int balance = m_Balance;
	m_Balance = 0;
	m_AllIn = true;
	return balance;
}

int Player::Call(int raise)
{
	int bet = m_ToCall + raise;

	if (bet < m_Balance)
	{
		m_Balance -= bet;
		m_ToCall -= bet;
		return bet;
	}
	else
		return AllIn();
}
