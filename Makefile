CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lm

server : Resources/api.o strategyOne.o Resources/json_conversions.o Resources/server.o
	$(LD) Resources/api.o strategyOne.o Resources/server.o Resources/json_conversions.o $(LDFLAGS) -o server

server.o : Resources/server.cpp Resources/api.h Resources/crow_all.h Resources/json.hpp
	$(CXX) $(CXXFLAGS) Resources/server.cpp

strategyOne.o : strategyOne.cpp Resources/api.h
	$(CXX) $(CXXFLAGS) strategyOne.cpp

api.o : Resources/api.cpp api.h Resources/json.hpp
	$(CXX) $(CXXFLAGS) Resources/api.cpp

json_conversions.o : Resources/json_conversions.cpp Resources/api.h
	$(CXX) $(CXXFLAGS) Resources/json_conversions.cpp

clean :
	rm *.o server
