// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "MinesweeperSettings.h"

class SWidgetSwitcher;
class SEditableTextBox;
class SUniformGridPanel;
class SMinesweeperCell;
struct FMinesweeperDifficulty;




/**
 * Slate Minesweeper game for the Unreal Engine editor.
 * 
 * TODO:
 * - implement algo to eliminate the chance of a player having to guess
 * 
 */
class MINESWEEPEREDITOR_API SMinesweeper : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeper) { }
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);


	/** The actual size of each cell on the screen in pixels. */
	static const int32 CellSize = 28;

	static const FMinesweeperDifficulty DefaultDifficulty;

	static const int32 MinGridSize = 2;
	static const int32 MaxGridSize = 48;

	static const int32 MinMineCount = 1;
	int32 MaxMineCount = 576;

	static const int32 MaxScore = 1000000;


private:
	//friend class SMinesweeperCell;

	/** Holds the singleton instance of this class so all cells can acess without needing each cell to hold the data. */
	//static TSharedPtr<SMinesweeper> Instance;


	UMinesweeperSettings* Settings = nullptr;

	TSharedPtr<FActiveTimerHandle> ActiveTimerHandle;

	int32 CurrentGridSeed = 0;
	FMinesweeperDifficulty CurrentSettings;

	bool IsGameActive = false;
	bool IsGamePaused = false;

	/** The total time in seconds the current game has been running. */
	float GameTime = 0.0f;

	int32 TotalClicks = 0;
	int32 FlagsRemaining = 0;
	int32 NumClosedCells = 0;
	int32 NumOpenedCells = 0;

	int8 LastHighScoreRank = -1;


	int32 ActiveMainPanel = 0;
	int32 ActiveGameSetupPanel = 0;

	TSharedPtr<SEditableTextBox> NameTextBox;
	TSharedPtr<SUniformGridPanel> GridPanel;

	TMap<int32, TSharedRef<SMinesweeperCell>> CellMap;


	EActiveTimerReturnType UpdateGameTick(double InCurrentTime, float InDeltaTime);


	void HandleNameChanged(const FText& NewText);
	void HandleNameCommitted(const FText& NewText, ETextCommit::Type InTextCommit);


	FReply OnDifficultyClick(const int32 InDifficultyLevel);
	FReply OnStartNewGameClick();
	FReply OnContinueGameClick();
	FReply OnNewGameClick();
	FReply OnSwitchWindowModeClick();
	FReply OnGotoNewGamePanel();


	FSlateColor GetDifficultyColor(const int32 InDifficultyLevel) const;
	FText GetTimerText() const;
	const FSlateBrush* GetSmileImage() const;
	FText GetFlagsRemainingText() const;
	EVisibility GetWinLoseVisibility() const;
	FSlateColor GetWinLoseColor() const;
	FText GetWinLoseText() const;


	void OnLeftClickCell(TSharedPtr<SMinesweeperCell> InCellWidget);
	void OnRightClickCell(TSharedPtr<SMinesweeperCell> InCellWidget);


	#pragma region Grid / Cell Functions

	inline int32 TotalCellCount() const { return CurrentSettings.Width * CurrentSettings.Height; }

	bool IsValidGridIndex(const int32 InCellIndex) const;
	bool IsValidGridCoord(const FIntVector2 InCellCoord) const;
	int32 GridCoordToIndex(const FIntVector2 InCellCoord) const;
	FIntVector2 GridIndexToCoord(const int32 InCellIndex) const;

	TSharedPtr<SMinesweeperCell> GetCellWidget(const FIntVector2 InCellCoord);
	TArray<TSharedPtr<SMinesweeperCell>> GetNeighborCellWidgets(const FIntVector2 InCellCoord);

	void OpenCell(TSharedPtr<SMinesweeperCell> InCellWidget);
	void OpenNeighbors(TSharedPtr<SMinesweeperCell> InCellWidget);

	#pragma endregion

};

