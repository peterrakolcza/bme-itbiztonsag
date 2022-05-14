#include <iostream>
#include <sqlite3.h>
using namespace std;
// Call-back function for the execution of SQL queries
// (out_data can be used to return data to the caller function)
static int callback(void *out_data, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
        cout << (argv[i] ? argv[i] : "NULL") << endl;
    return 0;
}
int main(int argc, char **argv)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    // Open database (local file)
    if (sqlite3_open("adult.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }
    // Execute SQL statement
    if (sqlite3_exec(db, "SELECT salary FROM adult;", callback, 0, &zErrMsg) != SQLITE_OK)
    {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    return 0;
}