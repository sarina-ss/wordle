/**
 * Author: Sarina Sukhiani
 * Student Number: 251152869
 * Date: October 3, 2023
 * Description: Wordle file that does all the background processing of the wordle Wt application,
 * including taking in user guesses, providing feedback and displaying user guesses on the screen.
 */

#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include "wordle.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

/*
 * The env argument contains information about the new session, and
 * the initial request. It must be passed to the WApplication
 * constructor so it is typically also an argument for your custom
 * application constructor.
 */

wordle::wordle(const Wt::WEnvironment &env)
    : WApplication(env), guessCounter(0)
{
    /**
     * Value is assigned to the random variable using the getRandom function
     */

    random = getRandom();

    /**
     * Application title is set as Wordle
     */

    setTitle("Wordle");

    /**
     * Past guesses container is created with a widget to ensure all the past guesses that
     * the user inputs are displayed on the screen.
     */

    pastGuessesContainer_ = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
    root()->addWidget(std::make_unique<Wt::WBreak>());

    /**
     * Text prompting the user to enter a guess with a maximum of 5 letters is outputted below
     * the user input textbox.
     */

    root()->addWidget(std::make_unique<Wt::WText>("Enter guess, 5 letters maximum: ")); // show some text

    /**
     * A nameEdit widget is created to allow the user to input text, and the focus is set
     * to the input box.
     */

    nameEdit_ = root()->addWidget(std::make_unique<Wt::WLineEdit>());
    nameEdit_->setFocus();

    /**
     * Submit button is created with a 5 pixel margin.
     */

    auto button = root()->addWidget(std::make_unique<Wt::WPushButton>("Submit"));
    button->setMargin(5, Wt::Side::Left);

    /**
     * Line break is added, greeting is made as a widget with empty text.
     */

    root()->addWidget(std::make_unique<Wt::WBreak>());
    greeting_ = root()->addWidget(std::make_unique<Wt::WText>());

    /*
     * Connect signals with slots:
     * If submit button is clicked, the checkGuess function will be prompted.
     *
     * - simple Wt-way: specify object and method
     */

    button->clicked().connect(this, &wordle::checkGuess);

    /*
     * Connect signals with slots:
     * If 'enter' on the keyboard is hit, the checkGuess function will be prompted.
     *
     * - using an arbitrary function object, e.g. useful to bind
     *   values with std::bind() to the resulting method call
     */

    nameEdit_->enterPressed().connect(std::bind(&wordle::checkGuess, this));
}

/*
 * Get random function searches through the wordlist.txt file and returns a random word from that file.
 */

std::string wordle::getRandom()
{
    /**
     * Ensures the random words generated by the program are different each time it is run.
     */

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    /**
     * Opens the file.
     */

    std::ifstream file("wordlist.txt");

    /**
     * Returns an empty string to indicate that there is an erorr if the file doesn't open.
     */

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return "";
    }
    /**
     * Uses a vector to read the words from the wordlist.txt file.
     */

    std::vector<std::string> words;
    std::string word;

    while (std::getline(file, word))
    {
        words.push_back(word);
    }

    /**
     * Checks if the file is empty - if so it returns an empty string to indicate there is an error.
     */

    if (words.empty())
    {
        std::cerr << "Error: No words found in the file." << std::endl;
        return "";
    }

    /**
     * Generates a random index to select a word using a modulus function on the size of the words list.
     */

    int randomIndex = std::rand() % words.size();

    /**
     * Retrieves the random word using the randomIndex generated.
     */

    std::string randomWord = words[randomIndex];

    /**
     * Closes the file.
     */

    file.close();

    /**
     * Returns whichever random word was generated from the txt file.
     */

    return randomWord;
}

/**
 * bool checkConditions function returns true or false based on whether the user's guess
 * matches the conditions or not. It takes in two string variables, one is the guess word
 * which will be checked against the conditions and the other is the random word that was
 * generated which will only be used to pass to the highlightWord function if the user has
 * run out of guesses.
 */

