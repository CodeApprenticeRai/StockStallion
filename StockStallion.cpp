#include <limits>
#include <iostream>
#include "StockStallion.h"
#include <string>
#include <vector>
#include "./sqlite/sqlite3.h"
#include "Database.h"
#include "User.h"

// Constructor: Initiatlizes component and prints Welcome Message.
StockStallion::StockStallion(){
  // initialize db on startup
    StockStallion::initializeDB();

  std::cout << "Welcome to Stock Stallion!\n\n";
  std::cout << "StockStallion in a system designed for the viewing, analysis and prediction of performance of stocks.\nIt simplifies investment strategy.\n\n";
}


// ################# VIEW FUNCTIONS #################

// Register / Login User else exit
void StockStallion::commandLineLoginRegisterView(){

    // This function can be abstracted as a function for handling all user
    // interaction.
    bool break_condition = false;

    while (!break_condition) {
        int choice = StockStallion::loginRegisterPrompt();

        switch(choice){
            case 1:
                if( authorizeLogin() ){
                    return;
                }
                break;
            case 2:
                StockStallion::registerNewUser();
                break;
            case 3:
                std::cout << "Thank you for using Stock Stallion!\n\n";
                exit(0);
        }
    }
}

//opens a switch statement that interacts with a menu of options
void StockStallion::portfolioView(){
    std::cout << "\n\n Welcome to the StockStallion Main Menu!\n\n";

    User* user = this->loggedInAsUser;


    bool break_condition = false;

    while (!break_condition) {
        int choice = StockStallion::portfolioViewPrompt();

        switch(choice){
            case 1:
                //returns current price
                StockStallion::currentPrice();
                break;
            case 2:
                //adds a stock to a users list of stock
                StockStallion::addStock();
                break;
            case 3:
                //removes a stock from the users list
                StockStallion::removeStock();
                break;
            case 4:
                //prints out the portfolio
                viewStocks();
                break;
            case 5:
                //exits program
                std::cout << "Thank you for using Stock Stallion!\n\n";
                exit(0);
        }
    }
    return;
}

// ################# END OF VIEWS #################





// ################# PROMPTS #################

// displays loginRegister View options to user and returns their choice
int StockStallion::loginRegisterPrompt()
{
  std::cout << "\nChoose an option:\n\n";
  std::cout << "[1]\tLogin\n";
  std::cout << "[2]\tRegister\n";
    std::cout << "[3]\tExit\n\n";
  int choice;
    std:cin >> choice;
  bool validChoice;

  //valdiate input
  validChoice = verifyChoiceInRange(choice, 3);
  //std::cout << validChoice;
  while( (std::cin.fail()) or !validChoice ) {
      std::cout << "Enter an integer in range 1-3.\n";
      std::cin.clear();
      std::cin.ignore(256, '\n');
      std::cout << "Choice: ";
      std::cin >> choice;
      validChoice = verifyChoiceInRange(choice, 2);
  }
  return choice;
}

int StockStallion::portfolioViewPrompt() {
        std::cout << "\nChoose an option:\n\n";
        std::cout << "[1]\tView Current Stock Prices and indicators\n";
        std::cout << "[2]\tAdd a Stock\n";
        std::cout << "[3]\tRemove a Stock\n";
        std::cout << "[4]\tView Portfolio\n";
        std::cout << "[5]\tExit\n\n";
        int choice;
        std:cin >> choice;
        bool validChoice;

        //valdiate input
        validChoice = verifyChoiceInRange(choice, 5);
        //std::cout << validChoice;
        while( (std::cin.fail()) or !validChoice ) {
            std::cout << "Enter an integer in range 1-5.\n";
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cout << "Choice: ";
            std::cin >> choice;
            validChoice = verifyChoiceInRange(choice, 2);
        }
        return choice;
}




// USER MANIPULATION FUNCTIONS

