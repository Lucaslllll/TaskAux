#include "sqlite3.h"
#include <iostream>
#include <vector>




class Database{

private:
	sqlite3 *m_db;
	bool m_open;


public:
	struct category{
		int id;
		std::string name;
		
	};

	struct task{
		int id;
		std::string name;
		std::string text;
		std::string created;
		bool finished;
		int id_category;
	};


	Database();
	bool createTableTask();
	bool insertTableTask(int id, std::string name, std::string text, std::string created, bool finished, int id_category);
	bool removeTableTask(int id);
	bool updateTableTask(int id, std::string name, std::string text, std::string created, bool finished, int id_category);

	bool createTableCategory();
	bool insertTableCategory(int id, std::string name);
	bool removeTableCategory(int id);
	bool updateTableCategory(int id, std::string name);


	std::vector <category> selectTableCategory();
	std::vector <category> selectTableCategory(int id);
	std::vector <task> selectTableTask();
	std::vector <task> selectTableTask(int id);


	void closeDB();

};