#include <limits>
#include <iostream>
#include "StockStallion.h"
#include <string>
#include "sqlite3.h"

// Constructor: Initiatlizes component and prints Welcome Message.
StockStallion::StockStallion(){
  // initialize db on startup
  // StockStallion::initializeDB();

  std::cout << "Welcome to Stock Stallion!\n\n";
  std::cout << "StockStallion in a system designed for the viewing, analysis and prediction of performance of stocks.\nIt simplifies investment strategy.\n\n";
}


// ################# VIEW FUNCTIONS #################

// Register / Login User else exit
void StockStallion::commandLineLoginRegisterView(){

    // This function can be abstracted as a function for handling all user
    // interaction.
    int choice = StockStallion::loginRegisterPrompt();

    switch(choice){
       case 1:
         // getLoginUsername();
         // getLoginPassword();
         // authorizeLogin();
         break;
       case 2:
         // getRegisterUsername();
         // getRegisterPassword();
         // StockStallion::registerNewUser();
         break;
      case  3:
        std::cout << "Thank you for using Stock Stallion!\n\n";
        exit(0);
   }

    return;
  }
void StockStallion::portfolioView(){};

// ################# END OF VIEWS #################





// ################# PROMPTS #################

// displays loginRegister View options to user and returns their choice
int StockStallion::loginRegisterPrompt(){
  std::cout << "\nChoose an option:\n\n";
  std::cout << "[1]\tLogin\n";
  std::cout << "[2]\tRegister\n";

  int choice;
  bool validChoice;

  //valdiate input
  validChoice = verifyChoiceInRange(choice, 2);
  std::cout << validChoice;
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

// ################# END OF PROMPTS #################



// ################# DATABASE INTERFACE FUNCTIONS #################

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
// Create database when the app runs if it doesn't already exist.
// Test the connection to the db if it does exist.
void StockStallion::initializeDB(){
  sqlite3 *db;
  char *zErrMsg = 0;
  const char *sql;
  int rc;

  rc = sqlite3_open("stockstallion.db", &db);

  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db) );
    return;
  } else {
    fprintf(stdout, "Opened database successfully\n");
  }

  sql = "CREATE TABLE IF NOT EXITS users(" \
        "id       INT PRIMARY KEY   NOT NULL," \
        "username TEXT              NOT NULL" \
        "password TEXT              NOT NULL);";

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    } else {
       fprintf(stdout, "Table created successfully\n");
    }
  sqlite3_close(db);
  return;
}
void StockStallion::addUserToDB(std::string username, std::string password){
  sqlite3 *db;
  char *zErrMsg = 0;
  const char *sql;
  int rc;

  rc = sqlite3_open("stockstallion.db", &db);

  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db) );
    return;
  } else {
    fprintf(stdout, "Opened database successfully\n");
  }

  std::string insertUser = std::string("VALUES (") + "'" + username + "'"  + "," + "'" + password + "');";

  sql = ("INSERT INTO users(username,password) " + insertUser).c_str();

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    } else {
       fprintf(stdout, "Table created successfully\n");
    }
  sqlite3_close(db);
  return;
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
<<<<<<< HEAD

// I (Tare') couldn't get the template to work.
// If someone can figure this out, it would be useful
// The idea is to use to create a funtion that could
// handle all user in / output.
template <typename T>
T StockStallion::requestInputFromUser(const std::string &userPrompt){
    std::cout << userPrompt << "\n> ";
    T out = T();
    while (!(std::cin >> out)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << "Error!" "\n";
        std::cout << userPrompt << "\n> ";
    }
    return out;
=======
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

  std::cout << "Congratulations! You may have been registered as " << "''" << username << "''.";
  return;
}

void StockStallion::loginRegisterSequence(){
    // templates are constrained such that the implementation
    // and instantiation must happen in the same place (otherwise there will be errors)
    // (still doesn't work )
  int choice = requestInputFromUser("\nChoose an option:\n\n" "[1]\tLogin\n" "[2]\tRegister\n" );
  // StockStallion::loginRegisterPrompt();
  // int choice;
  // bool validChoice;
  // std::cout << "Choice: ";
  // std::cin >> choice;
  //
  // //valdiate input
  // validChoice = verifyChoiceInRange(choice, 2);
  // std::cout << validChoice;
  // while( (std::cin.fail()) or !validChoice ) {
  //   std::cout << "Enter an integer in range 1-2.\n";
  //   std::cin.clear();
  //   std::cin.ignore(256,'\n');
  //   std::cout << "Choice: ";
  //   std::cin >> choice;
  //   validChoice = verifyChoiceInRange(choice, 2);
  // }

  // switch(choice){
  //   case 1:
  //     // authorizeLoginSequence();
  //   case 2:
  //     StockStallion::registerNewUser();
  // }
  return;
>>>>>>> 8b42150a8107a5f748354f6783e313986b583c28
}

// ################# END OF INPUT HANDLING FUNCTIONS #################
