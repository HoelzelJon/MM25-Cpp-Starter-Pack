# MechMania25 C++ Starter Pack

Here's all the code you need to get started with making a bot for MechMania in C++. Just do these steps:

* Pre-Setup -- install Java, Node, and the `mm` command line tools
* Setup -- Clone this repository and start running your bot!

# Pre-Setup
1. Follow the Pre-setup instructions on the wiki [here](https://github.com/HoelzelJon/MechMania-25-Wiki/wiki#pre-setup)

2. Install or update clang++ to compile your C++ code! You can check out [this guide on getting started with clang](https://clang.llvm.org/get_started.html)

# Setup

1. Clone this repo (or fork it or download it somewhere as a ZIP)
2. Modify the script at `strategy.cpp`.
    * Write your code in the `Strategy::doTurn` method.
    * You may also add other files or dependencies. If you have any questions about this, we're here to help!
3. Run `make server`. This step should be completed whenever you want to run any `mm` commands with a new strategy.
    * If you encounter any linker errors that seem to do nothing with your files, this may be a result of some dependency errors.
         * [Crow](https://github.com/ipkn/crow) is the library used for the webserver, and has caused issues in the past. In particular, the [boost](https://www.boost.org/users/download/) C++ library.
         * If you are still having issues, consult [Boost Installation](#boost-installation-faq)
4. Run `mm play .`
    * This will build the bot in the given directory (`.`) and then starts a game in which your bot fights against itself.
    * On Windows, if an `mm play` command fails, make sure to close any Java SE Runtime Binary processes with Task Manager.
    * run `mm play` with the `--logfile [filename]` option set from within Bash Subsystem for Windows to get a log for a game
         * then run `mm play` with the `--input [filename]` option set from the Windows command prompt to view the game
         * This also means you can't run `mm play human` from bash subsystem

5. To run two different bots against each other, run `mm play bot1_directory bot2_directory`.
6. To submit your bot, run `mm push .`

Use `mm help` for more information!

# Game API Information
The header file (`strategy.h`) has a list of functions that you should implement, and a list of functions to get data from the game are in `api.h`.  

*Note: you should not need to edit the files inside of `Resources`.*  

Good luck, and happy coding!

# Boost Installation FAQ
## Mac
```
brew install boost
```

## Linux and Windows Ubuntu Subsystem
```
apt-get install libboost-all-dev
```

