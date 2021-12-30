// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    ProcessWordList();
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();

    if (bGameOver) {
        SetupGame();
    }
    else {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::ProcessWordList() 
{
    TArray<FString> WordsFile;

    const FString WordsFilePath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(WordsFile, *WordsFilePath);

    WordList = GetValidWords(WordsFile);

    //PrintLine(TEXT("The number of possible words is %i\n"), WordsFile.Num());
    //PrintLine(TEXT("The number of valid words is %i\n"), WordList.Num());
}

void UBullCowCartridge::SetupGame()
{
    int32 HiddenWordListLength = WordList.Num();
    if (HiddenWordListLength <= 0) {
        PrintLine(TEXT("Error: Unable to load a hidden word.\n"));
        return;
    }

    HiddenWord = WordList[rand() % HiddenWordListLength];
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("The HiddenWord is: %s"), *HiddenWord); //Debug Line

    PrintLine(TEXT("Welcome to the Bull Cows game!"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Type your guess and then press ENTER.\nYou have %i lives.\n"), Lives);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;

    PrintLine(TEXT("The game is over. Press ENTER to continue...\n"));
}

void UBullCowCartridge::ProcessGuess(FString Guess)
{
    if (HiddenWord.ToLower().Equals(Guess.ToLower())) {
        PrintLine(TEXT("You have guessed the word!\n"));
        EndGame();
        return;
    }

    --Lives;

    PrintLine(TEXT("Your health decreased.\nYou now have %i lives.\n"), Lives);

    if (HiddenWord.Len() != Guess.Len()) {
        PrintLine(TEXT("The Hidden Word is %i characters long.\n"), HiddenWord.Len());
        CheckLives();
        return;
    }

    PrintLine(TEXT("There are %i bulls and %i cows.\n"), NumberOfBulls(Guess), NumberOfCows(Guess));

    if (!IsIsogram(Guess)) {
        PrintLine(TEXT("The Guess Word must be an Isogram.\n"));
        CheckLives();
        return;
    }

    PrintLine(TEXT("The Guess Word doesn't match the Hidden Word.\n"));
}

void UBullCowCartridge::CheckLives() 
{
    if (Lives > 0) {
        PrintLine(TEXT("Type your word. Press ENTER to continue."));
    }
    else {
        EndGame();
    }
}

bool UBullCowCartridge::IsIsogram(FString Guess) const
{
    bool bRet = true;
    int32 Length = Guess.Len() - 1; // Not required to make the check on the last letter (they are done on previous iterations), hence Len - 1

    for (int Index = 0; Index < Length; Index++) {
        for (int Comparison = Index + 1; Comparison < Guess.Len(); Comparison++) {
            if (Guess[Comparison] == Guess[Index]) {
                bRet = false;
                break;
            }
        }

        if (!bRet) {
            break;
        }
    }

    return bRet;
}

int32 UBullCowCartridge::NumberOfBulls(FString Guess) const
{
    int32 BullsCount = 0;
    int32 Length = Guess.Len();

    for (int Index = 0; Index < Length; Index++) {
        if (Guess[Index] == HiddenWord[Index]) {
            BullsCount++;
        }
    }

    return BullsCount;
}

int32 UBullCowCartridge::NumberOfCows(FString Guess) const {
    int32 CowsCount = 0;
    int32 GuessLength = Guess.Len();
    int32 HiddenWordLength = HiddenWord.Len();

    // Should need to not repeat Cows count. Revise code.
    for (int Index = 0; Index < GuessLength; Index++) {
        for (int Comparison = 0; Comparison < HiddenWordLength; Comparison++) {
            if (Comparison != Index && Guess[Index] == HiddenWord[Comparison]) {
                CowsCount++;
            }
        }
    }

    return CowsCount;
}

TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> Words) const
{
    TArray<FString> ValidWords;

    for (FString Word : Words) {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)) {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}