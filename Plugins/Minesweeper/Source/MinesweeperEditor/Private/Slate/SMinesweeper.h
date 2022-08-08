// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UMinesweeperGame;
class UMinesweeperGridCanvas;
class SMinesweeperGrid;
struct FMinesweeperDifficulty;




DECLARE_DELEGATE_RetVal_ThreeParams(int32, FMinesweeperGameOverHighScoreDelegate, const bool, const float, const int32);


/**
 * Slate Minesweeper game UI visualtion for UMinesweeperGame class.
 */
class SMinesweeper : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeper)
		: _CellDrawSize(28.0f)
	{ }

		SLATE_ARGUMENT(float, CellDrawSize)

		SLATE_ATTRIBUTE(FText, PlayerName)

		SLATE_EVENT(FSimpleDelegate, OnGameSetupClick)

		SLATE_EVENT(FMinesweeperGameOverHighScoreDelegate, OnGameOver)

	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);

	
	inline UMinesweeperGame* GetGame() const { return Game.Get(); }


	bool IsGameActive() const;
	void StartNewGame(const FMinesweeperDifficulty& InDifficulty);
	void RestartGame();
	void PauseGame();
	void ContinueGame();


	void SetGridSize(const FIntVector2& InGridSize, const float InNewCellDrawSize = -1.0f);

	float GetCellDrawSize() const;
	void SetCellDrawSize(const float InCellDrawSize);


private:
	/** Minesweeper game logic object. */
	TStrongObjectPtr<UMinesweeperGame> Game;
	/** Render target texture where the cell textures are drawn for each cell. */
	TStrongObjectPtr<UMinesweeperGridCanvas> GridCanvas;
	FSlateBrush GridCanvasBrush;


	FSimpleDelegate OnGameSetupClick;
	FMinesweeperGameOverHighScoreDelegate OnGameOver;

	int8 LastHighScoreRank = -1;


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


	void OnCellLeftClick(const FVector2D& InGridPosition);
	void OnCellRightClick(const FVector2D& InGridPosition);
	void OnHoverCellChanged(const bool InIsHovered, const FVector2D& InGridPosition);

};

