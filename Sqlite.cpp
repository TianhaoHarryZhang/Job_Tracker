#include <filesystem>
#include <iostream>
#include <sqlite3.h>
#include "mainwindow.h"
#include <string>

using std::cout;
using std::cin;
using std::endl;


void Database::create_database(sqlite3*& db, const char* filename) {
    int rc;

    // Check that the SQLite library is installed and linked
    cout << "SQLite version: " << sqlite3_libversion() << endl;

    // Check if the database file exists
    std::cout << "Creating or Opening Database at: "<< filename << std::endl;

    if (!std::filesystem::exists(filename)) {
        // Create a new database file if it doesn't exist
        rc = sqlite3_open(filename, &db);
        if (rc != SQLITE_OK) {
            cout << "Error in creating database: " << sqlite3_errmsg(db) << endl;
            return;
        }

        // Close the database
        //sqlite3_close(db);

        cout << "New database created successfully!" << endl;
        return;
    }

    // Open the existing database
    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        cout << "Error opening database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "Opened database successfully!" << endl;
}




void Database::create_table(sqlite3* db) {

    char* ErrorMSG = 0;
    int exec;

    // Check if database was opened successfully
    if (db == nullptr) {
        cout << "Error: Failed to open database" << endl;
        return;
    }

    // Use a prepared statement to create the table
    sqlite3_stmt* stmt;

    const char* sql = "CREATE TABLE Job_List("
                      "ID INTEGER PRIMARY KEY   AUTOINCREMENT, "
                      "Company           TEXT    NOT NULL, "
                      "Position          TEXT     NOT NULL, "
                      "Status            INT     NOT NULL, "
                      "Weblink        TEXT NOT NULL)";


    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        cout << "Error in preparing statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Execute the prepared statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cout << "Error in executing statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Finalize the prepared statement
    sqlite3_finalize(stmt);

    cout << "Table created successfully!" << endl;

    //sqlite3_close(db);
}



void Database::print_database_error(sqlite3* db){

    const char* errmsg = sqlite3_errmsg(db);

    cout<<"Database Error: "<<errmsg<<endl;

}



void Database::add_new_record(sqlite3* db, char* new_record_text[4]){


    char* sql = sqlite3_mprintf("INSERT INTO Job_List (Company, Position, Status, Weblink) VALUES ('%q', '%q', '%q', '%q')",
                                new_record_text[0],new_record_text[1],new_record_text[2],new_record_text[3]);

    int rc = sqlite3_exec(db,sql,NULL,NULL,0);

    if (rc != SQLITE_OK){

        cout<< "Error in inserting record: "<< sqlite3_errmsg(db) << endl;
    }

    else{

        cout<< "New record added successfully!" << endl;
    }

    sqlite3_free((void*)sql);

    /**INSERT INTO my_table (Company, Position, Status, Weblink)
VALUES ('New company', 'New position', 'Unknown Status', 'Unavailable Weblink');**/

}


void Database::delete_record(sqlite3* db, int index){

    // Open a transaction to ensure atomicity of the delete operation
    sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);

    // Prepare the delete statement
    sqlite3_stmt* stmt;
    char* sql = sqlite3_mprintf("DELETE FROM Job_List WHERE ROWID = %d;", index+1);
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Error in preparing delete statement:" << sqlite3_errmsg(db) << endl;
        sqlite3_free(sql);
        return;
    }

    sqlite3_free(sql);

    // Execute the delete statement
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        cout << "Error in deleting record:" << sqlite3_errmsg(db)<<endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr); //to undo all executed steps
        return;
    }
    sqlite3_finalize(stmt);

    // Commit the transaction
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);

    //In SQLite, a transaction is a sequence of database operations that are treated as a single logical unit of work.
    //Transactions provide a way to ensure that a group of database operations are executed atomically, i.e., either all operations succeed or none of them do.
    //Transactions are important for ensuring the consistency and integrity of the database.



}


void Database::update_record_by_input_text(sqlite3* db, int index, char* update_record_text[4]){


    char* sql = sqlite3_mprintf("UPDATE Job_List SET Company='%q', Position='%q', Status='%q', Weblink='%q' WHERE ROWID=%d;",
                                update_record_text[0],update_record_text[1],update_record_text[2],update_record_text[3], index);

    int rc = sqlite3_exec(db, sql, NULL, NULL, 0);

    if (rc != SQLITE_OK){

        cout<< "Error in updating record: "<< sqlite3_errmsg(db) << endl;
    }

    else{

        cout<< "Record updated successfully!" << endl;
    }

    sqlite3_free((void*)sql);

}



void Database::update_record_by_sql_cmd (sqlite3* db, char* sql){

    int rc = sqlite3_exec(db, sql, NULL, NULL, 0);

    if (rc != SQLITE_OK){

        cout<< "Error in updating record: "<< sqlite3_errmsg(db) << endl;
    }

    else{

        cout<< "Record updated successfully!" << endl;
    }

    sqlite3_free((void*)sql);
}


void Database::count_applied_job (sqlite3* db){

    //to cound how many jobs have been applied

}




