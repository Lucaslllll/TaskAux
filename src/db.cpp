#include "db.hpp"

using namespace std;



Database::Database(){
	int rc = sqlite3_open("sqldata.db", &m_db);
	if(rc != SQLITE_OK){
		m_open = false;
	}else{
		m_open = true;
	}

}

bool Database::createTableCategory(){
	string sql = "CREATE TABLE CATEGORY("
				 "ID INT PRIMARY KEY 	NOT NULL, "
				 "NAME 			TEXT 	NOT NULL); ";

	char* messageError;
	int exit = sqlite3_exec(m_db, sql.c_str(), NULL, 0, &messageError);

    cout << messageError << "\n";
	if(exit != SQLITE_OK){
		return false;
	}
	else{
		return true;
	}

}

bool Database::insertTableCategory(int id, string name){
	char* messageError;
    int exit = sqlite3_open("sqldata.db", &m_db);
    string query = "SELECT * FROM CATEGORY;";
  
    cout << "STATE OF TABLE BEFORE INSERT" << "\n";
  
    sqlite3_exec(m_db, query.c_str(), NULL, 0, &messageError);
  


  	// insert
    string sql = "INSERT INTO CATEGORY VALUES("+to_string(id)+", '"+name+"');";
  
    exit = sqlite3_exec(m_db, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << messageError << "\n";
        sqlite3_free(messageError);
    	
    	return false;
    }
    else{
        cout << "Records created Successfully!" << "\n";
    	return true;
    }


}


bool Database::createTableTask(){

	if (createTableCategory()){
		string sql = "CREATE TABLE TASK("
					 "ID INT PRIMARY KEY  NOT NULL, "
					 "NAME 			TEXT  NOT NULL, "
					 "TEXT 			TEXT, "
					 "CREATED       DATE  NOT NULL, "
					 "FINISHED      BIT, "
					 "ID_CATEGORY   INT FOREIGN REFERENCES CATEGORY(ID_CATEGORY) );";
		char* messageError;
		int exit = sqlite3_exec(m_db, sql.c_str(), NULL, 0, &messageError);
		
		cout << messageError << "\n";
		if(exit != SQLITE_OK){
			return false;
		}
		else{
			return true;
		}

	}else{
		return false;
	}


}



bool Database::insertTableTask(int id, string name, string text, string created, bool finished, int id_category){
	char* messageError;
    int exit = sqlite3_open("sqldata.db", &m_db);
    string query = "SELECT * FROM TASK;";
  
    cout << "STATE OF TABLE BEFORE INSERT" << "\n";
  
    sqlite3_exec(m_db, query.c_str(), NULL, 0, &messageError);
  


  	// insert
    string sql = "INSERT INTO TASK VALUES("+to_string(id)+", '"+name+"', '"+text+"', "+created+", "+to_string(finished)+", "+to_string(id_category)+");";
  
    exit = sqlite3_exec(m_db, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << messageError << "\n";
        sqlite3_free(messageError);
    	
    	return false;
    }
    else{
        cout << "Records created Successfully!" << "\n";
    	return true;
    }


}


static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
  
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
  
    printf("\n");
    return 0;
}


vector <Database::category> Database::selectTableCategory(){
	sqlite3_stmt *stmt;
    int rc = sqlite3_open("sqldata.db", &m_db);
    string query = "SELECT * FROM CATEGORY;";
  	std::vector<category> vector_c;

	rc = sqlite3_prepare_v2(m_db, query.c_str(), query.length(), &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cerr << "Erro" << "\n";
		return vector_c;
	}

	// Loop through the results, a row at a time.
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		category c;
		
		c.id = sqlite3_column_int(stmt, 0);
		c.name = (const char*)sqlite3_column_text(stmt, 1);
		vector_c.push_back(c);
		
		// delete c;
	}
	
	// Free the statement when done.
	sqlite3_finalize(stmt);

	return vector_c;

}


void Database::closeDB(){
	sqlite3_close(m_db);
}
