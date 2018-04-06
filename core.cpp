#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <afxres.h>
#include "./sqlite/sqlite3.h"
#include "StockStallion.h"
#include "StockStallion.cpp"




int main(){
//  system { "title Stock Stallion: Stocks Made Simple" };
//  system { "cls" };
//  system { "color 9f" };

//    int bytes = GetModuleFileName(NULL, pBuf, len);
//    if(bytes == 0)
//        return -1;
//    else
//        return bytes;


  StockStallion app = StockStallion();

  app.commandLineLoginRegisterView();

  // StockStallion::User user = app.commandLineLoginRegisterView();

  // app.commandLineMainMenuView( user )

  return 0;
}
