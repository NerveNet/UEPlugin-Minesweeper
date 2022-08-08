// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "MinesweeperGridCanvas.h"
#include "MinesweeperSettings.h"

class SWidgetSwitcher;
class SEditableTextBox;
class SMinesweeper;
class SMinesweeperHighScores;
struct FMinesweeperDifficulty;




/**
 * Minesweeper game parent window that handles game setup.
 */
class SMinesweeperWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperWindow) { }
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);


	/** The default difficulty settings when the game window is first shown. */
	static const FMinesweeperDifficulty DefaultDifficulty;

	int32 MaxMineCount = 225;

	/** Max score used in high score calculation. */
	static const int32 MaxScore = 1000000;


private:
	/** Settings saved to disk. */
	UMinesweeperSettings* Settings = nullptr;


	FTimerHandle CellDrawResizeTimerHandle;


	int32 TitleTextAnimIndex = 20;


	/** Holds the last achieved high score ranking. */
	int8 LastHighScoreRank = -1;


	/** The currently active main panel. This should always be 0 or 1. 0 = Game Setup Panel, 1 = Minesweeper Grid Game Panel */
	int32 ActiveMainPanel = 0;

	/** The currently active game setup panel. This should always be 0 or 1. 0 = Game Settings Panel, 1 = High Scores Panel */
	int32 ActiveGameSetupPanel = 0;


	TSharedPtr<SEditableTextBox> NameTextBox;
	TSharedPtr<SMinesweeperHighScores> HighScoresList;
	TSharedPtr<SMinesweeper> GameWidget;


	void OnCellDrawSizeChanged(const FPropertyChangedEvent& InPropertyChangedEvent);


	FText GetPlayerName() const;
	void OnPlayerNameChanged(const FText& NewText);
	void OnPlayerNameCommitted(const FText& NewText, ETextCommit::Type InTextCommit);


	FText GetTitleText() const;
	FSlateColor GetDifficultyButtonColor(const int32 InDifficultyLevel) const;


	/** Button click handlers. */
	FReply OnDifficultyClick(const int32 InDifficultyLevel);
	FReply OnStartNewGameClick();
	FReply OnContinueGameClick();
	FReply OnNewGameClick();
	FReply OnRestartGameClick();
	FReply OnSwitchWindowModeClick();
	FReply OnGotoNewGamePanel();
	void GotoNewGamePanel();
	FReply OnSettingsClick();


	int32 OnGameOverCallback(const bool InWon, const float InTime, const int32 InClicks);

};

