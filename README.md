# Running MechMania locally

## Requirements
docker

## Instructions for running bot server
```
docker build -t mm25 .
docker run -t -i -p <port>:3000 mm25
```
- remember to replace <port> with your desired port number (if running two instances of game, then run on two different ports)
- Ctrl-C to exit

## Instructions for running Game Engine
```
java -jar <location of GameEngine.jar> <location of Maps/maps1.csv> <name of playerOne> <name of playerTwo> <address of playerOne> <address of playerTwo> STDOUT
```
Explanation:
- `maps1.csv` is the map of the game
- `name of player ___` is a display name for each player
- each address is the locally hosted address of the bots that were ran in the section above
  - when using this starter pack, the address is `https://0.0.0.0:PORTNUMBER/`
  - remember to change out `PORTNUMBER` to the ports that were set above
  - this can be exchanged for `HUMAN` to bring up a visual version
- `STDOUT` means to print the output of the game engine to standard out
  - this can be changed to a filename to output to a file
