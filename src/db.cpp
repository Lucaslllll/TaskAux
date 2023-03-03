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

	if(exit != SQLITE_OK){
		return false;
	}
	else{
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


void Database::closeDB(){
	sqlite3_close(m_db);
}