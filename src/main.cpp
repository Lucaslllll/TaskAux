#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

#include "crow.h"
#include "db.hpp"

using namespace std;





int main(int argc, char *argv[]){
    crow::SimpleApp app;

    
    if (argc > 1){ 
        cout << argv[1] << "\n";   
        if (strcmp( argv[1], "createtables") == 0){
            Database *data = new Database();
            data->createTableCategory();
            data->createTableTask();
            cout << "Tables Created" << "\n";
        }
    }

    CROW_ROUTE(app, "/")([](){
        std::string paths = "paths = [\n task/\n category/\n ]";
        return paths;
    });


    CROW_ROUTE(app, "/task").methods("GET"_method, "POST"_method)([](const crow::request& req){
        if (req.method == "POST"_method){
            auto x = crow::json::load(req.body);
            if (!x){
                return crow::response(400);
            }

            Database *data = new Database();
            data->insertTableTask(x["id"].i(), x["name"].s(), x["text"].s(), x["created"].s(), x["finished"].b(), x["id_category"].i());


            return crow::response("ok");
        
        }else if(req.method == "GET"_method){
            Database data = Database();
            crow::json::wvalue x;
            int contador = 0;
            
            for (auto cat : data.selectTableTask()){
                x[contador]["id"] = cat.id;
                x[contador]["name"] = cat.name;
                x[contador]["text"] = cat.text;
                x[contador]["created"] = cat.created;
                x[contador]["finished"] = cat.finished;
                x[contador]["id_category"] = cat.id_category;

                contador++;
            }

            
            return crow::response(x);   
        }

        return crow::response(405);

    });


    CROW_ROUTE(app, "/category").methods("GET"_method, "POST"_method)([](const crow::request& req){
        
        if (req.method == "GET"_method){
            Database data = Database();
            crow::json::wvalue x;
            int contador = 0;
            
            for (auto cat : data.selectTableCategory()){
            
                x[contador]["id"] = cat.id;
                x[contador]["name"] = cat.name;
                contador++;

            }

            
            return crow::response(x);

        }else if(req.method == "POST"_method){
            auto x = crow::json::load(req.body);
            if (!x){
                return crow::response(400);
            }

            Database *data = new Database();
            data->insertTableCategory(x["id"].i(), x["name"].s());
            
            delete data;
            return crow::response("ok");    
        }

        // code not accept method
        return crow::response(405);
        
    
    });

    app.port(18080).run();
} 
