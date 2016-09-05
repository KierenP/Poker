#include "Card.h"

Card::Card(Rank rank, Suit suit)
{
	m_Rank = rank;
	m_Suit = suit;
}


Card::~Card()
{
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
	std::string ranks[NUM_RANKS] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
	std::string suits[NUM_SUITS] = { "c", "d", "h", "s" };

	os << ranks[card.GetRank()] << suits[card.GetSuit()];
	return os;
}


