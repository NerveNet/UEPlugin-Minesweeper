// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperSettings.h"




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

	if (propertyName == GET_MEMBER_NAME_CHECKED(UMinesweeperSettings, AddNewHighScore))
	{
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

	if (propertyName == GET_MEMBER_NAME_CHECKED(UMinesweeperSettings, AddNewHighScore))
	{
		HighScores.Insert(FMinesweeperHighScore(LastPlayerName, FMath::RandRange(5000, 200000), FMath::RandRange(20, 300), FMath::RandRange(50, 2000)), 0);
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

	LastDifficulty = SMinesweeperWindow::DefaultDifficulty;

	CellDrawSize = 28.0f;
	UseGridCanvas = false;
}