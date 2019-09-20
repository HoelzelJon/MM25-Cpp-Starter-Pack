# MechMania25 C++ Starter Pack

Here's all the code you need to get started with making a bot for MechMania in C++. Just do these steps:

* Pre-Setup -- install Node and the `mm` command line tools
* Setup -- Clone this repository and start running your bot!

# Pre-Setup

1. Install or update clang++ to compile your C++ code!
        
2. Install Node. To do this, go [here](https://nodejs.org/en/download/) and download the appropriate installer for your operating system.
    * Run the installer with all the defaults.

3. Run `npm install -g mechmania`.  This gets the `mm` command line tools, which are used to run the game, test and submit bots for the tournament.

4. Run `mm download` to download required files.

# Setup

1. Clone this repo (or fork it or download it somewhere as a ZIP)
2. Modify the script at `strategy.cpp`.
    * Write your code in the `Strategy::doTurn` method.
    * You may also add other files or dependencies. If you have any questions about this, we're here to help!
3. Run `make server`. This step should be completed whenever you want to run any `mm` commands with a new strategy.
    * If you encounter any linker errors that seem to do nothing with your files, this may be a result of some dependency errors.
         * [Crow](https://github.com/ipkn/crow) is the library used for the webserver, and has caused issues in the past. In particular, the [boost](https://www.boost.org/users/download/) C++ library. 
4. Run `mm play .`
    * This will build the bot in the given directory (`.`) and then starts a game in which your bot fights against itself.
    * On Windows, if an `mm play` command fails, make sure to close any Java SE Runtime Binary processes with Task Manager.
5. To run two different bots against each other, run `mm play bot1_directory bot2_directory`.
6. To submit your bot, run `mm push .`

Use `mm help` for more information!

# Game API Information
The header file (`strategy.h`) has a list of functions that you should implement, as well as other functions that you may use from the API.  
   
*Note: you should not need to edit the files inside of `Resources`.*  
  
Good luck, and happy coding!
