// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    ProcessWordList();
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    ClearScreen();

    if (bGameOver)
        SetupGame();
    else ProcessGuess(PlayerInput);
}

void UBullCowCartridge::ProcessWordList() 
{
    const FString WordsFilePath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordsFilePath, [](const FString& Word) { 
        return Word.Len() >= 4 && Word.Len() <= 8 && UBullCowCartridge::IsIsogram(Word); 
    });

    //PrintLine(TEXT("The number of valid words is %i\n"), Isograms.Num());
}

void UBullCowCartridge::SetupGame()
{
    int32 IsogramsLength = Isograms.Num();
    if (IsogramsLength <= 0) 
    {
        PrintLine(TEXT("Error: Unable to load a hidden word.\n"));
        return;
    }

    HiddenWord = Isograms[FMath::RandRange(0, IsogramsLength - 1)];
    Lives = HiddenWord.Len() * 8;
    bGameOver = false;

    PrintLine(TEXT("Welcome to the Bull Cows game!"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Type your guess and then press ENTER.\nYou have %i lives.\n"), Lives);
    //PrintLine(TEXT("The HiddenWord is: %s"), *HiddenWord); //Debug Line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;

    PrintLine(TEXT("The game is over.\nThe hidden word was: %s\nPress ENTER to continue.\n"), *HiddenWord);
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (HiddenWord.ToLower().Equals(Guess.ToLower())) 
    {
        PrintLine(TEXT("You have guessed the word!\n"));
        EndGame();
        return;
    }

    --Lives;

    PrintLine(TEXT("Your health decreased.\nYou now have %i lives.\n"), Lives);

    if (HiddenWord.Len() != Guess.Len()) 
    {
        PrintLine(TEXT("The Hidden Word is %i characters long.\n"), HiddenWord.Len());
        CheckLives();
        return;
    }

    if (!IsIsogram(Guess)) 
    {
        PrintLine(TEXT("The guess word must be an isogram.\n"));
        CheckLives();
        return;
    }

    FBullCowCount Count = GetBullCows(Guess);

    PrintLine(TEXT("The guess word doesn't match.\nYou have %i bulls and %i cows.\n"), Count.Bulls, Count.Cows);
    CheckLives();
}

void UBullCowCartridge::CheckLives() 
{
    if (Lives > 0)
        PrintLine(TEXT("Type your word. Press ENTER to continue."));
    else EndGame();
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    int32 Length = Guess.Len();
    int32 HiddenWordLength = HiddenWord.Len();

    for (int GuessIndex = 0; GuessIndex < Length; GuessIndex++) 
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex]) 
        {
            Count.Bulls++;
            continue;
        }
        
        for (int HiddenIndex = 0; HiddenIndex < HiddenWordLength; HiddenIndex++) 
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex]) 
            {
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
}

bool UBullCowCartridge::IsIsogram(const FString& Guess)
{
    bool bRet = true;
    int32 Length = Guess.Len() - 1; // Not required to make the check on the last letter (they are done on previous iterations), hence Len - 1

    for (int Index = 0; Index < Length; Index++)
    {
        for (int Comparison = Index + 1; Comparison < Guess.Len(); Comparison++)
        {
            if (Guess[Comparison] == Guess[Index])
            {
                bRet = false;
                break;
            }
        }

        if (!bRet)
            break;
    }

    return bRet;
}