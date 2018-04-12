/*
This is  the console executable, that makes use of BullCow class
This Acts as the view in MVC pattern , and  is responsible for all
user interaction. For game logic see FBullCowGame class 
*/
#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

//to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// finction prototypes as outside a class
void PrintIntro();
void PlayGame();
bool AskToPlayAgain();
FText GetValidGuess();
void PrintGameSummary();
void CheckHintValidity();
void FinalMessage();
FBullCowGame BCGame; // instantiate a new game, which we re-use accross plays

//the entry point for our application
int main() 
{
	
	BCGame.SetTotalHints(3);
	BCGame.SetScore(0);
	
	do
	{
		BCGame.Reset();
		PrintIntro();
		PlayGame();
		BCGame.SetRemainingWords(BCGame.GetRemainingWords() - 1);
	} 
	while (!BCGame.CheckRemainingWords() && AskToPlayAgain());
	
	FinalMessage();
	
	return 0; //exit application
}


void PrintIntro()
{
	std::cout << "\n\nWelcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "  {    } " << std::endl;
	std::cout << "   ^--^    ^--^ " << std::endl;
	std::cout << "  ( o o ) ( o o )" << std::endl;
	std::cout << "    (..)    (..)" << std::endl;
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	std::cout << std::endl;
	std::cout << "Your Score : " << BCGame.GetScore() << std::endl;
	std::cout << "Hints : " << BCGame.GetTotalHints() << std::endl;
	return;
}

// Plays a single game to completion
void PlayGame()
{

	int32 MaxTries = BCGame.GetMaxTries();
	FText Guess ="";
	// loop asking for guess while the game 
	// is NOT won and there are still tries remaining
	while(!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)
	{
		Guess = GetValidGuess();
		// Submit valid guess to the Game, and receive counts
		FBullCowCount BullCowCount =  BCGame.SubmitValidGuess(Guess);
		
		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << " Cows = " << BullCowCount.Cows << "\n\n";
	}
	
	PrintGameSummary();
	return;
}

// loop continueally until the user gives a valid guess
FText GetValidGuess()
{
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	FText Guess = "";
	do
	{
		//get the guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries() << ". Enter your Guess: ", getline(std::cin, Guess);

		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::No_Lowercase:
			std::cout << "Please enter all lowercase letters.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;
		case EGuessStatus::Hint:
			CheckHintValidity();
			break;
		case EGuessStatus::Error:
			std::cout << "Unknown Error!";
			break;
		default:
			// assume guess is valid
			break;
		}
		
	} while (Status != EGuessStatus::OK);
	return Guess;
}

bool AskToPlayAgain()
{
	FText Response = "";
	std::cout << "Do you want to play again (y/n)?", getline(std::cin, Response);
	return (Response[0] == 'y' || Response[0] == 'Y');
}

void PrintGameSummary() 
{
	if (BCGame.IsGameWon()) 
	{
		std::cout << "Your Score is : " << BCGame.GetScore() << std::endl;
		std::cout << "YOU WON" << std::endl;
	}
	else if (BCGame.GetCurrentTry() > BCGame.GetMaxTries())
	{
		std::cout << "The hidden word was : " << BCGame.GetHiddenWord() << std::endl;
		std::cout << "Your Score was : " << BCGame.GetScore() << std::endl;
		std::cout << "GAME OVER" << std::endl;
	}
	return;
}

void CheckHintValidity()
{
	char HintEvaluation = BCGame.ShowLetterHint();
	if (HintEvaluation == '\0')
	{
		std::cout << "Sorry you do not have any Hints left! \n\n";
	}
	else 
	{
		std::cout << "Hint! The letter \' " << HintEvaluation << " \' is in the hidden word!\n\n";
	}
}

void FinalMessage() 
{
	if (BCGame.CheckRemainingWords()) 
	{
		std::cout << "\n\nCongratulations!\nThere are no remaining words to play!" << std::endl;
		std::cout << "Final Score: " << BCGame.GetScore() << std::endl;
	}
	else 
	{
		std::cout << "\n\nYou still had " << BCGame.GetRemainingWords()  << " remaining words!" << std::endl;
		std::cout << "Final Score: " << BCGame.GetScore() << std::endl;
	}

}