struct httpLink {
    std::string base = std::string("https://www.alphavantage.co/query?");
    std::string function = std::string("function=TIME_SERIES_INTRADAY");
    std::string symbol = std::string("symbol=");
    std::string interval = std::string("interval=1min");
    std::string api_key = std::string("apikey=CRINRUHDWCNFTVK2");

};


std::string StockStallion::SMA(std::string ticker){
    CURL *curl = curl_easy_init();

    httpLink request;

    request.function = "function=SMA";
    request.interval = "interval=daily";


    request.symbol.append(ticker);

    std::string req = request.base + request.function + "&" + request.symbol + "&" +
                      request.interval + "&" + "time_period=30" + "&series_type=close&" + request.api_key;
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, req.c_str());

        std::string resultBody { };
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resultBody);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<size_t (__stdcall*)(char*, size_t, size_t, void*)>(
                [](char* ptr, size_t size, size_t nmemb, void* resultBody){
                    *(static_cast<std::string*>(resultBody)) += std::string {ptr, size * nmemb};
                    return size * nmemb;
                }
        ));

        CURLcode curlResult = curl_easy_perform(curl);

        if(curlResult !=CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(curlResult));
        }

        //finds ands returns recent sma
        std::string SMA;
        std::string resultBody2;
        int position2 = 0;

        for(int position = 0; position < resultBody.length(); position++){
            if(resultBody.find("\"SMA\": \"") != -1){
                position = resultBody.find("\"SMA\": \"");

                resultBody2 = resultBody.substr(position+8, resultBody.length()-1);
                position2 = resultBody2.find(".");

                SMA = resultBody2.substr(0,position2+3);

                /* always cleanup */
                curl_easy_cleanup(curl);
                return SMA;
            }
        }

        curl_easy_cleanup(curl);
    }
}


std::string StockStallion::curlRequestPrice(std::string ticker){
    CURL *curl = curl_easy_init();

    httpLink request;

    request.symbol = "symbol=";

    request.symbol.append(ticker);

    std::string req = request.base + request.function + "&" + request.symbol + "&" + request.interval + "&" + request.api_key;
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, req.c_str());

        std::string resultBody { };
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resultBody);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<size_t (__stdcall*)(char*, size_t, size_t, void*)>(
                [](char* ptr, size_t size, size_t nmemb, void* resultBody){
                    *(static_cast<std::string*>(resultBody)) += std::string {ptr, size * nmemb};
                    return size * nmemb;
                }
        ));

        CURLcode curlResult = curl_easy_perform(curl);

        if(curlResult !=CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(curlResult));
        }

        //Finds and returns the current price of the stock
        std::string openingPrice;
        std::string resultBody2;
        int position2 = 0;

        for(int position = 0; position < resultBody.length(); ++position){
            if (resultBody.find("1. open\": \"") != -1)
            {
                position = resultBody.find("1. open\": \"");

                resultBody2 = resultBody.substr(position+11, resultBody.length()-1);
                position2 = resultBody2.find(".");


                openingPrice = resultBody2.substr(0,position2+3);

                /* always cleanup */
                curl_easy_cleanup(curl);
                return openingPrice;
            }
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
        return std::string("");
    }
}

