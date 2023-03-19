#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "sqlite3.h"
#include "crow.h"
#include "db.hpp"

using namespace std;



static int callback(void* data, int argc, char** argv, char** azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
  
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
  
    printf("\n");
    return 0;
}



int main(int argc, char *argv[]){
    crow::SimpleApp app;

    
    if (argc > 1){ 
        cout << argv[1] << "\n";   
        if (strcmp( argv[1], "createtables") == 0){
            Database *data = new Database();
            data->createTableCategory();
            data->createTableTask();
            data->closeDB();
            delete data;
        
            cout << "Tables Created" << "\n";
        }else if(strcmp( argv[1], "sqlmanipulation") == 0){
            // MANIPULATION AND TESTING


            cout << "to exit type /q" << "\n";
            sqlite3 *m_db;
            char* messageError;
            string query = "";
            bool looping = true;
            int exit = sqlite3_open("sqldata.db", &m_db);
    
            // update
            
            
            while(looping == true){
                getline(cin, query);

                if (query == "/q"){
                    looping = false;
                }else{


                    exit = sqlite3_exec(m_db, query.c_str(), callback, 0, &messageError);
                    if (exit != SQLITE_OK) {
                        cerr << messageError << "\n";
                        sqlite3_free(messageError);    
                        
                    }


                }

            
            }
        
            // ia me esquecendo de fechar
            sqlite3_close(m_db);
        }
    }

    CROW_ROUTE(app, "/")([](){
        std::string paths = "paths = "
                            "[\n\n  "
                            "task/\n  task/<int>\n  "
                            "category/\n  category/<int>"
                            "\n\n]";
        return paths;
    });



    // TASK ROUTES 
    CROW_ROUTE(app, "/task").methods("GET"_method, "POST"_method)([](const crow::request& req){
        if (req.method == "POST"_method){
            auto x = crow::json::load(req.body);
            if (!x){
                return crow::response(400);
            }

            Database *data = new Database();
            bool posted = data->insertTableTask(x["id"].i(), x["name"].s(), x["text"].s(), x["created"].s(), x["finished"].b(), x["id_category"].i());
            data->closeDB();
            delete data;

            if (posted){
                return crow::response(200, "ok");
            }else{
                return crow::response(400, "UNIQUE constraint failed: CATEGORY.ID");   
            }
            
        
        }else if(req.method == "GET"_method){
            Database data = Database();
            crow::json::wvalue x;
            int contador = 0;
            
            for (auto cat : data.selectTableTask()){
                cout << cat.created << "\n";

                x[contador]["id"] = cat.id;
                x[contador]["name"] = cat.name;
                x[contador]["text"] = cat.text;
                x[contador]["created"] = cat.created;
                x[contador]["finished"] = cat.finished;
                x[contador]["id_category"] = cat.id_category;

                ++contador;
            }

            data.closeDB();

            return crow::response(x);   
        
        }

        return crow::response(405, "method not accept");

    });


    CROW_ROUTE(app, "/task/<int>").methods("DELETE"_method, "PUT"_method, "GET"_method)([](const crow::request& req, int id){
        if (req.method == "DELETE"_method){
            auto x = crow::json::load(req.body);
            if (!x){
                return crow::response(400);
            }

            Database *data = new Database();
            bool deleted = data->removeTableTask(id);
            data->closeDB();
            delete data;

            if (deleted){
                return crow::response(200, "deleted with success");
            }else{
                return crow::response(404, "not found task");   
            }
            
        
        }else if (req.method == "PUT"_method){
            auto x = crow::json::load(req.body);
            if (!x){
                return crow::response(400);
            }

            Database *data = new Database();
            bool posted = data->updateTableTask(id, x["name"].s(), x["text"].s(), x["created"].s(), x["finished"].b(), x["id_category"].i());
            data->closeDB();
            delete data;

            if (posted){
                return crow::response(200, "ok");
            }else{
                return crow::response(400, "UNIQUE constraint failed: TASK.ID");   
            }

        }else if(req.method == "GET"_method){
            // retrieve
            Database data = Database();
            crow::json::wvalue x;
            int contador = 0;
            
            // Database::task
            for (auto task : data.selectTableTask(id)){

                x[contador]["id"] = task.id;
                x[contador]["name"] = task.name;
                x[contador]["text"] = task.text;
                x[contador]["created"] = task.created;
                x[contador]["finished"] = task.finished;
                x[contador]["id_category"] = task.id_category;

                ++contador;
            }

            data.closeDB();

            return crow::response(x);   
        
        }

        return crow::response(405, "method not accept");

    });

    
    // TASK ROUTES END




    // CATEGORIES ROUTES

    CROW_ROUTE(app, "/category").methods("GET"_method, "POST"_method)([](const crow::request& req){
        
        if (req.method == "GET"_method){
            Database data = Database();
            crow::json::wvalue x;
            int contador = 0;
            
            for (auto cat : data.selectTableCategory()){
            
                x[contador]["id"] = cat.id;
                x[contador]["name"] = cat.name;
                ++contador;

            }
            // fechando o db
            data.closeDB();
            
            return crow::response(x);

        }else if(req.method == "POST"_method){
            auto x = crow::json::load(req.body);
            if (!x){
                return crow::response(400);
            }

            Database *data = new Database();
            bool posted = data->insertTableCategory(x["name"].s());
            data->closeDB();
            delete data;

            if(posted){
                return crow::response(200, "ok");    
            }else{
                return crow::response(400, "UNIQUE constraint failed: CATEGORY.ID");
            }

        }
        // code not accept method
        return crow::response(405);        
    
    });

    CROW_ROUTE(app, "/category/<int>").methods("PUT"_method, "DELETE"_method, "GET"_method)([](const crow::request& req, int id){
        if (req.method == "DELETE"_method){
            auto x = crow::json::load(req.body);
            if (!x){
                return crow::response(400);
            }

            Database *data = new Database();
            bool deleted = data->removeTableCategory(id);
            data->closeDB();
            delete data;

            if (deleted){
                return crow::response(200, "deleted with success");
            }else{
                return crow::response(404, "not found category");
            }


        }else if (req.method == "PUT"_method){
            auto x = crow::json::load(req.body);
            if (!x){
                return crow::response(400);
            }

            Database *data = new Database();
            bool posted = data->updateTableCategory(id, x["name"].s());
            data->closeDB();
            delete data;

            if (posted){
                return crow::response(200, "ok");
            }else{
                return crow::response(400, "UNIQUE constraint failed: CATEGORY.ID");   
            }

        }else if(req.method == "GET"_method){
            // retrieve
            Database data = Database();
            crow::json::wvalue x;
            int contador = 0;
            
            for (auto cat : data.selectTableCategory(id)){

                x[contador]["id"] = cat.id;
                x[contador]["name"] = cat.name;

                ++contador;
            }

            data.closeDB();

            return crow::response(x);   
        
        }


        return crow::response(405, "method not accept");

    });

    // CATEGORIES ROUTES END



    app.port(18080).run();


} 
