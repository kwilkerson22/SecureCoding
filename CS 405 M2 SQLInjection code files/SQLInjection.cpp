// SQLInjection.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Name: Kenneth Wilkerson
// Course: CS 405

#include <algorithm>
#include <iostream>
#include <locale>
#include <tuple>
#include <vector>
#include <string>

#include "sqlite3.h"

// DO NOT CHANGE
typedef std::tuple<std::string, std::string, std::string> user_record;
const std::string str_where = " where ";

// DO NOT CHANGE
static int callback(void* possible_vector, int argc, char** argv, char** azColName)
{
    if (possible_vector == NULL)
    { // no vector passed in, so just display the results
        for (int i = 0; i < argc; i++) 
        {
            std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
        }
        std::cout << std::endl;
    }
    else
    {
        std::vector< user_record >* rows =
            static_cast<std::vector< user_record > *>(possible_vector);

        rows->push_back(std::make_tuple(argv[0], argv[1], argv[2]));
    }
    return 0;
}

// DO NOT CHANGE
bool initialize_database(sqlite3* db)
{
    char* error_message = NULL;
    std::string sql = "CREATE TABLE USERS(" \
        "ID INT PRIMARY KEY     NOT NULL," \
        "NAME           TEXT    NOT NULL," \
        "PASSWORD       TEXT    NOT NULL);";

    int result = sqlite3_exec(db, sql.c_str(), callback, NULL, &error_message);
    if (result != SQLITE_OK)
    {
        std::cout << "Failed to create USERS table. ERROR = " << error_message << std::endl;
        sqlite3_free(error_message);
        return false;
    }
    std::cout << "USERS table created." << std::endl;

    // insert some dummy data
    sql = "INSERT INTO USERS (ID, NAME, PASSWORD)" \
        "VALUES (1, 'Fred', 'Flinstone');" \
        "INSERT INTO USERS (ID, NAME, PASSWORD)" \
        "VALUES (2, 'Barney', 'Rubble');" \
        "INSERT INTO USERS (ID, NAME, PASSWORD)" \
        "VALUES (3, 'Wilma', 'Flinstone');" \
        "INSERT INTO USERS (ID, NAME, PASSWORD)" \
        "VALUES (4, 'Betty', 'Rubble');";

    result = sqlite3_exec(db, sql.c_str(), callback, NULL, &error_message);
    if (result != SQLITE_OK)
    {
        std::cout << "Data failed to insert to USERS table. ERROR = " << error_message << std::endl;
        sqlite3_free(error_message);
        return false;
    }

    return true;
}

/*
The run_query function has SQL injection detection
This function executes a SQL query on the given SQLite database and fills
the records vector with results while preventing simple "OR value=value" injection.
*/
bool run_query(sqlite3* db, const std::string& sql, std::vector< user_record >& records)
{
    records.clear();

    // --- SQL Injection Detection ---
    // convert query to lowercase to remove case sensitivity
    std::string lower_sql = sql;
    std::transform(lower_sql.begin(), lower_sql.end(), lower_sql.begin(), ::tolower);

    // looks for the patter "OR" in the query
    size_t or_pos = lower_sql.find(" or ");
    if (or_pos != std::string::npos)
    {
        // looks for an "=" after the "OR"
        size_t equal_pos = lower_sql.find("=", or_pos);
        if (equal_pos != std::string::npos)
        {
            //extract left and right sides of equality
            std::string left = lower_sql.substr(or_pos + 4, equal_pos - (or_pos + 4));
            std::string right = lower_sql.substr(equal_pos + 1);

            // remove trailing semicolon and spaces
            right.erase(std::remove(right.begin(), right.end(), ';'), right.end());
            left.erase(std::remove(left.begin(), left.end(), ' '), left.end());
            right.erase(std::remove(right.begin(), right.end(), ' '), right.end());

            // if both sides are identical, found a "OR value=value" attack
            if (!left.empty() && left == right)
            {
                // alert user of SQL injection
                std::cout << "[ALERT] SQL Injection attempt detected: '" << sql << "'" << std::endl;
                std::cout << "Query aborted for safety." << std::endl;
                return false;
            }
        }
    }

    sqlite3_stmt* stmt = nullptr;

    // safely prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        // if preparation fails print error message and abort
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // execute and collect rows
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        // extract columns as test
        const unsigned char* c0 = sqlite3_column_text(stmt, 0);
        const unsigned char* c1 = sqlite3_column_text(stmt, 1);
        const unsigned char* c2 = sqlite3_column_text(stmt, 2);

        // convert C-style string to std::string
        std::string s0 = c0 ? reinterpret_cast<const char*>(c0) : "";
        std::string s1 = c1 ? reinterpret_cast<const char*>(c1) : "";
        std::string s2 = c2 ? reinterpret_cast<const char*>(c2) : "";

        // add the row to results vector
        records.emplace_back(s0, s1, s2);
    }

    // check for errors during execution
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // finalize statement to free memory
    sqlite3_finalize(stmt);
    // success
    return true;
}

// DO NOT CHANGE
bool run_query_injection(sqlite3* db, const std::string& sql, std::vector< user_record >& records)
{
    std::string injectedSQL(sql);
    std::string localCopy(sql);

    std::transform(localCopy.begin(), localCopy.end(), localCopy.begin(), ::tolower);
    if(localCopy.find(str_where) != std::string::npos)
    {
        if(localCopy.back() == ';')
        {
            injectedSQL.pop_back();
        }

        switch (rand() % 4)
        {
        case 1:
            injectedSQL.append(" or 2=2;");
            break;
        case 2:
            injectedSQL.append(" or 'hi'='hi';");
            break;
        case 3:
            injectedSQL.append(" or 'hack'='hack';");
            break;
        case 0:
        default:
            injectedSQL.append(" or 1=1;");
            break;
        }
    }

    return run_query(db, injectedSQL, records);
}

// DO NOT CHANGE
void dump_results(const std::string& sql, const std::vector< user_record >& records)
{
    std::cout << std::endl << "SQL: " << sql << " ==> " << records.size() << " records found." << std::endl;

    for (auto record : records)
    {
        std::cout << "User: " << std::get<1>(record) << " [UID=" << std::get<0>(record) << " PWD=" << std::get<2>(record) << "]" << std::endl;
    }
}

// DO NOT CHANGE
void run_queries(sqlite3* db)
{
    std::vector< user_record > records;

    // query all
    std::string sql = "SELECT * from USERS";
    if (!run_query(db, sql, records)) return;
    dump_results(sql, records);

    //  query 1
    sql = "SELECT ID, NAME, PASSWORD FROM USERS WHERE NAME='Fred'";
    if (!run_query(db, sql, records)) return;
    dump_results(sql, records);

    //  run query 1 with injection 5 times
    for (auto i = 0; i < 5; ++i)
    {
        if (!run_query_injection(db, sql, records)) continue;
        dump_results(sql, records);
    }
}

// You can change main by adding stuff to it, but all of the existing code must remain
int main()
{
    srand(time(nullptr));

    int return_code = 0;
    std::cout << "SQL Injection Example" << std::endl;

    sqlite3* db = NULL;

    int result = sqlite3_open(":memory:", &db);

    if(result != SQLITE_OK)
    {
        std::cout << "Failed to connect to the database and terminating. ERROR=" << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    std::cout << "Connected to the database." << std::endl;

    if(!initialize_database(db))
    {
        std::cout << "Database Initialization Failed. Terminating." << std::endl;
        return_code = -1;
    }
    else
    {
        run_queries(db);
    }

    if(db != NULL)
    {
        sqlite3_close(db);
    }

    return return_code;
}