//returns the current price of a given stock
void StockStallion::currentPrice() {
    bool checker = true;
    std::string ticker_symbol;

    //error checking for ensuring the stock the user wants to add is valid
    while(checker){
        std::cout << "\nEnter the Ticker Symbol of the Stock whose price you would like to check: ";
        std::cin >> ticker_symbol;

        //dealing with bad input
        if(cin.get() == ' '){
            // allows program to ignore everything after the first space
            while (cin.get() != '\n')
            {
                cin.clear();
            }
            cout << "\nInvalid symbol, try again\n" << endl;
            checker = true;
        }

        else if (ticker_symbol.length() > 5 ){
            std::cout <<"\nInvalid symbol, try again\n" << endl;
            checker = true;
        }
        else if( (std::string("") == curlRequestPrice(ticker_symbol)) ){
            std::cout <<"\nInvalid symbol, try again\n" << endl;
            checker = true;
        }
        else{
            checker = false;
        }
    }


    std::string price;
    std::string sma;

    price = StockStallion::curlRequestPrice(ticker_symbol);
    sma = StockStallion::SMA(ticker_symbol);

    double x1 = stoi(sma);
    double x2 = stoi(price);

    std::cout << "\nThe current price of " << ticker_symbol << " is: $" << price << endl;

    std::cout << "\nThe average price of " << ticker_symbol << " over the last 30 days is: $"
                                                               << setprecision(3) << sma << endl;

    std::cout << ticker_symbol << " Change from Average: " << percentChange(x1, x2) << "%" << endl;

}


void StockStallion::addStock(){
    std::string username = loggedInAsUser->getUsername();

    bool checker = true;
    std::string ticker_symbol;

    //error checking for ensuring the stock the user wants to add is valid
    while(checker){
        std::cout << "\nEnter the Ticker Symbol You Would Like to Add: ";
        std::cin >> ticker_symbol;

        //dealing with bad input
        if(cin.get() == ' '){
            // allows program to ignore everything after the first space
            while (cin.get() != '\n')
            {
                cin.clear();
            }
            cout << "\nInvalid symbol, try again\n" << endl;
            checker = true;
        }
        else if (ticker_symbol.length() > 5 ){
            std::cout <<"\nInvalid symbol, try again\n" << endl;
            checker = true;
        }
        else if( (std::string("") == curlRequestPrice(ticker_symbol)) ){
            std::cout <<"\nCould not complete request, try again\n" << endl;
            checker = true;
        }
        else{
            checker = false;
        }
    }

    //mechanics for adding multiple shares
    checker = true;
    int numShares = 0;
    bool check = false;

    while(checker){
        std::cout << "\n**NOTE: number of shares for one add is capped at 100**" << endl;
        std::cout << "\nHow many shares of " << ticker_symbol << " would you like to add? " << endl;

        std::cin >> numShares;

        //dealing with bad input
        if(cin.get() == ' '){
            // allows program to ignore everything after the first space
            while (cin.get() != '\n')
            {
                std::cin.clear();
            }
            cout << "\nInvalid symbol, try again\n" << endl;
            return;
        }

        bool validChoice;

        validChoice = verifyChoiceInRange(numShares, 100);
        while( (std::cin.fail()) or !validChoice ) {
            checker =  true;
            std::cout << "\nInvalid input, try again" << endl;
            if(numShares == 0){
                std::cout << "\nNo shares of " << ticker_symbol << " were added" << endl;
                checker = false;
                return;
            }
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cout << "\nEnter a number between 0 and 100: ";
            std::cin >> numShares;
            validChoice = verifyChoiceInRange(numShares, 100);
        }

        if(validChoice){
            checker =  false;
        }

    }

    //put string of ticker symbols, the price they were bought at, and the number of shares into the database
    std::string currentPrice = curlRequestPrice(ticker_symbol);
    if(currentPrice == ""){
        while(currentPrice == ""){
            currentPrice = curlRequestPrice(ticker_symbol);
        }
    }

    if(numShares > 0){
        this->loggedInAsUser->appendStockList(ticker_symbol, currentPrice, std::to_string(numShares));
        std::string stockList = loggedInAsUser->getStockList();

        //adds stock to user profile and db
        // take username as a param, stocklist (the comma separated string version) as a param
        Database *db;
        db = new Database("stockstallion.db");
        std::string _q = std::string("UPDATE users SET stocklist ='") + stockList  + "' WHERE username='" + username + "';";
        char *q = &_q[0u];

        db->query(q);
//    delete q;
        db->close();
    }


    return;
};

