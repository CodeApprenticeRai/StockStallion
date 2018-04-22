#include <limits>
#include <iostream>
#include "StockStallion.h"
#include <string>
#include <vector>
#include "./sqlite/sqlite3.h"
#include "Database.h"
#include "User.h"
/*
 * StockStallion is the premier choice in checking current prices and creating personal private
 * portfolios for your personal investment choices. Unlike other services that require you to sign up
 * with a bank account StockStallion is free and anonymous so do whatever you like with it without
 * fearing the consequences of blowing your money before the market decides to dive lower than manic depression.
 */

// Constructor: Initializes a database by calling the InitializeDB method and returns a prompt
StockStallion::StockStallion(){
  // initialize db on startup
    StockStallion::initializeDB();

  std::cout << "Welcome to Stock Stallion!\n\n";
  std::cout << "StockStallion in a system designed for the viewing, analysis and prediction of performance of stocks.\nIt simplifies investment strategy.\n\n";
}


// ################# VIEW FUNCTIONS #################

// Register / Login User else exit
/*
 * Creates a boolean value initialized as false and then runs a while loop checking an
 * integer value inputted by the user using the loginRegisterPrompt method. If the user inputs 1
 * then the method calls the authorizeLogin method and breaks out of the loop, if the user instead inputs 2
 * the registerNewUser method will be called and then the loop breaks, else if the user returns 3 the program
 * ends normally.
 */
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
/*
 * The function prints a welcome prompt and uses a switch statement in order which calls
 * the currentPrice, addStock, removeStock, viewStocks, or exit the program.
 */
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
/*
 * Returns a prompt asking the user to Register (1), Login (2), or Exit (3).
 * The method creates an integer value and a boolean and then checks to see if
 * the integer inputted by the user is valid and if so returns the value if valid.
 */
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

/*
 *prints a prompt asking the user to enter what the user would like to do with his portfolio object
 * then it verifies to make the input is corect before returning the value
 */
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

/*
 * Makes a curl request with the selected ticker symbol and reads the json output produced by the curl request.
 * Then finds the most recent simple moving average and returns it as a string.
 */
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

/*
 * Makes a curl request using the selected ticker symbol and then reads the json output produced via
 * the curl request and returns the recent price of the given stock in a string.
 */
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

        std::cout << "\nLoading...\n" << endl;

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

/*
 * Takes the username of the current database and then requests the user to add a stock and checks to make sure
 * the user input is valid. If the user doesn’t input a proper stock name then will loop continuously.
 * Once the stock is verified the method will once again ask the user to input the number of shares that the
 * user desires and if valid then will add the correct number of shares. If the user inputs 0 the method ends
 * immediately, else the method then takes the number of shares of said stock and adds it to the database.
 */
void StockStallion::addStock(){
    std::string username = loggedInAsUser->getUsername();

    bool checker = true;
    std::string ticker_symbol;

    //error checking for ensuring the stock the user wants to add is valid
    while(checker){
        std::cout << "\nEnter the Ticker Symbol You Would Like to Add: ";
        std::cin >> ticker_symbol;

        std::cout << "\nLoading...\n" << endl;

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

        std::cout << "\nAdding...\n" << endl;

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
    int numSharesToRemove = 0;

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
        if(numSharesToRemove == 0){
            std::cout << "\nInvalid input, " << ticker_symbol << " was not removed" << endl;
        }

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

/*
 * Constructs a Database object with with the string literal “stockstallion.db” and then creates another
 * character pointer q0. The method then calls the query method from the current Database object using this
 * new pointer before closing the database.
 */
void StockStallion::initializeDB(){
    Database *db;
    db = new Database("stockstallion.db");
    char* q0 = "CREATE TABLE IF NOT EXISTS users( id INT , username TEXT PRIMARY KEY NOT NULL, password TEXT NOT NULL, stocklist TEXT);";

    db->query(q0);
    db->close();
}

<<<<<<< HEAD
// void StockStallion::addUserToDB(std::string username, std::string password){
//   sqlite3 *db;
//   char *zErrMsg = 0;
//   const char *sql;
//   int rc;
//
//   rc = sqlite3_open("stockstallion.db", &db);
//
//   if( rc ){
//     fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db) );
//     return;
//   } else {
//     fprintf(stdout, "Opened database successfully\n");
//   }
//
//   std::string insertUser = std::string("VALUES (") + "'" + username + "'"  + "," + "'" + password + "');";
//
//   sql = ("INSERT INTO users(username,password) " + insertUser).c_str();
//
//   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
//   if( rc != SQLITE_OK ){
//     fprintf(stderr, "SQL error: %s\n", zErrMsg);
//        sqlite3_free(zErrMsg);
//     } else {
//        fprintf(stdout, "Table created successfully\n");
//     }
//   sqlite3_close(db);
//   return;
// }
=======
/*
 * Constructs a database object, a string with some random value, and then a string with an empty space.
 * Using the two previous strings the method then constructs another string and finally creates a character pointer.
 * Using the character pointer it calls a query method from the current Database using this new pointer before
 * closing the database.
 */
>>>>>>> 6725efed34abc4f904100475522b4a6ce0297788
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
/*
 * Constructs a Database object, a string and a character pointer.
 * Makes a vector of a vector of strings that holds query result(s) from the current Database object.
 * If the vector (result) is empty then it will return a prompt saying incorrect username and combination,
 * closes the database and ends the method. If the vector contains a value then an iterator is constructed and
 * will iterate through the vector checking the values at row(0) and row(1) and matching the username and
 * password and then creating a static object before ending the method
 */
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

/*
 * Creates two integers two count the occurances of characters and integers, the method then loops through
 * the user’s input and will return false if not enough alphabetic characters or integers are entered, if
 * any other characters are inputted, or if the string the user inputted isn’t long enough.
 */
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
//see verifyUsername for function
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

//If the choice is less than 0 or greater than max then the method will return false
bool StockStallion::verifyChoiceInRange(int choice, int max){
  if ( choice < 0){
    return false;
  }
  return (choice <= max);
}

/*
 * Prints a string welcoming the the user to the program and subsequently creates two string variables named
 * username and password. User then inputs a string and verifies if it matches the requirements as stated in
 * the verifyUsername method and does the same with the password using the verifyPassword method. Once both
 * password and username are verified the method calls the addUserToDB method and finally prints a string
 * congratulating the user for registering
 */
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

/*
 * Prints a string welcoming the the user to the program and
 * subsequently creates two string variables named username and password. User then inputs a string and
 * verifies if it matches the requirements as stated in the verifyUsername method and does the same with the
 * password using the verifyPassword method. Once both password and username are verified the method calls the
 * verifyLogin method
 */
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

/*
 *Constructs a database object, a string with some random value, and then a string with an empty space.
 * Using the two previous strings the method then constructs another string and finally creates a character pointer.
 * Using the character pointer it calls a query method from the current Database using this new pointer before
 * closing the database. It then reads from the database looking for the user information and then uses it to
 * construct a user object
 */
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