bool wordle::checkConditions(std::string guess, std::string random)
{

    /*
     * It first checks if the guess length is 5 letters long,
     * and if it isn't, it prompts the user to enter a 5-letter word
     * in the greeting widget. It then sets the textbox back to blank and returns false.
     */

    if (guess.length() != 5)
    {
        greeting_->setText("Please enter a 5-letter word.");
        nameEdit_->setText("");
        return false;
    }

    /*
     *It then checks if the user has run out of guesses (since guessCounter started at 0, it checks if
     * the guessCounter is 5 guesses). If the user has run out of guesses, it puts the guess word and the
     * random word into the highlightWord function (described below). It then sets the message
     * in the greeting widget to let the user know they ran out of guesses and tells them what
     * the correct word was. It then returns false.
     */

    if ((guessCounter == 5) && (guess != random))
    {
        highlightWord(guess, random);
        nameEdit_->setText("");
        greeting_->setText("Sorry! You ran out of guesses! The word was: " + random);
        return false;
    }

    /*
     * If none of these conditions prompt the function to return false, the guess passed the
     * conditions check and the function will return true.
     */

    return true;
}

/*
 * void highlightWord function takes the guess word and the random word and compares them to highlight
 * them according to if the letter is in the correct position, if the letter guessed is in the random word
 * or if the letter is not in the random word at all. Its parameters are 2 strings: guess and random, which
 * is the user's guess and the random word previously generated.
 */

void wordle::highlightWord(std::string guess, std::string random)
{

    /**
     * Transforms the guess word and random word to uppercase to ensure accuracy when comparing.
     */

    std::transform(guess.begin(), guess.end(), guess.begin(),
                   [](unsigned char c)
                   { return std::toupper(c); });

    std::transform(random.begin(), random.end(), random.begin(),
                   [](unsigned char c)
                   { return std::toupper(c); });

    /**
     * New string is created to add each letter based on what colour they are highlighted.
     */

    std::string newWord;

    /**
     * Loops through each letter of the guess word and the random word and compares them.
     */

    for (int i = 0; i < 5; ++i)
    {
        char letter1 = guess[i];
        char letter2 = random[i];

        /**
         * If the letters match and are in the same index, the letter is in the correct spot and is highlighted green.
         */

        if (letter1 == letter2)
        {
            newWord += "<span style ='color: green; font-weight:bold'>" + std::string(1, guess[i]) + "</span>";
        }

        /**
         * If the letter in the guess word matches one of the letters in the random word, it is highlighted yellow
         * to show that it is a correct letter in the word but in the wrong spot.
         */

        else if (random.find(letter1) != std::string::npos)
        {
            newWord += "<span style ='color:yellow; font-weight:bold'>" + std::string(1, guess[i]) + "</span>";
        }

        /**
         * If the letter matches none of these conditions, it is highlighted grey.
         */

        else
        {
            newWord += "<span style ='color:gray; font-weight:bold'>" + std::string(1, guess[i]) + "</span>";
        }
    }

    /**
     * Inputs the guess into the past guess container to display on the screen with the according letters highlighted.
     */

    auto newWordText = std::make_unique<Wt::WText>();
    newWordText->setInline(true);
    newWordText->setText(Wt::WString::fromUTF8(newWord));

    pastGuessesContainer_->addWidget(std::move(newWordText));
    pastGuessesContainer_->addWidget(std::make_unique<Wt::WBreak>());

    return;
}

/*
 * void checkGuess function is called when enter or submit is clicked after the user has entered input in the
 * textbox prompting them for a guess. It passes the guess word through the checkConditions function, and then
 * passes it through the highlightWord function and finally determines whether or not the guess is right.
 * Based on if the guess is correct or not, it generates a message telling the user to either try again, or
 * congratulates them on their correct guess.
 */

void wordle::checkGuess()
{

    /**
     * Creates a string guess word from the input in the nameEdit textbox.
     */

    std::string guess = nameEdit_->text().toUTF8();

    /**
     * If the guess word fails the condition check, the user is re prompted to enter a guess word.
     */

    if (!checkConditions(guess, random))
    {
        return;
    }

    /**
     * If the guess word passes the condition check, the word is passed through the highlightWord function
     * to ensure the letters are highlighted accordingly and show up on the screen in the past guesses container.
     */

    highlightWord(guess, random);

    /**
     * If the guess word is the same as the random word, the user is congratulated and told their guess is correct.
     */

    if (guess == random)
    {
        greeting_->setText("Congratulations! Your guess is correct.");
        nameEdit_->setText("");
    }

    /**
     * If the guess word is not the same as the random word, the user is prompted to re enter a word and
     * the input box is cleared. GuessCounter is incremented to ensure users can only have 6 guesses.
     */

    else
    {
        greeting_->setText("Sorry, that's not correct. Try again.");
        nameEdit_->setText("");
        guessCounter++;
    }
    return;
}