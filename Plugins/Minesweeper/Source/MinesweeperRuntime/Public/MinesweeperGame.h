// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MinesweeperDifficulty.h"
#include "MinesweeperGame.generated.h"




/**
 * Data representation for a single grid cell.
 */
USTRUCT()
struct MINESWEEPERRUNTIME_API FMinesweeperCell
{
	GENERATED_USTRUCT_BODY()

	/** Holds the number of mines that surround this cell. */
	UPROPERTY() int32 NeighborMineCount;

	/** True if this cell contains a mine. */
	UPROPERTY() uint8 bHasMine : 1;

	/** True if this cell has been left clicked and is open. */
	UPROPERTY() uint8 bIsOpened : 1;

	/** True if the user has marked this cell with a flag. */
	UPROPERTY() uint8 bIsFlagged : 1;

	FMinesweeperCell() 
		: NeighborMineCount(-1), bHasMine(false), bIsOpened(false), bIsFlagged(false)
	{ }

	void Reset()
	{
		NeighborMineCount = -1;
		bHasMine = false;
		bIsOpened = false;
		bIsFlagged = false;
	}
};




DECLARE_MULTICAST_DELEGATE_ThreeParams(FMinesweeperGameOverDelegated, const bool, const float, const int32);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMinesweeperGameOverDelegate, const bool, Won, const float, Time, const int32, Clicks);
//DECLARE_DYNAMIC_DELEGATE_ThreeParams(FMinesweeperGameOverDelegatedd, const bool, Won, const float, Time, const int32, Clicks);


/**
 * Minesweeper game logic.
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class MINESWEEPERRUNTIME_API UMinesweeperGame : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	static const int32 MinGridSize = 2;
	static const int32 MaxGridSize = 30; // 900 cells max

	static const int32 MinMineCount = 1;
	static const int32 MaxMineCount = 400;


	/**  */
	UFUNCTION(BlueprintCallable, Category = "Minesweeper")
		void SetupGame(const FMinesweeperDifficulty& InDifficulty);

	/** Resets game timer and all grid cells. */
	UFUNCTION(BlueprintCallable, Category = "Minesweeper")
		void RestartGame();

	/** Returns false if the cell coordinate is invalid or the cell is open. */
	UFUNCTION(BlueprintCallable, Category = "Minesweeper")
		bool TryOpenCell(const int32 CellX, const int32 CellY);

	/** Returns false if the cell coordinate is invalid or the cell is open. */
	UFUNCTION(BlueprintCallable, Category = "Minesweeper")
		bool TryFlagCall(const int32 CellX, const int32 CellY);


	/**  */
	UPROPERTY(BlueprintAssignable, Category = "Minesweeper")
		FMinesweeperGameOverDelegate OnGameOver;
	/**  */
	//UPROPERTY(BlueprintAssignable, Category = "Minesweeper")
		//FMinesweeperGameOverDelegatedd OnGameOveredd;
	FMinesweeperGameOverDelegated OnGameOvered;


	inline const FMinesweeperDifficulty& GetDifficulty() const { return Difficulty; }

	inline bool IsGameActive() const { return IsActive; }
	inline bool IsGamePaused() const { return IsPaused; }

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Minesweeper")
		FORCEINLINE void PauseGame() { if (IsActive) { IsPaused = true; } }

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Minesweeper")
		FORCEINLINE void ResumeGame() { IsPaused = false; }

	/**  */
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		FORCEINLINE float GetGameTime() const { return GameTime; }

	inline bool IsGameActiveAndRunning() const { return IsActive && !IsPaused; }
	inline bool HasGameStarted() const { return IsActive && GameTime > 0.0f; }
	inline bool IsGameOver() const { return !IsActive && GameTime > 0.0f; }
	inline bool HasWon() const { return GameTime > 0.0f && NumClosedCells == Difficulty.MineCount; }

	inline int32 TotalCellCount() const { return Difficulty.TotalCells(); }

	inline int32 GetFlagsRemaining() const { return FlagsRemaining; }


protected:
	//~ Begin FTickableGameObject Interface
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UMinesweeperGame, STATGROUP_Tickables); }
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Conditional; }
	virtual bool IsTickableWhenPaused() const override { return true; }
	virtual bool IsTickableInEditor() const override { return true; }
	virtual bool IsTickable() const override;
	virtual void Tick(float InDeltaTime) override;
	//~ End FTickableGameObject Interface


	int32 GridRandomSeed = 0;
	FMinesweeperDifficulty Difficulty;

	TMap<int32, TSharedRef<FMinesweeperCell>> CellMap;

	bool IsActive = false;
	bool IsPaused = false;
	float GameTime = 0.0f;

	int32 FlagsRemaining = 0;
	int32 NumClosedCells = 0;
	int32 NumOpenedCells = 0;

	int32 TotalClicks = 0;
	int8 LastHighScoreRank = -1;


public:
	bool IsValidGridIndex(const int32 InCellIndex) const;
	bool IsValidGridCoord(const FIntVector2& InCellCoord) const;
	int32 GridCoordToIndex(const FIntVector2& InCellCoord) const;
	FIntVector2 GridIndexToCoord(const int32 InCellIndex) const;

	TSharedPtr<FMinesweeperCell> GetCell(const int32 InCellIndex);
	TArray<TSharedRef<FMinesweeperCell>> GetNeighborCells(const int32 InCellIndex);

private:
	void OpenCell(TSharedRef<FMinesweeperCell> InCell, const int32 InCellIndex);
	void OpenNeighbors(TSharedRef<FMinesweeperCell> InCell, const int32 InCellIndex);

public:
	void ForEachCell(TFunctionRef<void(TSharedRef<FMinesweeperCell> InCell, const int32 InCellIndex, const FVector2D InCellCoord)> InFunc);

};
