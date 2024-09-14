/**
 * Author: Sarina Sukhiani
 * Student Number: 251152869
 * Date: October 3, 2023
 * Description: Main file to ensure the program is able to be executed.
 */

/**
 * Imports the Wt application header to ensure the program works with Wt. Also imports the
 * wordle header file and various other C++ libraries that will be used in the wordle.cpp file.
 */

#include <Wt/WApplication.h>
#include "wordle.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

/**
 * Main function of the program where execution begins.
 */

int main(int argc, char **argv)
{

    /**
     * Sets up and runs the Wt application. Creates a new instance of wordle.
     */

    return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env)
                    { return std::make_unique<wordle>(env); });
}
