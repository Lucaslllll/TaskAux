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

        // save in db
        Database *data = new Database();
        bool resp = data->createTableTask();

//         delete data; 
        // save in db end

        
        return crow::response("FEITO");
    });

    app.port(18080).run();
} 
