#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../sqlite/sqlite3.h"
#include "../StockStallion.h"

// template <typename T>
// T requestInputFromUser(const std::string &userPrompt){
//     std::cout << userPrompt << "\n> ";
//     T out = T();
//     while (!(std::cin >> out)) {
//         std::cin.clear();
//         std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
//         std::cout << "Error!" "\n";
//         std::cout << userPrompt << "\n> ";
//     }
//     return out;
// }


// Hello World




int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("test.db", &db);
2
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
   sqlite3_close(db);
}









// int main(){

  // ### Simple Authentication Registration Test  ### //
  // std::ofstream exampleAuthFile;

  // std::string exampleUsername = "GregGNV101";
  // std::string examplePassword = "hello";


  // std:: string filename = "./simpleAuth/" + exampleUsername + ".txt";
  // exampleAuthFile.open(filename);

  //  if (exampleAuthFile.is_open()){
  // exampleAuthFile << examplePassword << "\n";
//   }
  // std::cout << filename;
  // ### END OF Simple Authentication Registration Test  ### //

  //   int choice = requestInputFromUser("\nChoose an option:\n\n" "[1]\tLogin\n" "[2]\tRegister\n" );
  //registration needs to make sure that entry doesn't already exist and that




// #### GET CWD ########333

//
//#include <stdio.h>  /* defines FILENAME_MAX */
//#ifdef WINDOWS
//#include <direct.h>
//    #define GetCurrentDir _getcwd
//#else
//#include <unistd.h>
//#define GetCurrentDir getcwd
//#endif
//


//char cCurrentPath[FILENAME_MAX];
//
//if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
//{
//return errno;
//}
//
//cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
//
//printf ("The current working directory is %s", cCurrentPath);
//