//menu for yes or no with error catching
int StockStallion::yesNoMenu(){
    std::cout << "[1]\tYes\n";
    std::cout << "[2]\tNo\n";

    int choice;
    std:cin >> choice;
    bool validChoice;


    //valdiate input
    validChoice = verifyChoiceInRange(choice, 2);
    while( (std::cin.fail()) or !validChoice ) {
        std::cout << "Enter an integer in range 1-2.\n";
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cout << "Choice: ";
        std::cin >> choice;
        validChoice = verifyChoiceInRange(choice, 2);
    }

    return choice;
}

//talks with user to remove a certain number of shares from a particular purchase
void StockStallion::removeStock(){
    std::string ticker_symbol;
    std::string stocks = loggedInAsUser->getStockList();

    bool checker = true;
    bool progression = false;

    int numShares = 0;

    if(stocks == ""){
        std::cout << "\nPortfolio is empty, add stocks before removing them\n";
        checker = false;
    }

    while(checker){
        std::cout << "Enter the ticker symbol of the stock you would like to remove: ";
        std:: cin >> ticker_symbol;

        if(cin.get() == ' '){
            // allows program to ignore everything after the first space
            while (cin.get() != '\n')
            {
                std::cin.clear();
                std::cin.ignore(256, '\n');
            }
            cout << "\nInvalid symbol, try again\n" << endl;
            checker = true;
        }
        else if(stocks.find(ticker_symbol) == -1){
            std::cout << "\nNot in your portfolio, try again\n" << endl;
            checker = true;
        }
        else{
            checker = false;
        }
    }

    checker = true;
    std::string firstPrice;
    std::istringstream stream(stocks);
    std::string line;
    bool repeated = false;

    while(std::getline(stream, line)){
        //find name
        std::string compName;
        char until(' ');
        compName = line.substr(0, line.find(until));

        if(compName == ticker_symbol){
            if(repeated){
                std::cout << "\nOther additions of " << ticker_symbol <<  " were found" << endl;
            }

            //find price bought at
            int pos1 = line.find(" at ");
            firstPrice = line.substr(pos1+4, (line.find(' ')+1 - pos1+4));

            //find original numShares
            int pos0 = line.find(" x");
            std:string numOwned = line.substr(pos0+2, (line.find(' ')+1 - pos0+2));
            numShares = stoi(numOwned);

            std::cout << "\nYou bought " << numShares << " shares of " << ticker_symbol << " at " << firstPrice << endl;
            std::cout << "Would you like to remove shares from this purchase?\n " << endl;

            int selection = yesNoMenu();

            if(selection == 1){
                progression = true;
                break;
            }
            else{
                progression = false;
                repeated = true;
                std::cout << "\nChecking for other purchases of: " << ticker_symbol << endl;
            }

        }
    }

    if(progression){
        checker = true;
        int numSharesToRemove = 0;

        while(checker){
            std::cout << "Enter how many shares of " << ticker_symbol
                      << " bought at " << firstPrice << " you would like to remove " << endl;
            std::cin >> numSharesToRemove;

            bool validChoice;

            validChoice = verifyChoiceInRange(numSharesToRemove, numShares);
            while( (std::cin.fail()) or !validChoice ) {
                checker =  true;
                std::cout << "\nInvalid input, try again" << endl;
                if(numShares == 0){
                    std::cout << "\nNo shares of " << ticker_symbol << " were removed" << endl;
                    return;
                }
                std::cin.clear();
                std::cin.ignore(256, '\n');
                std::cout << "\nEnter a number between 0 and "<< numShares << ": ";
                std::cin >> numShares;
                validChoice = verifyChoiceInRange(numSharesToRemove, numShares);
            }

            if(validChoice){
                checker =  false;
            }

        }

            if(numSharesToRemove > 0){
                this->loggedInAsUser->removeFromStockList(ticker_symbol, firstPrice, std::to_string(numShares), numSharesToRemove);
                std::string stockList = loggedInAsUser->getStockList();

                std::string username = loggedInAsUser->getUsername();

                //updates stocklist attached to user profile and db
                // take username as a param, stocklist (the comma separated string version) as a param
                Database *db;
                db = new Database("stockstallion.db");
                std::string _q = std::string("UPDATE users SET stocklist ='") + stockList  + "' WHERE username='" + username + "';";
                char *q = &_q[0u];

                db->query(q);
                db->close();

            }

    }
    else{
        return;
    }

};

