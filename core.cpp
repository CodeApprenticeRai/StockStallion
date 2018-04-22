#include <iostream>
#include <string>
#include <vector>
#include <fstream>
//#include <afxres.h>
#include "./sqlite/sqlite3.h"
#include "StockStallion.h"
#include "StockStallion.cpp"




int main(){

    StockStallion app = StockStallion();

    app.commandLineLoginRegisterView();

    app.portfolioView();

return 0;
}
