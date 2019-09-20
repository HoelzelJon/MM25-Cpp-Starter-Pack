CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lm -lc++ -pthread -lboost_system

server : Resources/api.o strategy.o Resources/json_conversions.o Resources/server.o
	$(LD) Resources/api.o strategy.o Resources/server.o Resources/json_conversions.o $(LDFLAGS) -o server

server.o : Resources/server.cpp api.h Resources/crow_all.h Resources/json.hpp
	$(CXX) $(CXXFLAGS) Resources/server.cpp

strategy.o : strategy.cpp api.h
	$(CXX) $(CXXFLAGS) strategy.cpp

api.o : Resources/api.cpp api.h Resources/json.hpp
	$(CXX) $(CXXFLAGS) Resources/api.cpp

json_conversions.o : Resources/json_conversions.cpp api.h
	$(CXX) $(CXXFLAGS) Resources/json_conversions.cpp

clean :
	rm -f *.o server Resources/*.o
