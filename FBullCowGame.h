/*
The game logic (no view or direct user interaction
The game is a simple guess the word based on Mastermind
*/

#pragma once
#include <string>
#include <map>
#include <vector>

//to make syntax Unreal friendly
#define TMap std::map 
#define Vector std::vector 
using FString = std::string;
using int32 = int;

struct FBullCowCount 
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

enum class EGuessStatus 
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Wrong_Length,
	No_Lowercase,
	Hint,
	Error
};

enum class EResetStatus {
	No_Hidden_Word,
	OK
};

class FBullCowGame
{
public:
	FBullCowGame(); //constructor

	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;
	bool IsGameWon() const;
	EGuessStatus CheckGuessValidity(FString) const;
	void CreateDictionary();
	int32 GetTotalHints() const;
	void SetTotalHints(int32);
	int32 GetNumberOfWords() const;
	void Reset();
	char ShowLetterHint();
	FBullCowCount SubmitValidGuess(FString);
	void Score();
	void SetScore(int32);
	int32 GetScore() const;
	FString GetHiddenWord();
	int32 GetRemainingWords() const;
	bool CheckRemainingWords() const;

private:
	//see constructor for initialisation
	int32 MyCurrentTry;
	int32 ScoreResult;
	FString MyHiddenWord;
	int32 NumberOfWords;
	int32 TotalHints;
	int32 LengthOfDictionary;
	int32 NumberOfRemainingWords;
	Vector<FString> Dictionary;
	TMap<FString, bool> WordSeen;
	TMap<char, bool> HintLetterCheckMap;
	bool bGameIsWon;
	bool IsIsogram(FString) const;
	bool IsLowerCase(FString) const;
};
