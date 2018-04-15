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
            case 2:
                StockStallion::registerNewUser();
            case 3:
                std::cout << "Thank you for using Stock Stallion!\n\n";
                exit(0);
        }
    }
}


void StockStallion::portfolioView(){
    std::cout << "\n\n Welcome to the StockStallion Main Menu!\n\n";

    User* user = this->loggedInAsUser;


    bool break_condition = false;

    while (!break_condition) {
        int choice = StockStallion::portfolioViewPrompt();

        switch(choice){
            case 1:
                StockStallion::addStock(user->getUsername(), user->getStockList());
                break;
            case 2:
                StockStallion::removeStock();
                break;
            case 3:
                viewStocks();
                break;
            case 4:
                std::cout << "Thank you for using Stock Stallion!\n\n";
                //saveState();
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
      std::cout << "Enter an integer in range 1-2.\n";
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
        std::cout << "[1]\tAdd a Stock\n";
        std::cout << "[2]\tRemove a Stock\n";
        std::cout << "[3]\tView Stock Prices\n";
        std::cout << "[4]\tExit\n\n";
        int choice;
        std:cin >> choice;
        bool validChoice;

        //valdiate input
        validChoice = verifyChoiceInRange(choice, 4);
        //std::cout << validChoice;
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




// USER MANIPULATION FUNCTIONS

struct httpLink {
    std::string base = std::string("https://www.alphavantage.co/query?");
    std::string function = std::string("function=TIME_SERIES_INTRADAY");
    std::string symbol = std::string("symbol=");
    std::string interval = std::string("interval=1min");
    std::string api_key = std::string("apikey=CRINRUHDWCNFTVK2");

};

//checks if the curl request can be made for the given user input
bool StockStallion::curlRequest(std::string ticker){
    CURL *curl;

    httpLink request;

    request.symbol.append(ticker);

    std::string req = request.base + request.function + "&" + request.symbol + "&" + request.interval + "&" + request.api_key;

    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, req.c_str());
        CURLcode curlResult = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);

        if(curlResult !=CURLE_OK){
            return false;
        }
        else{
            return true;
        }
    }
    else{
        return false;
    }


}

std::string StockStallion::curlRequestPrice(std::string ticker){
    CURL *curl;

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

        for(int position = 0; position < resultBody.length();){
            if (resultBody.find("1. open\": \"") != -1)
            {
                position = resultBody.find("1. open\": \"");

                openingPrice = resultBody.at(position+11);
                openingPrice = openingPrice + resultBody.at(position+12) + resultBody.at(position+13) +
                               resultBody.at(position+14) + resultBody.at(position+15) + resultBody.at(position+16)
                               + resultBody.at(position+17);

                return openingPrice;
            }
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}


void StockStallion::addStock(std::string username, std::string stocklist){
    bool checker = true;
    std::string ticker_symbol;

    //error checking for ensuring the stock the user wants to add is valid
    while(checker){
        std::cout << "\nEnter the Ticker Symbol You Would Like to Add: ";
        std::cin >> ticker_symbol;

        if (ticker_symbol.length() > 5 ){
            std::cout <<"\nInvalid symbol, try again\n" << endl;
            checker = true;
        }
        else if(curlRequest(ticker_symbol)){
            std::cout <<"\nInvalid symbol, try again\n" << endl;
            checker = true;
        }
        else{
            checker = false;
        }
    }

    stocklist = stocklist + ticker_symbol + ",";

    //adds stock to user profile and db
    // take username as a param, stocklist (the comma separated string version) as a param
//    Database *db;
//    db = new Database("stockstallion.db");
//    std::string _q = std::string("UPDATE users SET stocklist ='") + stocklist + "' WHERE username='" + username + "';";
//    char *q = &_q[0u];
//
//
//    db->query(q);
//    delete q;
//    db->close();



};
void StockStallion::removeStock(){


};
void StockStallion::viewStocks(){
    //uses curlRequestPrice function for each stock object in list then returns the stock and its price



};


// ################# END OF PROMPTS #################



// ################# DATABASE INTERFACE FUNCTIONS #################

// static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
//    int i;
//    for(i = 0; i<argc; i++) {
//       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
//    return 0;
// }
// Create database when the app runs if it doesn't already exist.
// Test the connection to the db if it does exist.
// void StockStallion::initializeDB(){
//   sqlite3 *db;
//   char *zErrMsg = 0;
//   const char *sql;
//   int rc;
//    sqlite3_stmt *statement;
//
//    rc = sqlite3_open("stockstallion.db", &db);
//
//   if( rc ){
//     fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db) );
//     return;
//   } else {
//     fprintf(stdout, "Opened database successfully\n");
//
//   }
//
//   sql = "CREATE TABLE IF NOT EXITS users(" \
//         "id       INT PRIMARY KEY   NOT NULL," \
//         "username TEXT              NOT NULL" \
//         "password TEXT              NOT NULL);";
//
//   rc = sqlite3_prepare_v2(db, sql, -1, &statement, 0);
////    sqlite3_prepare_v2(database, query, -1, &statement, 0);
//   if( rc != SQLITE_OK ){
//     fprintf(stderr, "SQL error: %s\n", zErrMsg);
//        sqlite3_free(zErrMsg);
//     } else {
//       sqlite3_step(statement);
//       sqlite3_finalize(statement);
//       fprintf(stdout, "Table created successfully\n");
//
//     }
//
//   sqlite3_close(db);
//   return;
// }

void StockStallion::initializeDB(){
    Database *db;
    db = new Database("stockstallion.db");
    char* q0 = "CREATE TABLE IF NOT EXISTS users( id INT , username TEXT PRIMARY KEY NOT NULL, password TEXT NOT NULL, stocklist TEXT);";

    db->query(q0);
    db->close();
}

void StockStallion::saveState(){};
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
//        std::vector<std::string> user_vector = result.at(0);
//        return;
        }
    }

// void StockStallion::verifyLogin(std::string username, std::string password){
//     sqlite3 *db;
//     char *zErrMsg = 0;
//
//     const char *zSql; /* SQL statement, UTF-8 encoded */
//     sqlite3_stmt **ppStmt; /* OUT: Statement handle */
//
//     int rc;
//     char *sql;
//     const char* data = "Callback function called";
//
//     /* Open database */
//     rc = sqlite3_open("stockstallion.db", &db);
//
//     if( rc ) {
//         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//         return;
//     } else {
//         fprintf(stderr, "Opened database successfully\n");
//     }
//
//     std::vector< std::vector < std:: string > > result;
//     for( int i = 0; i < 2; i++ )
//         result.push_back(std::vector< std::string >());
//
//
//     /* Create SQL statement */
//     zSql = "SELECT * from users";
//
////     int sqlite3_prepare_v2(*db, *zSql, sqlite3_stmt **ppStmt);
//     /* Execute SQL statement */
//     rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
//
//     if( rc != SQLITE_OK ) {
//         fprintf(stderr, "SQL error: %s\n", zErrMsg);
//         sqlite3_free(zErrMsg);
//     } else {
//         fprintf(stdout, "Operation done successfully\n");
//     }
//
//
//     sqlite3_close(db);
//     return;
// }
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
    return true;
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
