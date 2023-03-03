#include "sqlite3.h"
#include <iostream>
#include <vector>



class Database{

private:
	sqlite3 *m_db;
	bool m_open;


public:

	Database();
	bool createTableTask();
	bool createTableCategory();
	void closeDB();

};