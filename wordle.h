/**
 * Author: Sarina Sukhiani
 * Student Number: 251152869
 * Date: October 3, 2023
 * Description: Header file to declare all public and private variables and functions for the wordle game.
 * Description will be continued below.
 */

/**
 * Include statements ensure the header files necessary to building a Wt application are imported.
 */
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

/*
 * A simple Wordle application class which demonstrates how to react
 * to events, read input, and give feedback.
 */

class wordle : public Wt::WApplication
{
public:
    wordle(const Wt::WEnvironment &env);

private:
    Wt::WLineEdit *nameEdit_;
    Wt::WText *greeting_;
    Wt::WContainerWidget *pastGuessesContainer_;
    int guessCounter;
    std::string random;

    std::string getRandom();
    void checkGuess();
    void highlightWord(std::string, std::string);
    bool checkConditions(std::string, std::string);
};
