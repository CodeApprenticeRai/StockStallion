#include <iostream>
#include <string>
#include <vector>
#include <fstream>


// ### main app class StockStallion is for controlling the state of the app, ###
// ### and making sure that changes persist each time the program is ran     ###
class StockStallion{
	public:
		StockStallion();

		// Login View which handles authorization
		void commandLineLoginView();
		bool authorizeLogin();
		std:: string getUsernameFromUser();
		std:: string getPasswordFromUser();


		bool registerNewUser();
		void registrationSuccess();
		void registrationFailed();

		// Main Menu which allows user to views stocks and perform other actions
		void commandLineMainMenuView();
		void displayOptions();
		int userChoice();
		void viewStocks();






	private:
		// This is for a crude authentication system and must be changed.
		vector <User> userList;
		// or another idea, read usernames and passwords from a file.

}

bool StockStallion::registerNewUser(){
	bool status = false;
	//get user credentials from stdin (create a function to do this)

	//Write to file in SimpleAuth (See tests.cpp)

	// if done successfully status = true

	//Craete user object and add it to userlist

	// return status
}




class User{
public:
	User(string username, string password, string email, double balance);

	void addMoney(double money);
	void removeMoney(double money);
	void closePortfolio(Portfolio portfolio);
	void openPortfolio(Portfolio portfolio);
	/*
	void recoverPassword();
	void recoverUsername();
	*/


private:
	string username;
	string password;
	string email;
	double balance;
	vector<Portfolio> portfolios;
};

User::User(string username, string password, string email, double balance)
{
	this->username = username;
	this->password = password;
	this->email = email;
	this->balance = balance;
}

void User::addMoney(double money)
{
	balance += money;
}

void User::removeMoney(double money)
{
	balance -= money;
}

void User::closePortfolio(Portfolio portfolio)
{
	//Search through portfolio vector for the right portfolio and call the "close()" function on that portfolio
}

void User::openPortfolio(Portfolio portfolio)
{
	//Add portfolio to Portfolios vector
	portfolios.push_back(portfolio);
}


class Portfolio{
public:
  double add_stock(Stock, int);
  double rmv_stock(Stock);
  /* adds/removes a Stock to the stocks vector
     along with an initial # of shares, and
     returns the dollar value of the transaction */

  double close ();
  /* calls rmv_stock on all stocks in the stocks vector,
     and returns the total dolar value */

  void buy_shares(Stock, int);
  void sell_shares(Stock, int);
  /* adds/removes shares from a stock already in the vector,
     and returns the dollar value of the transaction */

  double update_value();
  /* reads the dollar value of each stock in the stocks value
     and returns the total */

  double roi();
  // returns current value divided by invested value


private:
  string name;
  double investedValue;
  double currentValue;
  vector<Stock> stocks;
}

class stock{
    private:
        string stockName;
        string companyName;
        int currentPricePerShare;
        int numberofSharesOwned;


        void setCompanyName(string);
        void setCurrentPrice(string);
        void setNumberOfShares(string);

    public:
        string getCompanyName(string);
        int getCurrentPrice(string);
        int getNumberOfShares(string);

}

stock::stock(string stockName){
    //initializes a stock object using the parameter of its NYSE identifier
    this -> stockName = stockName;
}

//accessor and mutator for company name
void setCompanyName(string stockName){
    //using stockname retrieve company name from API
    string foundFromAPI;
    string companyName = foundFromAPI;
    this-> companyName = foundFromAPI;

}

string getCompanyName(string stockName){
    return this-> companyName;
}

//accessor and mutator for current price
void setCurrentPricePerShare(string stockName){
    //using stockname retrieve current price from API
    int foundFromAPI;
    int currentPricePerShare = foundFromAPI;
    this-> currentPricePerShare = foundFromAPI;

}

string getCurrentPricePerShare(string stockName){
    return this-> currentPricePerShare;
}


//accessor and mutator for number of shares
void setNumberOfShares(string stockName){
    int numberAdded;
    //number added should come from buy (+) and sell (-) number of shares methods in portfolio
    this-> numberofSharesOwned += numberAdded;

}

string getNumberOfShares(string stockName){
    return this-> numberofSharesOwned;
}
