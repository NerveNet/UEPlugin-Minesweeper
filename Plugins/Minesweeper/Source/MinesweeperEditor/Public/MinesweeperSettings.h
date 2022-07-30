// Copyright 2022 Brad Monahan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MinesweeperEditorModule.h"
#include "MinesweeperSettings.generated.h"




/**
 * Holds game information about the difficulty.
 */
USTRUCT()
struct MINESWEEPEREDITOR_API FMinesweeperDifficulty
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Config, EditAnywhere, Category = "Difficulty")
		int32 Width = 1;

	UPROPERTY(Config, EditAnywhere, Category = "Difficulty")
		int32 Height = 1;

	UPROPERTY(Config, EditAnywhere, Category = "Difficulty")
		int32 MineCount = 1;

	FMinesweeperDifficulty() { }
	FMinesweeperDifficulty(const int32 InWidth, const int32 InHeight, const int32 InMineCount)
		: Width(InWidth), Height(InHeight), MineCount(InMineCount) { }

	bool operator ==(const FMinesweeperDifficulty& InOther) const
	{
		return (Width == InOther.Width && Height == InOther.Height && MineCount == InOther.MineCount);
	}
	bool operator !=(const FMinesweeperDifficulty& InOther) const
	{
		return !(*this == InOther);
	}

	int32 TotalCells() const { return Width * Height; }

	bool IsBeginner() const { return *this == Beginner; }
	bool IsIntermediate() const { return *this == Intermediate; }
	bool IsExpert() const { return *this == Expert; }

	static const FMinesweeperDifficulty Beginner;
	static const FMinesweeperDifficulty Intermediate;
	static const FMinesweeperDifficulty Expert;
};




/**
 * Data representation for a single user high score.
 */
USTRUCT()
struct MINESWEEPEREDITOR_API FMinesweeperHighScore
{
	GENERATED_USTRUCT_BODY()

	/** Name of the user who achieved the high score. */
	UPROPERTY(Config, EditAnywhere, Category = "HighScore")
		FString Name = "";

	/** The calculated high score. */
	UPROPERTY(Config, EditAnywhere, Category = "HighScore")
		int32 Score = 0;

	/** The time length in seconds the user took to achieve the high score. */
	UPROPERTY(Config, EditAnywhere, Category = "HighScore")
		float Time = 0;

	UPROPERTY(Config, EditAnywhere, Category = "HighScore")
		int32 Clicks = 0;

	FMinesweeperHighScore() { }
	FMinesweeperHighScore(const FString& InName, const int32 InScore, const float InTime, const int32 InClicks = 0)
		: Name(InName), Score(InScore), Time(InTime), Clicks(InClicks) { }

};




/**
 * Developer settings class for the Minesweeper plugin.
 */
UCLASS(Config = "Minesweeper")
class MINESWEEPEREDITOR_API UMinesweeperSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()
	
public:
	//~ Begin UObject Overrides
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	//~ End of UObject Overrides


	//~ Begin UDeveloperSettings Overrides
	virtual bool SupportsAutoRegistration() const override { return true; }
	virtual FName GetContainerName() const override { return FName("Editor"); }
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	virtual FName GetSectionName() const override { return FName("Minesweeeper"); }
	virtual FText GetSectionText() const override { return FMinesweeperEditorModule::GetMinesweeperLabel(); }
	virtual FText GetSectionDescription() const override { return NSLOCTEXT("Minesweeper", "MinesweeperSectionDescription", "Settings for the Minesweeper plugin."); }
	//~ End UDeveloperSettings Overrides


	void ResetToDefaults();


	static UMinesweeperSettings* Get() { return GetMutableDefault<UMinesweeperSettings>(); }
	static const UMinesweeperSettings* GetConst() { return GetDefault<UMinesweeperSettings>(); }


	/** Event triggered when any setting is changed. */
	FOnFinishedChangingProperties OnSettingsChanged;


	/// ************************************************** SETTINGS **************************************************
	///
	/** True if using dock tab window mode. False if using standalone window mode. */
	UPROPERTY(Config) bool UseDockTab;

	/** The current saved name that was last entered for the game. */
	UPROPERTY(Config) FString LastPlayerName;

	/** The current saved settings that were last entered for the game. */
	FMinesweeperDifficulty LastSettings;

	/** List of the last ten high scores. */
	UPROPERTY(Config) TArray<FMinesweeperHighScore> HighScores;

};

