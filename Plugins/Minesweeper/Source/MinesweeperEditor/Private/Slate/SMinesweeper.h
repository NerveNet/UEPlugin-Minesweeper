// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "MinesweeperGridCanvas.h"
#include "MinesweeperSettings.h"

class SWidgetSwitcher;
class SEditableTextBox;
class SVerticalBox;
class SUniformGridPanel;
class UMinesweeperGame;
class SMinesweeperGrid;
struct FMinesweeperDifficulty;




DECLARE_DELEGATE_RetVal_ThreeParams(int32, FMinesweeperGameOverHighScoreDelegate, const bool, const float, const int32);


/**
 * Slate Minesweeper game UI visualtion for MinesweeperGame class.
 */
class SMinesweeper : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeper)
		: _CellDrawSize(28)
	{ }

		SLATE_ARGUMENT(int32, CellDrawSize)

		SLATE_ATTRIBUTE(FText, PlayerName)

		SLATE_EVENT(FSimpleDelegate, OnGameSetupClick)

		SLATE_EVENT(FMinesweeperGameOverHighScoreDelegate, OnGameOver)

	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);


	static const int32 MinGridSize = 2;
	static const int32 MaxGridSize = 30;

	static const int32 MinMineCount = 1;
	int32 MaxMineCount = 225;

	/** Max score used in high score calculation. */
	static const int32 MaxScore = 1000000;

	
	inline UMinesweeperGame* GetGame() const { return Game.Get(); }


	bool IsGameActive() const;
	void StartNewGame(const FMinesweeperDifficulty& InDifficulty);
	void RestartGame();
	void PauseGame();
	void ContinueGame();


private:
	TStrongObjectPtr<UMinesweeperGame> Game;


	FSimpleDelegate OnGameSetupClick;
	FMinesweeperGameOverHighScoreDelegate OnGameOver;


	/** The active "tick" timer handle. We are using a timer instead of the tick event. */
	TSharedPtr<FActiveTimerHandle> ActiveTimerHandle;


	/** Holds the last achieved high score ranking. */
	int8 LastHighScoreRank = -1;


	TSharedPtr<SMinesweeperGrid> CellGrid;


	/** The games update timer "tick". */
	EActiveTimerReturnType UpdateGameTick(double InCurrentTime, float InDeltaTime);


	FText GetTimerText() const;
	const FSlateBrush* GetSmileImage() const;
	FText GetFlagsRemainingText() const;
	EVisibility GetWinLoseVisibility() const;
	FSlateColor GetWinLoseColor() const;
	FText GetWinLoseText() const;
	EVisibility GetHighScoreRankVisibility() const;
	FText GetHighScoreRankText() const;


	FReply OnRestartGameButtonClick();
	FReply OnGameSetupButtonClick();

};

