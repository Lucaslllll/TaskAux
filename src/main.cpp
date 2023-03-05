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


    CROW_ROUTE(app, "/category").methods("POST"_method)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x){
            return crow::response(400);
        }

        Database *data = new Database();
        // data->createTableCategory();
        // data->insertTableCategory(x["id"].i(), x["name"].s());
        
        Database::category cat = data->selectTableCategory();
        
        cout << cat.id << "\n";
        cout << cat.name << "\n";

        delete data;

        return crow::response("ok");
    
    });

    app.port(18080).run();
} 
