#include <iostream>
#include <sqlite3.h>
#include <armadillo>
#include <fstream>
#include <cmath>

using namespace std;

void print_mat(arma::mat my_matrix)
{

    uint cols = my_matrix.n_cols;
    uint rows = my_matrix.n_rows;

    cout << "--------\n";
    for (uint rX = 0; rX < rows; rX++)
    {
        //cout << " " << rX << ": ";
        for (uint cX = 0; cX < cols; cX++)
        {
            cout << my_matrix(rX, cX) << " ";
        }
        cout << "\n";
    }
    cout << "--------\n";
}

void tokenize(std::string const &str, const char delim, std::vector<std::string> &out)
{
    // construct a stream from the string
    std::stringstream ss(str);

    std::string s;
    while (std::getline(ss, s, delim))
    {
        out.push_back(s);
    }
}

// Call-back function for the execution of SQL queries
// (out_data can be used to return data to the caller function)
static int callback(void *out_data, int argc, char **argv, char **azColName)
{
    arma::rowvec *out_data_mat = (arma::rowvec *)out_data;
    
    for (int i = 0; i < argc; i++)
    {
        (*out_data_mat)[atoi(argv[i])] = 1;
    }

    return 0;
}

static int callback2(void *out_data, int argc, char **argv, char **azColName)
{
    arma::mat *out_data_mat = (arma::mat *)out_data;
    for (int i = 0; i < argc; i++)
    {
        *out_data_mat << atoi((argv[i] ? argv[i] : "-1")) << arma::endr;
    }
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

    // arma::endr represents the end of a row in a matrix
    arma::mat A;
    A.set_size(4000, 1000);
    A.fill(0);
    arma::mat x;
    x.set_size(1, 1000);
    x.fill(0);
    arma::mat b;
    b.set_size(1, 4000);
    b.fill(0);

    std::ifstream file("queries2.sql");
    std::string str;
    int c = 0;
    while (std::getline(file, str))
    {
        std::vector<std::string> out;
        tokenize(str, ';', out); // ; is lost from the SQL query, because of splitting

        b.at(0, c) = stoi(out[1]);

        out[0] += ';';
        arma::rowvec temp(1000, arma::fill::zeros);
        // Execute SQL statement
        if (sqlite3_exec(db, out[0].data(), callback, &temp, &zErrMsg) != SQLITE_OK)
        {
            cerr << "SQL error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg);
        }

        A.row(c) = temp;

        if (c == 999) break;
        c++;
    }
    sqlite3_close(db);

    int sum = 0;
    for (int i = 0; i < 4000; i++)
    {
        int count = 0;
        for (int j = 0; j < 1000; j++)
        {
            if (A.at(i, j) == 1) count++;
        }

        //if (count > max) max = count;
        sum += count;
        
    }
    cout << "Atlagos lefedettseg: " << sum / 4000 << endl;

    A = pinv(A);
    b = b.t();

    arma::mat res = A * b;

    for (int i = 0; i < 4000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            res.at(i, j) = round(res.at(i, j));
        }
        
    }

    // x.t();
    print_mat(res);

    return 0;
}