//calculates the percent change between two inputs
double StockStallion::percentChange(double original, double updated) {
        double change;
        change = updated-original;
        change /= updated;
        return 100 * change;
}

//prints out portfolio
void StockStallion::viewStocks(){
    std::string stocks = loggedInAsUser->getStockList();
    std::string username = loggedInAsUser->getUsername();
    std::string currentPrice = "failed";
    double assetGrowth = 0;

    double long portfolioValue = 0;
    double intermediate = 0;

    double portfolioGrowth = 0;
    double originalBuy = 0;

    std::istringstream stream(stocks);
    std::string line;

    if(stocks == ""){
        std::cout << "Portfolio is currently empty" << endl;
    }
    else{
        std::cout << "\n" << username << "'s portfolio: \n" << endl;

        while(std::getline(stream, line)){
            std::string compName;
            char until(' ');
            compName = line.substr(0, line.find(until));

            currentPrice = curlRequestPrice(compName);

            //error catching
            if(currentPrice == ""){
                while(currentPrice == ""){
                    currentPrice = curlRequestPrice(compName);
                }
            }

            //find number of shares
            int pos0 = line.find(" x");
            std::string numOwned = line.substr(pos0+2, (line.find(' ') - pos0+2));
            int numShares = stoi(numOwned);


            //find original buy price
            int pos1 = line.find(" at ");
            std::string firstPrice;
            firstPrice = line.substr(pos1+4, (line.find(' ') - pos1+4));

            double intermediate2 = stod(firstPrice) * numShares;
            originalBuy += intermediate2;


            //find current portfolio value
            intermediate = stod(currentPrice) * numShares;
            portfolioValue += intermediate;

            //find each stocks change
            assetGrowth = percentChange((intermediate2 /numShares), (intermediate/numShares));


            //print out data
            std::cout << fixed;
            std::cout << line << " Current price is: " << currentPrice << setprecision(2)
                      << ", Total Current Value: $" << intermediate <<", Change: "  << assetGrowth << "%" << endl;

        }

        portfolioGrowth = percentChange(originalBuy, portfolioValue);
        char changeSign;

        if(portfolioGrowth >= 0){
            changeSign = '+';
        }
        else{
            changeSign = ' ';
        }

        std::cout << "\nTotal Portfolio value is: $" << portfolioValue << endl;
        std::cout << "\nTotal growth of included securities is: " << changeSign << portfolioGrowth << "%" << endl;
    }




};


// ################# END OF PROMPTS #################



// ################# DATABASE INTERFACE FUNCTIONS #################


void StockStallion::initializeDB(){
    Database *db;
    db = new Database("stockstallion.db");
    char* q0 = "CREATE TABLE IF NOT EXISTS users( id INT , username TEXT PRIMARY KEY NOT NULL, password TEXT NOT NULL, stocklist TEXT);";

    db->query(q0);
    db->close();
}

void StockStallion::addUserToDB(std::string username, std::string password) {
    Database *db;
    db = new Database("stockstallion.db");
    std::string _id = std::to_string( rand() % 1000);
    std::string stock_list = std::string("");
    std::string _q = std::string("INSERT INTO users(id, username, password, stocklist)") + "VALUES(" + "'" + _id + "'" + "," + "'" + username + "'" + "," + "'" + password + "'" + ",'');";
    char *q2 = &_q[0u];
    db->query(q2);
    db->close();
}

