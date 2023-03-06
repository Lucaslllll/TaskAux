#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "crow.h"
#include "db.hpp"

using namespace std;





int main(){
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });


    CROW_ROUTE(app, "/task").methods("POST"_method)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x){
            return crow::response(400);
        }

        // cout <<  << "\n";
        // save in db
        Database *data = new Database();
        // data->createTableTask();
        data->insertTableTask(x["id"].i(), x["name"].s(), x["text"].s(), x["created"].s(), x["finished"].i(), x["id_category"].i());


        delete data; 
        // save in db end

        return crow::response("ok");
    
    });


    CROW_ROUTE(app, "/category").methods("GET"_method, "POST"_method)([](const crow::request& req){
        
        if (req.method == "GET"_method){
            Database *data = new Database();
            

            vector <crow::json::wvalue> lista_resposta;
            for (Database::category* cat : data->selectTableCategory()){
                crow::json::wvalue resposta;
                resposta["id"] = cat->id;
                resposta["name"] = cat->name.c_str();

                lista_resposta.push_back(resposta);
            }

            
        
            delete data;
            return crow::response(lista_resposta);

        }else if(req.method == "POST"_method){
            auto x = crow::json::load(req.body);
            if (!x){
                return crow::response(400);
            }

            Database *data = new Database();
            // data->createTableCategory();
            data->insertTableCategory(x["id"].i(), x["name"].s());
            
            delete data;
            return crow::response("ok");    
        }

        // code not accept method
        return crow::response(405);
        
    
    });

    app.port(18080).run();
} 
