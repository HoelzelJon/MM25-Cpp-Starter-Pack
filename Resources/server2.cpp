#include "../strategy2.h"
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
  map<string, Strategy2> games;

  crow::SimpleApp app;

  CROW_ROUTE(app, "/game_init")
      .methods("POST"_method)([&games](const crow::request &req) {
        string body = req.body;
        json parsedJson = json::parse(body);
        std::cout << parsedJson << std::endl;
        int playerNum = parsedJson["playerNum"].get<int>();
        Strategy2 myGame(body, playerNum);
        // std::cout << "playerNum: " << playerNum << std::endl;
        string gameId = parsedJson["gameId"].get<string>();
        // std::cout << "gameId: " << gameId << std::endl;
        games[gameId] = myGame;
        vector<UnitSetup> setup = myGame.getSetup();

        json setupJson = setup;
        stringstream ss;
        ss << setupJson;
        // std::cout << "setupJson: " << setupJson << std::endl;
        return ss.str();
      });

  CROW_ROUTE(app, "/turn")
      .methods("POST"_method)([&games](const crow::request &req) {
        string body = req.body;
        json parsedJson = json::parse(body);
        // std::cout << parsedJson << std::endl;
        string gameId = parsedJson["gameId"].get<string>();
        // std::cout << "gameId: " << gameId << std::endl;
        Strategy2 myGame = games[gameId];
        myGame.updateGame(body);
        // std::cout << "updated game with new body" << std::endl;
        Decision decision = myGame.doTurn();

        json decisionJson = decision;
        stringstream ss;
        ss << decisionJson;
        // std::cout << "decisionJson: " << decisionJson << std::endl;
        return ss.str();
      });

  CROW_ROUTE(app, "/game_over")
      .methods("POST"_method)([&games](const crow::request &req) {
        string body = req.body;
        // std::cout << body << std::endl;
        json parsedJson = json::parse(body);
        games.erase(parsedJson["gameId"].get<string>());
        // std::cout << "game exited, length of games: " << games.size()
        // << std::endl;
        return "Game successfully exited.";
      });

  if (argc != 2) {
    std::cout << "please run ./server with 1 argument: the port number to "
                 "host on"
              << std::endl;
    return 1;
  }
  long port = strtol(argv[1], nullptr, 0);
  app.port(port).multithreaded().run();
}