bool StockStallion::verifyLogin(std::string username, std::string password) {
    Database *db;
    db = new Database("stockstallion.db");
    std::string _q = std::string("SELECT username, password, stocklist FROM users WHERE username='") + username + "' AND password='" + password +"';";
    char *q = &_q[0u];
    std::vector<std::vector<std::string> > result = db->query(q);

    if (result.empty()){
        std::cout << "Incorrect Username / Password Combination.";
        db->close();
        return false;
    }


    else {
        for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it){
            vector<string> row = *it;
//            std::cout << "'" << row.at(0)  << "'" << "\t" << "'" << row.at(1);
            if((row.at(0) == username) && (row.at(1) == password)){
                std::cout << "Successfully Authenticated, but unfortunately that doesn't really get you to the next step yet.";
                std::string username = row.at(0);
                std::string stock_list = row.at(2);
//                create static user object here
                db->close();
                return true;
            }
        }
        }
    }

// ################# END OF DATABASE INTERFACE FUNCTIONS #################





// ################# INPUT HANDLING FUNCTIONS #################

bool StockStallion::verifyUsername(std::string &username){
  /**
   * string must be at least 10 characters long and contain only
   * numerals or letters
   */
  int integer = 0;
  int chara = 0;
  for(int i = 0; i < username.length(); i++){
      if(isdigit(username[i])){
          integer++;
      }
      else if(isalpha(username[i])){
          chara++;
      }
      else{
          return false;
      }
  }
  if (chara > 1 && integer > 1){
      if(username.length() > 9){
          return true;
      }
  }
  return false;
}
// passwords shouldn't contain spaces.
bool StockStallion::verifyPassword(std::string &password){
  int integer = 0;
  int chara = 0;
  for(int i = 0; i < password.length(); i++){
      if(isdigit(password[i])){
          integer++;
      }
      else if(isalpha(password[i])){
          chara++;
      }
      else{
          return false;
      }
  }
  if (chara > 1 && integer > 1){
      if(password.length() > 7 && password.length() < 21){
          return true;
      }
  }
  return false;
}
bool StockStallion::verifyChoiceInRange(int choice, int max){
  if ( choice < 0){
    return false;
  }
  return (choice <= max);
}
void StockStallion::registerNewUser(){
  std::cout << "\n\n\nWelcome to the Stock Stallion Registration Prompt.";
  // other fields can be added { name, temperament, etc}
  std::string username, password;

  std::cout << "\n\nEnter a username: ";
  std::cin >> username;
  this -> verifyUsername(username);

  std::cout << "\n\nEnter a password: ";
  std::cin >> password;
  this -> verifyPassword(password);
   StockStallion::addUserToDB(username, password);

  std::cout << "Congratulations! You have been registered as " << "''" << username << "''.";
  return;
}

//perhaps return an object instead
bool StockStallion::authorizeLogin(){
    std::cout << "\n\n\nWelcome to the Stock Stallion Login Prompt.";
    // other fields can be added { name, temperament, etc}
    std::string username, password;

    std::cout << "\n\nEnter a username: ";
    std::cin >> username;
    this -> verifyUsername(username);

    std::cout << "\n\nEnter a password: ";
    std::cin >> password;
    this -> verifyPassword(password);
    bool verified = StockStallion::verifyLogin(username, password);

    if (verified){
        buildUserObject(username);
    }
    return verified;
}

void StockStallion::buildUserObject(std::string username){
    Database *db;
    db = new Database("stockstallion.db");
    std::string _q = std::string("SELECT username, password, stocklist FROM users WHERE username='") + username + "'";
    char *q = &_q[0u];
    std::vector<std::vector<std::string> > result = db->query(q);

    vector<std::string> userInfo = result.at(0);

    std::string stockList = userInfo.at(2);

    this->loggedInAsUser = new User(username, stockList);

    return;
}
// ################# END OF INPUT HANDLING FUNCTIONS #################
