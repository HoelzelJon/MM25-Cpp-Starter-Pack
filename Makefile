CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lm

server : api.o strategy.o json_conversions.o server.o
	$(LD) api.o strategy.o server.o json_conversions.o $(LDFLAGS) -o server

server.o : server.cpp api.h crow_all.h json.hpp
	$(CXX) $(CXXFLAGS) server.cpp

strategy.o : strategy.cpp api.h
	$(CXX) $(CXXFLAGS) strategy.cpp

api.o : api.cpp api.h json.hpp
	$(CXX) $(CXXFLAGS) api.cpp

json_conversions.o : json_conversions.cpp api.h
	$(CXX) $(CXXFLAGS) json_conversions.cpp

clean :
	rm *.o server
