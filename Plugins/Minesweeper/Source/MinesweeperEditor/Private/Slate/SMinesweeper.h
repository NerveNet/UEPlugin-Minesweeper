// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "MinesweeperStatics.h"
#include "Widgets/SCompoundWidget.h"

class SHorizontalBox;
class SVerticalBox;
class UMinesweeperGame;
class SMinesweeperGrid;
struct FMinesweeperDifficulty;




DECLARE_DELEGATE_RetVal_ThreeParams(int32, FMinesweeperGameOverHighScoreDelegate, const bool, const float, const int32);


/**
 * Slate Minesweeper game UI visualtion for UMinesweeperGame class.
 */
class MINESWEEPEREDITOR_API SMinesweeper : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeper)
	{ }

		SLATE_ATTRIBUTE(FText, PlayerName)

		SLATE_EVENT(FSimpleDelegate, OnGameSetupClick)

		SLATE_EVENT(FMinesweeperGameOverHighScoreDelegate, OnGameOver)

	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);
private:
	TSharedRef<SHorizontalBox> ConstructHeaderRow(TAttribute<FText> InPlayerName);
	TSharedRef<SVerticalBox> ConstructPlayerNameAndGameTimePanel(TAttribute<FText> InPlayerName);
	TSharedRef<SVerticalBox> ConstructFlagsRemainingPanel();
	TSharedRef<SVerticalBox> ConstructEndGameOverlayPanel();

	
public:
	inline UMinesweeperGame* GetGame() const { return Game.Get(); }


	bool IsGameActive() const;
	void StartNewGame(const FMinesweeperDifficulty& InDifficulty);
	void RestartGame();
	void PauseGame();
	void ContinueGame();


private:
	/** Minesweeper game logic object. */
	TStrongObjectPtr<UMinesweeperGame> Game;

	TSharedPtr<SMinesweeperGrid> GridWidget;


	FSimpleDelegate OnGameSetupClick;
	FMinesweeperGameOverHighScoreDelegate OnGameOver;

	int8 LastHighScoreRank = -1;


	FTimerHandle VisualThemeChangeTimerHandle;


	void OnVisualThemeChanged(const FPropertyChangedEvent& InPropertyChangedEvent);


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


	void OnCellLeftClick(const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition);
	void OnCellRightClick(const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition);
	void OnHoverCellChange(const bool InIsHovered, const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition);

};

