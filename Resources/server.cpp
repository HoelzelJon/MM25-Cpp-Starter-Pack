#include "api.h"
#include "../strategy.h"
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

int main() {
  map<string, Strategy> games;

  crow::SimpleApp app;

  CROW_ROUTE(app, "/game_init")
      .methods("POST"_method)([&games](const crow::request &req) {
        string body = req.body;
        json parsedJson = json::parse(body);
        // std::cout << parsedJson << std::endl;
        int playerNum = parsedJson["playerNum"].get<int>();
        Strategy myGame(body, playerNum);
        // std::cout << "playerNum: " << playerNum << std::endl;
        string gameId = parsedJson["gameId"].get<string>();
        // std::cout << "gameId: " << gameId << std::endl;
        games[gameId] = myGame;

        vector<UnitSetup> setup;
        try {
          setup = myGame.getSetup();
        } catch (const std::exception &e) {
          std::cout << "Exception caught: " << e.what() << std::endl;
        }

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
        Strategy &myGame = games[gameId];
        myGame.updateGame(body);

        vector<UnitDecision> decision;
        try {
          decision = myGame.doTurn();
        } catch (const std::exception &e) {
          std::cout << "Exception caught: " << e.what() << std::endl;
        }

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
        string result = parsedJson["result"].get<string>();
        string gameId = parsedJson["gameId"].get<string>();
        Strategy &myGame = games[gameId];
        myGame.onGameOver(result);
        games.erase(gameId);

        json gameOverJson;
        gameOverJson["message"] = "Game successfully exited";
        stringstream ss;
        ss << gameOverJson;
        return ss.str();
      });

  CROW_ROUTE(app, "/health")([] { return 200; });

  long port = strtol(std::getenv("PORT"), nullptr, 0);
  app.port(port).multithreaded().run();
}
