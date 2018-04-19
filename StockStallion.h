#ifndef STOCKSTALLION_H
#define STOCKSTALLION_H
#include <limits>
#include <iostream>
#include <string>
#include "User.h"
#include <curl/curl.h>
#include <sstream>



class StockStallion {
	public:
		StockStallion();

		// Views
		void commandLineLoginRegisterView();

		void portfolioView();

		// Prompts
		int loginRegisterPrompt();
        int portfolioViewPrompt();


        // User Object Manipulation Functions
//        bool curlRequest(std::string);
        std::string curlRequestPrice(std::string);

        void currentPrice();
        void addStock();
        void removeStock();
        void viewStocks();


		// Database Functions
		void initializeDB();

        void saveState();

		void addUserToDB(std::string username, std::string password);

		bool verifyLogin(std::string username, std::string password);

		//templates suck for multi file projects : (
		template<typename T>
		T requestInputFromUser(const std::string &userPrompt);


		bool verifyChoiceInRange(int choice, int max);

		// prompts user with options to login or register
		bool verifyUsername(std::string &username);

		bool verifyPassword(std::string &password);

		//gets user info (username, password), POSTS it to DB.
		void registerNewUser();

		bool authorizeLogin();

		void buildUserObject(std::string username);


	private:
		User *loggedInAsUser = nullptr;

};

#endif
