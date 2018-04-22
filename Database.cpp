#include "Database.h"
#include <iostream>
/*
 * A database is a collection of information organized in such a way that it can be easily accessed,
 * managed and updated. Data is organized into rows, columns and tables, and is indexed to render
 * it easier to find information.
 */
Database::Database(char* filename)
{
    /*
     * Constructor creates a database and sets it equal to NULL and then opens the database with name filename.
     */
    database = NULL;
    open(filename);
}

Database::~Database()
{
    /*
     * Nothing really happens
     */
}

bool Database::open(char* filename)
{
    /*
     * Checks to see if the database constructed is good and returns true if successful, else false
     */
    if(sqlite3_open(filename, &database) == SQLITE_OK)
        return true;

    return false;
}

vector<vector<string> > Database::query(char* query)
{
    sqlite3_stmt *statement;
    vector<vector<string> > results;

    if(sqlite3_prepare_v2(database, query, -1, &statement, 0) == SQLITE_OK)
    {
        int cols = sqlite3_column_count(statement);
        int result = 0;
        while(true)
        {
            result = sqlite3_step(statement);

            if(result == SQLITE_ROW)
            {
                vector<string> values;
                for(int col = 0; col < cols; col++)
                {
                    values.push_back((char*)sqlite3_column_text(statement, col));
                }
                results.push_back(values);
            }
            else
            {
                break;
            }
        }

        sqlite3_finalize(statement);
    }

    string error = sqlite3_errmsg(database);
    if(error != "not an error") cout << query << " " << error << endl;

    return results;
}

void Database::close()
{
    /*
     * Closes the database
     */
    sqlite3_close(database);
}