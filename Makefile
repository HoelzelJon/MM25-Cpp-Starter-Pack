CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lm

server : Resources/api.o strategy.o Resources/json_conversions.o Resources/server.o
	$(LD) Resources/api.o strategy.o Resources/server.o Resources/json_conversions.o $(LDFLAGS) -o server

both-strategies : server server2

server2 : Resources/api.o strategy2.o Resources/json_conversions.o Resources/server2.o
	$(LD) Resources/api.o strategy2.o Resources/server2.o Resources/json_conversions.o $(LDFLAGS) -o server2

server.o : Resources/server.cpp api.h Resources/crow_all.h Resources/json.hpp
	$(CXX) $(CXXFLAGS) Resources/server.cpp

server2.o : Resources/server2.cpp api.h Resources/crow_all.h Resources/json.hpp
	$(CXX) $(CXXFLAGS) Resources/server2.cpp

strategy.o : strategy.cpp api.h
	$(CXX) $(CXXFLAGS) strategy.cpp

strategy2.o : strategy2.cpp api.h
	$(CXX) $(CXXFLAGS) strategy2.cpp

api.o : Resources/api.cpp api.h Resources/json.hpp
	$(CXX) $(CXXFLAGS) Resources/api.cpp

json_conversions.o : Resources/json_conversions.cpp Resources/api.h
	$(CXX) $(CXXFLAGS) Resources/json_conversions.cpp

clean :
	rm *.o server* Resources/*.o
