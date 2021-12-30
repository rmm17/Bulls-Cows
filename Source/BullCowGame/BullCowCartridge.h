// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void ProcessWordList();
	void SetupGame();
	void EndGame();
	void ProcessGuess(FString Guess);
	void CheckLives();
	bool IsIsogram(FString Guess) const;
	int32 NumberOfBulls(FString Guess) const;
	int32 NumberOfCows(FString Guess) const;
	TArray<FString> GetValidWords(TArray<FString> Words) const;

	// Your declarations go below!
	private:
	TArray<FString> WordList;
	FString HiddenWord;
	int32 Lives;
	bool bGameOver;
};
