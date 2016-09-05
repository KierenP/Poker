#pragma once
#include <string>		

enum Suit {
	CLUB,
	DIAMOND,
	HEART,
	SPADE,
	NUM_SUITS = 4
};

enum Rank
{
	ACE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
	NUM_RANKS = 13
};

class Card
{
public:
	Card(Rank rank = ACE, Suit suit = SPADE);
	~Card();

	Suit GetSuit() const { return m_Suit; }
	Rank GetRank() const { return m_Rank; }
	//std::string GetSuitString();
	//std::string GetRankString();

	void SetSuit(Suit val) { m_Suit = val; }
	void SetRank(Rank val) { m_Rank = val; }
	friend std::ostream& operator<<(std::ostream& os, const Card& card);	//Overload << so that Card can be written to a iostream obj (cout << Card)

private:
	Suit m_Suit;
	Rank m_Rank;
};