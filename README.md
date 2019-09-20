# MechMania25 C++ Starter Pack

Here's all the code you need to get started with making a bot for MechMania in C++. Just do these steps:

* Pre-Setup -- install Java, Node, and the `mm` command line tools
* Setup -- Clone this repository and start running your bot!

# Pre-Setup

1. Install or update clang++ to compile your C++ code! You can check out [this guide on getting started with clang](https://clang.llvm.org/get_started.html)

2. Install Java. To do this, see [this guide](https://docs.oracle.com/en/java/javase/13/install/overview-jdk-installation.html#GUID-8677A77F-231A-40F7-98B9-1FD0B48C346A) for help.
    * (This is used for the visualization of the game)
    * Advice for Windows users:
        * Make sure to set the `JAVA_HOME` variable as a SYSTEM environment variable, rather than a user environment variable.
        * Make sure to NOT have `bin` at the end of your `JAVA_HOME` environment variable.
    * Check that the `JAVA_HOME` environment variable is set correctly.
        * For Windows, you can run `echo %JAVA_HOME%`. You should see a result similar to `C:\Program Files\Java\jdk1.8.0_171`. Note that this does NOT end with `\bin`.
        * For Mac and Linux users, you can run `echo $JAVA_HOME`. You should see a result similar to `/Library/Java/JavaVirtualMachines/jdk1.8.0_45.jdk/Contents/Home`.

3. Install Node. To do this, go [here](https://nodejs.org/en/download/) and download the appropriate installer for your operating system.
    * Run the installer with all the defaults.

4. Run `npm install -g mechmania`.  This gets the `mm` command line tools, which are used to run the game, test and submit bots for the tournament.

5. Run `mm download` to download required files.

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

5. To run two different bots against each other, run `mm play bot1_directory bot2_directory`.
6. To submit your bot, run `mm push .`

Use `mm help` for more information!

# Game API Information
The header file (`strategy.h`) has a list of functions that you should implement, and a list of functions to get data from the game are in `api.h`.  

*Note: you should not need to edit the files inside of `Resources`.*  

Good luck, and happy coding!

# Boost Installation FAQ
## Mac and Linux
```
brew install boost
```

## Windows

