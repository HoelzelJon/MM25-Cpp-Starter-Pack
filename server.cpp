#include "crow_all.h"

int main() {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/game_init")
      .methods("POST"_method)(
          [](const crow::request &req) { return "Hello world"; });

  CROW_ROUTE(app, "/turn").methods("POST"_method)([](const crow::request &req) {
    return "Hello world";
  });

  CROW_ROUTE(app, "/game_over")
      .methods("POST"_method)(
          [](const crow::request &req) { return "Hello world"; });

  app.port(18080).multithreaded().run();
}
