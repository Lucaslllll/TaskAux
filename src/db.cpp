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


// ROUTES CATEGORY

bool Database::createTableCategory(){
	string sql = "CREATE TABLE CATEGORY("
				 "ID INT PRIMARY KEY 	NOT NULL, "
				 "NAME 			TEXT 	NOT NULL); ";

	char* messageError;
	int exit = sqlite3_exec(m_db, sql.c_str(), NULL, 0, &messageError);

    
	if(exit != SQLITE_OK){
		cout << messageError << "\n";
		sqlite3_free(messageError);
		return false;
	}
	else{
		return true;
	}

}

bool Database::insertTableCategory(int id, string name){
	char* messageError;
    int exit = sqlite3_open("sqldata.db", &m_db);
    
  	// insert
    string sql = "INSERT INTO CATEGORY VALUES("+to_string(id)+", '"+name+"');";
  
    exit = sqlite3_exec(m_db, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << messageError << "\n";
    	sqlite3_free(messageError);    
    	
    	return false;
    }
    else{
        cout << "Records created Successfully!" << '\n';
    	return true;
    }


}

bool Database::removeTableCategory(int id){
	char* messageError;
    int exit = sqlite3_open("sqldata.db", &m_db);
    
    // remove
    string query = "DELETE FROM CATEGORY WHERE ID = "+to_string(id)+";";
  
    exit = sqlite3_exec(m_db, query.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << messageError << "\n";
    	sqlite3_free(messageError);    
    	
    	return false;
    }
    else{
        cout << "Records removed Successfully!" << '\n';
    	return true;
    }

}


bool Database::updateTableCategory(int id, string name){
	char* messageError;
    int exit = sqlite3_open("sqldata.db", &m_db);
    
    // update
    string query = "UPDATE CATEGORY SET NAME = '"+name+"' WHERE ID = "+to_string(id)+";";
  
    exit = sqlite3_exec(m_db, query.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << messageError << "\n";
    	sqlite3_free(messageError);    
    	
    	return false;
    }
    else{
        cout << "Records updated Successfully!" << '\n';
    	return true;
    }
}


vector <Database::category> Database::selectTableCategory(){
	sqlite3_stmt *stmt;
    int rc = sqlite3_open("sqldata.db", &m_db);
    string query = "SELECT * FROM CATEGORY;";
  	vector<category> vector_c;

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

vector <Database::category> Database::selectTableCategory(int id){
	sqlite3_stmt *stmt;
    int rc = sqlite3_open("sqldata.db", &m_db);
    string query = "SELECT * FROM CATEGORY WHERE ID='"+to_string(id)+"';";
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



// ROUTES TASK

bool Database::createTableTask(){

	
	string sql = "CREATE TABLE TASK("
				 "ID INT PRIMARY KEY  NOT NULL, "
				 "NAME 			TEXT  NOT NULL, "
				 "TEXT 			TEXT, "
				 "CREATED       DATE  NOT NULL, "
				 "FINISHED      BIT, "
				 "ID_CATEGORY   INT, "
				 "FOREIGN KEY(ID_CATEGORY) REFERENCES CATEGORY(ID_CATEGORY) );";
	char* messageError;
	int exit = sqlite3_exec(m_db, sql.c_str(), NULL, 0, &messageError);
	
	
	if(exit != SQLITE_OK){
		cout << messageError << "\n";
		sqlite3_free(messageError);
		return false;

	}
	else{
		return true;
	}



}



bool Database::insertTableTask(int id, string name, string text, string created, bool finished, int id_category){
	char* messageError;
    int exit = sqlite3_open("sqldata.db", &m_db);
    

    // insert
    string sql = "INSERT INTO TASK VALUES("+to_string(id)+", '"+name+"', '"+text+"', "+created+", "+to_string(finished)+", "+to_string(id_category)+");";
  
    exit = sqlite3_exec(m_db, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << messageError << "\n";
        sqlite3_free(messageError);
    	
    	return false;
    }
    else{
        cout << "Records created Successfully!" << '\n';
    	return true;
    }


}


bool Database::removeTableTask(int id){
	char* messageError;
    int exit = sqlite3_open("sqldata.db", &m_db);
    
    // remove
    string query = "DELETE FROM TASK WHERE ID = "+to_string(id)+";";
  
    exit = sqlite3_exec(m_db, query.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << messageError << "\n";
    	sqlite3_free(messageError);    
    	
    	return false;
    }else{
        cout << "Records removed Successfully!" << '\n';
    	return true;
    }
}


bool Database::updateTableTask(int id, string name, string text, string created, bool finished, int id_category){
	char* messageError;
    int exit = sqlite3_open("sqldata.db", &m_db);
    
    // update
    string query = "UPDATE TASK " 
    			   "SET NAME = '"+name+"', TEXT = '"+text+"', CREATED = '"+created+"', "
    			   "	FINISHED = "+to_string(finished)+", ID_CATEGORY = "+to_string(id_category)+" "  
    			   "WHERE ID = "+to_string(id)+";";
  
    exit = sqlite3_exec(m_db, query.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << messageError << "\n";
    	sqlite3_free(messageError);    
    	
    	return false;
    }
    else{
        cout << "Records updated Successfully!" << '\n';
    	return true;
    }
}



vector <Database::task> Database::selectTableTask(){
	sqlite3_stmt *stmt;
	const char* messageError;
    int rc = sqlite3_open("sqldata.db", &m_db);
    string query = "SELECT * FROM TASK;";
  	vector<task> vector_t;

	rc = sqlite3_prepare_v2(m_db, query.c_str(), query.length(), &stmt, &messageError);
	if (rc != SQLITE_OK) {
		cerr << messageError << "\n";
		return vector_t;
	}

	// Loop through the results, a row at a time.
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		task t;
		
		t.id = sqlite3_column_int(stmt, 0);
		t.name = (const char*)sqlite3_column_text(stmt, 1);
		t.text = (const char*)sqlite3_column_text(stmt, 2);
		t.created = (const char*)sqlite3_column_text(stmt, 3);
		t.finished = (const char*)sqlite3_column_type(stmt, 4);
		t.id_category = sqlite3_column_int(stmt, 5);

		vector_t.push_back(t);
		
		// delete c;
	}
	
	// Free the statement when done.
	sqlite3_finalize(stmt);

	return vector_t;

}

vector <Database::task> Database::selectTableTask(int id){
	sqlite3_stmt *stmt;
	const char* messageError;
    int rc = sqlite3_open("sqldata.db", &m_db);
    string query = "SELECT * FROM TASK WHERE ID='"+to_string(id)+"';";
  	vector<task> vector_t;

	rc = sqlite3_prepare_v2(m_db, query.c_str(), query.length(), &stmt, &messageError);
	if (rc != SQLITE_OK) {
		cerr << messageError << "\n";
		return vector_t;
	}

	// Loop through the results, a row at a time.
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		task t;
		
		t.id = sqlite3_column_int(stmt, 0);
		t.name = (const char*)sqlite3_column_text(stmt, 1);
		t.text = (const char*)sqlite3_column_text(stmt, 2);
		t.created = (const char*)sqlite3_column_text(stmt, 3);
		t.finished = (const char*)sqlite3_column_type(stmt, 4);
		t.id_category = sqlite3_column_int(stmt, 5);

		vector_t.push_back(t);
		
		// delete c;
	}
	
	// Free the statement when done.
	sqlite3_finalize(stmt);

	return vector_t;

}




void Database::closeDB(){
	sqlite3_close(m_db);
}
