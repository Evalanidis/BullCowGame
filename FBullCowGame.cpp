#pragma once
#include "FBullCowGame.h"
#include <fstream>
#include <iostream>
#include <ctime>

//to make syntax Unreal friendly
using int32 = int;
TMap<FString, bool> WordSeen;
TMap<char, bool> HintLetterCheckMap;

int32 LengthOfDictionary;
int32 RemainingWords;
int32 TotalHints = 3;

//FBullCowGame::FBullCowGame() { Reset(); } // default constructor
FBullCowGame::FBullCowGame() { CreateDictionary(); }

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }
void FBullCowGame::SetScore(int32 score) { this->ScoreResult = score; }
int32 FBullCowGame::GetScore() const { return ScoreResult; }
FString FBullCowGame::GetHiddenWord() {return MyHiddenWord;}
int32 FBullCowGame::GetTotalHints() const { return TotalHints; }
void FBullCowGame::SetNumberOfWords(int32 i) { NumberOfWords = i; }
int32 FBullCowGame::GetNumberOfWords() const { return NumberOfWords; }
void FBullCowGame::SetRemainingWords(int32 num) { NumberOfRemainingWords = num; }
int32 FBullCowGame::GetRemainingWords() const { return NumberOfRemainingWords; }
bool FBullCowGame::CheckRemainingWords() const { return GetRemainingWords() == 0; }
int32 FBullCowGame::GetMaxTries() const 
{ 
	TMap<int32, int32> WordLengthToMaxTries{ {3,4},{4,7},{5,10},{6,15},{7,20}};
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

void FBullCowGame::Reset() 
{
	do {
		LengthOfDictionary = GetNumberOfWords();
		srand(time(NULL));
		MyHiddenWord = Dictionary[rand() % LengthOfDictionary];
	} while (WordSeen[MyHiddenWord]);
	WordSeen[MyHiddenWord] = true;
	
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
	int32 i = 0;
	inFile.open("WordDictionary.txt");
	if (!inFile) 
	{
		std::cerr << "Unable to open file WordDictionary.txt";
		exit(1);   // call system to stop
	}
	while(inFile >>word) {
		Dictionary[i] = word;
		i++;
	}
	inFile.close();
	SetNumberOfWords(i);
	return;
}

char FBullCowGame::ShowLetterHint() const
{
	char hint = MyHiddenWord[rand() % MyHiddenWord.length()];
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
				hint = MyHiddenWord[rand() % MyHiddenWord.length()];
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
