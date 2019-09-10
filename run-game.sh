# compile c++ files for strategy 1
make
# run server for strategy 1
./server 3000 &
# run server for strategy 2
./server 3001 &
# run game
java -jar mm25_game_engine.jar GameID Maps playerOne playerTwo http://0.0.0.0:3000/ http://0.0.0.0:3001/
