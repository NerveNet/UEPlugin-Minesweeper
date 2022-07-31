// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperSettings.h"
#include "SMinesweeper.h"




FMinesweeperDifficulty const FMinesweeperDifficulty::Beginner = FMinesweeperDifficulty(9, 9, 10);		// mine probability: 10 /  81 = 0.12345%
FMinesweeperDifficulty const FMinesweeperDifficulty::Intermediate = FMinesweeperDifficulty(16, 16, 40);	// mine probability: 40 / 256 = 0.15625%
FMinesweeperDifficulty const FMinesweeperDifficulty::Expert = FMinesweeperDifficulty(30, 16, 99);		// mine probability: 99 / 576 = 0.17187%


UMinesweeperSettings::UMinesweeperSettings(const FObjectInitializer& Initializer) : Super(Initializer)
{
	ResetToDefaults();
}


#if WITH_EDITOR
bool UMinesweeperSettings::CanEditChange(const FProperty* InProperty) const
{
	const bool bSuper = Super::CanEditChange(InProperty);

	if (!InProperty) return bSuper;

	const FName propertyName = InProperty->GetFName();
	const FString propertyNameStr = InProperty->GetName();

	if (
		propertyName == GET_MEMBER_NAME_CHECKED(UMinesweeperSettings, LastPlayerName) || 
		propertyName == GET_MEMBER_NAME_CHECKED(UMinesweeperSettings, LastSettings)
	) {
		// framework for setting enable/disable

		return bSuper;
	}

	return bSuper;
}

void UMinesweeperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName propertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	const FName memberPropertyName = PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;
	const FString memberPropertyStr = PropertyChangedEvent.MemberProperty->GetName();

	if (
		propertyName == GET_MEMBER_NAME_CHECKED(UMinesweeperSettings, LastPlayerName) ||
		propertyName == GET_MEMBER_NAME_CHECKED(UMinesweeperSettings, LastSettings)
	) {
		// framework for post setting change
	}
	else if (propertyName == GET_MEMBER_NAME_CHECKED(UMinesweeperSettings, AddNewHighScore))
	{
		HighScores.Insert(FMinesweeperHighScore(LastPlayerName, FMath::FRandRange(5000, 200000), FMath::FRandRange(20, 300), FMath::FRandRange(50, 2000)), 0);
		HighScores.RemoveAt(HighScores.Num() - 1);
		AddNewHighScore = false;
	}

	OnSettingsChanged.Broadcast(PropertyChangedEvent);

	SaveConfig();
}
#endif


void UMinesweeperSettings::ResetToDefaults()
{
	ShowToolbarButton = true;

	UseDockTab = false;

	LastPlayerName = "Some_Randy";

	HighScores.Empty();
	HighScores.Add(FMinesweeperHighScore(LastPlayerName, 100,  100.0f)); // index 0 is rank 1
	HighScores.Add(FMinesweeperHighScore(LastPlayerName,  90,  200.0f));
	HighScores.Add(FMinesweeperHighScore(LastPlayerName,  80,  300.0f));
	HighScores.Add(FMinesweeperHighScore(LastPlayerName,  70,  400.0f));
	HighScores.Add(FMinesweeperHighScore(LastPlayerName,  60,  500.0f));
	HighScores.Add(FMinesweeperHighScore(LastPlayerName,  50,  600.0f));
	HighScores.Add(FMinesweeperHighScore(LastPlayerName,  40,  700.0f));
	HighScores.Add(FMinesweeperHighScore(LastPlayerName,  30,  800.0f));
	HighScores.Add(FMinesweeperHighScore(LastPlayerName,  20,  900.0f));
	HighScores.Add(FMinesweeperHighScore(LastPlayerName,  10, 1000.0f)); // index 9 is rank 10

	LastSettings = SMinesweeper::DefaultDifficulty;
}