// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "MinesweeperSettings.h"

class SWidgetSwitcher;
class SEditableTextBox;
class SVerticalBox;
class SUniformGridPanel;
class SMinesweeperCell;
struct FMinesweeperDifficulty;




/**
 * Slate Minesweeper game for the Unreal Engine editor.
 * 
 * TODO:
 * - implement algo to eliminate the chance of a player having to guess
 * - fix switch window mode button stops working after SDockTab window is docked
 */
class MINESWEEPEREDITOR_API SMinesweeper : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeper) { }
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);


	/** The actual size of each cell on the screen in pixels. */
	static const int32 CellSize = 28;

	/** The default difficulty settings when the game window is first shown. */
	static const FMinesweeperDifficulty DefaultDifficulty;

	static const int32 MinGridSize = 2;
	static const int32 MaxGridSize = 30;

	static const int32 MinMineCount = 1;
	int32 MaxMineCount = 225;

	/** Max score used in high score calculation. */
	static const int32 MaxScore = 1000000;


private:
	/** Settings saved to disk. */
	UMinesweeperSettings* Settings = nullptr;

	/** The active "tick" timer. */
	TSharedPtr<FActiveTimerHandle> ActiveTimerHandle;


	/** The random seed used to generate the current grid mine layout. */
	int32 CurrentGridSeed = 0;

	/** The current game difficulty settings. Holds the main grid size and mine count. */
	FMinesweeperDifficulty CurrentSettings;

	/** True if the game is active and running. Game is not active until the user clicks the first cell. */
	bool IsGameActive = false;

	/** True if the game is paused and the user is setting up a game with new settings. */
	bool IsGamePaused = false;

	/** The time in seconds the current game has been running. Doesn't start until the game is active. */
	float GameTime = 0.0f;

	/** The total user left and right mouse clicks for the current game. Used for scoring. */
	int32 TotalClicks = 0;

	/** The number of flags left that the user can place. */
	int32 FlagsRemaining = 0;

	/** The number of cells the user has left to open. */
	int32 NumClosedCells = 0;

	/** The number of cells the user has opened. */
	int32 NumOpenedCells = 0;

	/** Holds the last achieved high score ranking. */
	int8 LastHighScoreRank = -1;


	/** The currently active main panel. This should always be 0 or 1. 0 = Game Setup Panel, 1 = Minesweeper Grid Game Panel */
	int32 ActiveMainPanel = 0;

	/** The currently active game setup panel. This should always be 0 or 1. 0 = Game Settings Panel, 1 = High Scores Panel */
	int32 ActiveGameSetupPanel = 0;


	/** Text box widget for the player name. */
	TSharedPtr<SEditableTextBox> NameTextBox;

	/** High scores widget. */
	TSharedPtr<SVerticalBox> HighScoresList;

	/** Game grid widget. */
	TSharedPtr<SUniformGridPanel> GridPanel;


	/** Indexed lookup map for quick grid cell widget access. */
	TMap<int32, TSharedRef<SMinesweeperCell>> CellMap;


	/** Refreshes the high scores list widget. */
	void RefreshHighScores();


	/** The games update timer "tick". */
	EActiveTimerReturnType UpdateGameTick(double InCurrentTime, float InDeltaTime);


	void OnPlayerNameChanged(const FText& NewText);
	void OnPlayerNameCommitted(const FText& NewText, ETextCommit::Type InTextCommit);


	FSlateColor GetDifficultyColor(const int32 InDifficultyLevel) const;
	FText GetTimerText() const;
	const FSlateBrush* GetSmileImage() const;
	FText GetFlagsRemainingText() const;
	EVisibility GetWinLoseVisibility() const;
	FSlateColor GetWinLoseColor() const;
	FText GetWinLoseText() const;
	EVisibility GetHighScoreRankVisibility() const;
	FText GetHighScoreRankText() const;


	/** Button click handlers. */
	FReply OnDifficultyClick(const int32 InDifficultyLevel);
	FReply OnStartNewGameClick();
	FReply OnContinueGameClick();
	FReply OnNewGameClick();
	FReply OnRestartGameClick();
	FReply OnSwitchWindowModeClick();
	FReply OnGotoNewGamePanel();


	/** Callback for when a cell is left clicked by the user. */
	void OnLeftClickCell(TSharedPtr<SMinesweeperCell> InCellWidget);

	/** Callback for when a cell is right clicked by the user. */
	void OnRightClickCell(TSharedPtr<SMinesweeperCell> InCellWidget);


	#pragma region Grid / Cell Functions

	inline bool HasWon() const { return NumClosedCells == CurrentSettings.MineCount; }

	inline int32 TotalCellCount() const { return CurrentSettings.Width * CurrentSettings.Height; }

	bool IsValidGridIndex(const int32 InCellIndex) const;
	bool IsValidGridCoord(const FIntVector2 InCellCoord) const;
	int32 GridCoordToIndex(const FIntVector2 InCellCoord) const;
	FIntVector2 GridIndexToCoord(const int32 InCellIndex) const;

	TSharedPtr<SMinesweeperCell> GetCellWidget(const FIntVector2 InCellCoord);
	TArray<TSharedPtr<SMinesweeperCell>> GetNeighborCellWidgets(const FIntVector2 InCellCoord);


	/** Sets a cell's state to open. */
	void OpenCell(TSharedPtr<SMinesweeperCell> InCellWidget);

	/** Opens a cell's neighbor cells based on the number of mines surrounding the cells. */
	void OpenNeighbors(TSharedPtr<SMinesweeperCell> InCellWidget);


	/** Perform a function on every cell of the grid. */
	void ForEachCell(TFunctionRef<void(TSharedPtr<SMinesweeperCell>)> InFunc);

	#pragma endregion

};

