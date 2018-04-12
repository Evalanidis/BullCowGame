#pragma once
#include "FBullCowGame.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <random>
#include <type_traits>
#include <algorithm>

inline std::default_random_engine& randint_engine()
{
	static thread_local std::default_random_engine eng{ std::random_device{}() };
	return eng;
}
template<typename IntType>
inline IntType randint(IntType a, IntType b)
{
	static_assert(std::is_integral<IntType>::value, "argument must be an integer type");
	using dist_t = std::uniform_int_distribution<IntType>;
	return dist_t(a, b)(randint_engine());
}

//to make syntax Unreal friendly
using int32 = int;
// default constructor

FBullCowGame::FBullCowGame() 
{
	CreateDictionary();
	TotalHints = 3;
} 

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }
void FBullCowGame::SetScore(int32 score) { ScoreResult = score; }
int32 FBullCowGame::GetScore() const { return ScoreResult; }
FString FBullCowGame::GetHiddenWord() { return MyHiddenWord; }
//void FBullCowGame::SetTotalHints(int32 num) { TotalHints = num; }
int32 FBullCowGame::GetTotalHints() const { return TotalHints; }
int32 FBullCowGame::GetNumberOfWords() const { return NumberOfWords; }
int32 FBullCowGame::GetRemainingWords() const { return Dictionary.size(); }
bool FBullCowGame::CheckRemainingWords() const { return GetRemainingWords() == 0; }

int32 FBullCowGame::GetMaxTries() const 
{ 
	TMap<int32, int32> WordLengthToMaxTries{ {3,4},{4,7},{5,10},{6,15},{7,20}};
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

void FBullCowGame::Reset() 
{
	LengthOfDictionary = Dictionary.size();
	MyHiddenWord = Dictionary[randint(0, LengthOfDictionary)];
	Dictionary.erase(std::remove(Dictionary.begin(), Dictionary.end(), MyHiddenWord), Dictionary.end());
	TotalHints = GetTotalHints();
	MyCurrentTry = 1;
	bGameIsWon = false;

	if (!HintLetterCheckMap.empty())
	{
		HintLetterCheckMap.clear();
	}
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{

	if (Guess == "hint")// hint 
	{
		return EGuessStatus::Hint;
	}
	else if (!IsIsogram(Guess))// if the guess is not an isogramm 
	{
		return EGuessStatus::Not_Isogram;
	}	
	else if (!IsLowerCase(Guess)) // if is not all lowercase
	{
		return EGuessStatus::No_Lowercase; 
	}
	else if (Guess.length() != GetHiddenWordLength()) //if length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else 
	{
		return EGuessStatus::OK;
	}
	
	return EGuessStatus::Error;
}

void FBullCowGame::CreateDictionary()
{
	std::ifstream inFile;
	FString word;
	inFile.open("WordDictionary.txt");
	if (!inFile) 
	{
		std::cerr << "Unable to open file WordDictionary.txt";
		exit(1);   // call system to stop
	}

	while(inFile >>word) 
	{
		Dictionary.push_back(word);
	}

	inFile.close();
	return;
}

char FBullCowGame::ShowLetterHint()
{
	int32 LenghtOfWord = MyHiddenWord.length();
	char hint = MyHiddenWord[randint(0, LenghtOfWord)];
	if (TotalHints > 0 )
	{
		// makes sure to not show a previous hint letter again
		do
		{
			if (!HintLetterCheckMap[hint])
			{
				HintLetterCheckMap[hint] = true;
				TotalHints--;
				return hint;
			}  
			else 
			{
				hint = MyHiddenWord[randint(0, LenghtOfWord)];
			}
		} while (true);
	}
	else
	{
		return '\0';
	}	
}

// receives a valid guess, increments turn and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

	// loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
	{
		for (int32 GChar = 0; GChar < WordLength; GChar++)
		{
			if ((Guess[MHWChar] == MyHiddenWord[GChar])) 
			{
				if (MHWChar == GChar) 
				{
					BullCowCount.Bulls++;
				}
				else 
				{
					BullCowCount.Cows++;
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength) 
	{
		bGameIsWon = true;
		TotalHints++;
		Score();
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}

void FBullCowGame::Score()
{
	if (GetCurrentTry() - 1 > 9) 
	{
		ScoreResult += 10;
	}
	else if (GetCurrentTry() - 1  == 1) 
	{
		ScoreResult += 200;
	}
	else
	{
		ScoreResult += 100 - ((GetCurrentTry() -1 ) * 10);
	}
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	// treat 0 and 1 letter strings words as isograms
	if (Word.length() <= 1) return true;

	TMap<char, bool> LetterSeen;//setup our map
	for (auto Letter : Word)
	{
		if (LetterSeen[tolower(Letter)])
		{
			return false;
		}
		else 
		{
			LetterSeen[Letter] = true;
		}
	}
	return true;
}

bool FBullCowGame::IsLowerCase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!islower(Letter))
		{
			return false;
		}
	}
	return true;
}
