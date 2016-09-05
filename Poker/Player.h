#pragma once
#include <string>

using namespace::std;

class Player {
public:
	Player(string name, int balance);
	~Player();

	int AllIn();											//Return the balance and set it to 0
	int Call(int raise = 0);								//Call and raise (if passed) and return the bet
	void AddToCall(int val) { m_ToCall += val; }			//add an amount to ToCall
	void AddBalance(int balance) { m_Balance += balance; }

	string GetName() { return m_Name; }
	int GetBalance() { return m_Balance; }
	int GetToCall() { return m_ToCall; }
	bool GetIsFolded() { return m_Folded; }
	bool GetAllInFlag() { return m_AllIn; }

	void SetName(string name) { m_Name = name; }
	void SetBalance(int balance) { m_Balance = balance; }
	void SetToCall(int call) { m_ToCall = call; }
	void SetIsFolded(bool folded) { m_Folded = folded; }
	void SetAllInFlag(bool flag) { m_AllIn = flag; }

private:
	string m_Name;
	int m_Balance;
	int m_ToCall;
	bool m_Folded;
	bool m_AllIn;
};

