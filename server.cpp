#include "api.h"
#include "crow_all.h"
#include "json.hpp"

#include <iostream>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

using json = nlohmann::json;
using std::map;
using std::string;
using std::stringstream;
using std::vector;
using namespace MechMania;

int main(int argc, char **argv) {
  map<string, Game *> games;

  crow::SimpleApp app;

  CROW_ROUTE(app, "/game_init")
      .methods("POST"_method)([&games](const crow::request &req) {
        string body = req.body;
        std::cout << body << std::endl;
        json parsedJson = json::parse(body);
        Game *myGame =
            new Game(body, std::atoi(parsedJson["playerNum"].get<string>()));
        games[parsedJson["gameId"].get<string>()] = myGame;
        vector<UnitSetup> setup = myGame->getSetup();

        json setupJson = setup;
        stringstream ss;
        ss << setupJson;
        return ss.str();
      });

  CROW_ROUTE(app, "/turn")
      .methods("POST"_method)([&games](const crow::request &req) {
        string body = req.body;
        std::cout << body << std::endl;
        json parsedJson = json::parse(body);
        Game *myGame = games[parsedJson["gameId"].get<string>()];
        myGame->updateGame(body);
        Decision decision = myGame->doTurn();

        json decisionJson = decision;
        stringstream ss;
        ss << decisionJson;
        return ss.str();
      });

  CROW_ROUTE(app, "/game_over")
      .methods("POST"_method)([&games](const crow::request &req) {
        string body = req.body;
        std::cout << body << std::endl;
        json parsedJson = json::parse(body);
        games.erase(parsedJson["gameId"].get<string>());
        return "Game successfully exited.";
      });

  long port = strtol(argv[1], nullptr, 0);
  app.port(port).multithreaded().run();
}
