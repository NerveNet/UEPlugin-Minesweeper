// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MinesweeperEditorModule.h"
#include "MinesweeperDifficulty.h"
#include "MinesweeperSettings.generated.h"




/**
 * Data representation for a single user high score.
 */
USTRUCT()
struct FMinesweeperHighScore
{
	GENERATED_USTRUCT_BODY()

	/** Name of the user who achieved the high score. */
	UPROPERTY(Config, EditAnywhere, Category = "HighScore")
		FString Name = "";

	/** The calculated high score. */
	UPROPERTY(Config, EditAnywhere, Category = "HighScore")
		int32 Score = 0;

	/** The time length in seconds the user used to achieve the high score. */
	UPROPERTY(Config, EditAnywhere, Category = "HighScore")
		float Time = 0;

	/** Total number of left and right mouse button clicks the user used to achieve the high score. */
	UPROPERTY(Config, EditAnywhere, Category = "HighScore")
		int32 Clicks = 0;

	FMinesweeperHighScore() { }
	FMinesweeperHighScore(const FString& InName, const int32 InScore, const float InTime, const int32 InClicks = 0)
		: Name(InName), Score(InScore), Time(InTime), Clicks(InClicks) { }
};




/**
 * Developer settings class for the Minesweeper plugin.
 */
UCLASS(Config="Minesweeper", DefaultConfig)
class UMinesweeperSettings : public UDeveloperSettings
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
	UPROPERTY(Config, EditAnywhere, Category = "General", Meta = (ConfigRestartRequired = true, 
			DisplayName = "Show Toolbar Button",
			Tooltip = "Show the Minesweeper toolbar button."))
		bool ShowToolbarButton;

	UPROPERTY(Config, EditAnywhere, Category = "General", Meta = (
			DisplayName = "Use Docking Tab",
			Tooltip = "Switch between standalone and docking tab window modes."))
		bool UseDockTab;

	UPROPERTY(Config, EditAnywhere, Category = "General", Meta = (
			DisplayName = "Player Name",
			Tooltip = "The current saved name that was last entered for the game."))
		FString LastPlayerName;

	UPROPERTY(Config, EditAnywhere, Category = "General", Meta = (
			DisplayName = "Difficulty Settings",
			Tooltip = "The current saved difficulty settings that were last entered for the game."))
		FMinesweeperDifficulty LastDifficulty;


	UPROPERTY(Config, EditAnywhere, AdvancedDisplay, Category = "General", Meta = (UIMin = 10.0, ClampMin = 10.0, UIMax = 40.0, ClampMax = 40.0,
			DisplayName = "Cell Draw Size",
			Tooltip = "The actual size of each cell on the screen in pixels."))
		float CellDrawSize;


	UPROPERTY(Config/*, EditAnywhere, Category = "General"*/) TArray<FMinesweeperHighScore> HighScores;

};

