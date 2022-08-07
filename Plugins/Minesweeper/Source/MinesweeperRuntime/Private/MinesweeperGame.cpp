// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperGame.h"




void UMinesweeperGame::SetupGame(const FMinesweeperDifficulty& InDifficulty)
{
	Difficulty = InDifficulty;

	const int32 totalCellCount = TotalCellCount();
	
	CellMap.Empty(totalCellCount);

	for (int32 cellIndex = 0; cellIndex < totalCellCount; ++cellIndex)
	{
		CellMap.Add(cellIndex, MakeShareable(new FMinesweeperCell));
	}
}

void UMinesweeperGame::RestartGame()
{
	IsActive = false;
	GameTime = 0.0f;

	ForEachCell([](TSharedRef<FMinesweeperCell> InCell, const int32 InCellIndex, const FVector2D InCellCoord)
		{
			InCell->Reset();
		});
}


bool UMinesweeperGame::TryOpenCell(const int32 CellX, const int32 CellY)
{
	const int32 cellIndex = GridCoordToIndex(FIntVector2(CellX, CellY));
	if (!CellMap.Contains(cellIndex)) return false;

	TSharedRef<FMinesweeperCell> openCell = CellMap[cellIndex];


	if (IsActive && GameTime > 0.0f) // game is active and started
	{
		++TotalClicks; // clicks always count towards score

		if (openCell->bIsOpened || openCell->bIsFlagged) return false;

		OpenCell(openCell, cellIndex);

		if (openCell->bHasMine)
		{
			// the game has ended in a loser!
			IsActive = false;

			LastHighScoreRank = -1;

			OnGameOver.Broadcast(false, GameTime, TotalClicks);
			OnGameOvered.Broadcast(false, GameTime, TotalClicks);

			// show all mines
			/*ForEachCell([](TSharedRef<FMinesweeperCell> InCell, const int32 InCellIndex, const FVector2D InCellCoord)
				{
					InCell->ShowMine = true;
				});*/
		}
		else if (HasWon()) // check for win condition
		{
			// the game has ended in a winner!
			IsActive = false;

			OnGameOver.Broadcast(true, GameTime, TotalClicks);
			OnGameOvered.Broadcast(true, GameTime, TotalClicks);
		}
	}
	else if (!IsActive && GameTime == 0.0f) // game is NOT active and has NOT started
	{
		// start of a new game
		IsActive = true;
		TotalClicks = 1;
		FlagsRemaining = Difficulty.MineCount;
		NumClosedCells = Difficulty.TotalCells();
		NumOpenedCells = 0;

		FRandomStream randStream(GridRandomSeed);

		// calculate placement of mines after user clicks to avoid the user ever clicking a mine on the first click
		const int32 totalCellCount = NumClosedCells;
		int32 minesToPlace = FlagsRemaining;
		while (minesToPlace > 0)
		{
			int32 randCellIndex = randStream.RandRange(0, totalCellCount - 1);

			TSharedPtr<FMinesweeperCell> cell = GetCell(randCellIndex);
			if (!cell.IsValid()) break;

			if (cell != openCell && !cell->bHasMine)
			{
				cell->bHasMine = true;
				--minesToPlace;
			}
		}

		// calculate neighboring mine counts for each cell
		int32 currentNeighborMineCount = 0;
		ForEachCell([&](TSharedRef<FMinesweeperCell> InCell, const int32 InCellIndex, const FVector2D InCellCoord)
			{
				TArray<TSharedRef<FMinesweeperCell>> neighborCells = GetNeighborCells(InCellIndex);

				currentNeighborMineCount = 0;
				for (uint32 i = 0; i < (uint32)neighborCells.Num(); ++i)
				{
					if (neighborCells[i]->bHasMine) ++currentNeighborMineCount;
				}

				TSharedPtr<FMinesweeperCell> cell = GetCell(InCellIndex);
				if (cell.IsValid())
				{
					cell->NeighborMineCount = currentNeighborMineCount;
				}
			});

		OpenCell(openCell, cellIndex);
	}

	return true;
}


bool UMinesweeperGame::TryFlagCall(const int32 CellX, const int32 CellY)
{
	const int32 cellIndex = GridCoordToIndex(FIntVector2(CellX, CellY));
	if (!CellMap.Contains(cellIndex)) return false;

	TSharedRef<FMinesweeperCell> clickCell = CellMap[cellIndex];

	++TotalClicks; // clicks always count towards score

	if (clickCell->bIsOpened) return false;


	clickCell->bIsFlagged = ~clickCell->bIsFlagged;

	if (clickCell->bIsFlagged)
	{
		if (FlagsRemaining > 0)
		{
			--FlagsRemaining;

			// disable flagging for all cells if they have used them all
			/*if (FlagsRemaining == 0)
			{
				ForEachCell([&](TSharedRef<FMinesweeperCell> InCell, const int32 InCellIndex, const FVector2D InCellCoord)
					{
						clickCell->CanPlaceFlag = false;
					});
			}*/
		}
	}
	else
	{
		// enable flagging for all cells
		/*if (FlagsRemaining == 0)
		{
			ForEachCell([&](TSharedPtr<FMinesweeperCell> InCell, const int32 InCellIndex, const FVector2D InCellCoord)
				{
					clickCell->CanPlaceFlag = true;
				});
		}*/

		++FlagsRemaining;
	}

	return true;
}


bool UMinesweeperGame::IsValidGridIndex(const int32 InCellIndex) const
{
	return InCellIndex >= 0 && InCellIndex < TotalCellCount();
}

bool UMinesweeperGame::IsValidGridCoord(const FIntVector2& InCellCoord) const
{
	return InCellCoord.X >= 0 && InCellCoord.Y >= 0 && InCellCoord.X < Difficulty.Width && InCellCoord.Y < Difficulty.Height;
}

int32 UMinesweeperGame::GridCoordToIndex(const FIntVector2& InCellCoord) const
{
	return (Difficulty.Width * InCellCoord.Y) + InCellCoord.X;
}

FIntVector2 UMinesweeperGame::GridIndexToCoord(const int32 InCellIndex) const
{
	return FIntVector2(FMath::FloorToInt32((float)(InCellIndex % Difficulty.Width)), FMath::FloorToInt32((float)InCellIndex / (float)Difficulty.Width));
}


TSharedPtr<FMinesweeperCell> UMinesweeperGame::GetCell(const int32 InCellIndex)
{
	if (!IsValidGridIndex(InCellIndex)) return nullptr;
	if (CellMap.Contains(InCellIndex))
	{
		return CellMap[InCellIndex];
	}
	return nullptr;
}

TArray<TSharedRef<FMinesweeperCell>> UMinesweeperGame::GetNeighborCells(const int32 InCellIndex)
{
	TArray<TSharedRef<FMinesweeperCell>> outCellWidgets;

	if (!IsValidGridIndex(InCellIndex)) return outCellWidgets;

	const FIntVector2 cellCoord = GridIndexToCoord(InCellIndex);

	// 2d neighboring tile offsets mapped to array indices
	// 0 - 1 - 2
	// 3 - 4 - 5
	// 6 - 7 - 8

	const TArray<FIntVector2> Offsets = {
		FIntVector2(-1,-1), FIntVector2(0,-1), FIntVector2(1,-1),
		FIntVector2(-1, 0), FIntVector2(0, 0), FIntVector2(1, 0),
		FIntVector2(-1, 1), FIntVector2(0, 1), FIntVector2(1, 1) };

	for (int32 i = 0; i < 9; ++i)
	{
		if (i == 4) continue; // skip middle cell (self)

		int32 cellIndex = GridCoordToIndex(FIntVector2(cellCoord.X + Offsets[i].X, cellCoord.Y + Offsets[i].Y));

		// all cells that are outside of the grid bounds will return null here and not be added
		TSharedPtr<FMinesweeperCell> cell = GetCell(cellIndex);
		if (cell.IsValid())
		{
			outCellWidgets.Add(cell.ToSharedRef());
		}
	}

	return outCellWidgets;
}

void UMinesweeperGame::OpenCell(TSharedRef<FMinesweeperCell> InCell, const int32 InCellIndex)
{
	InCell->bIsOpened = true;
	--NumClosedCells;
	++NumOpenedCells;

	if (InCell->NeighborMineCount == 0)
	{
		OpenNeighbors(InCell, InCellIndex);
	}
}

void UMinesweeperGame::OpenNeighbors(TSharedRef<FMinesweeperCell> InCell, const int32 InCellIndex)
{
	TArray<TSharedRef<FMinesweeperCell>> neighborCells = GetNeighborCells(InCellIndex);
	for (auto cell : neighborCells)
	{
		if (!cell->bIsOpened)
		{
			OpenCell(cell, InCellIndex);
		}
	}
}

void UMinesweeperGame::ForEachCell(TFunctionRef<void(TSharedRef<FMinesweeperCell> InCell, const int32 InCellIndex, const FVector2D InCellCoord)> InFunc)
{
	for (int32 cellIndex = 0; cellIndex < TotalCellCount(); ++cellIndex)
	{
		TSharedPtr<FMinesweeperCell> cell = GetCell(cellIndex);
		if (cell.IsValid())
		{
			InFunc(cell.ToSharedRef(), cellIndex, FVector2D(cellIndex % Difficulty.Width, cellIndex / Difficulty.Width));
		}
	}
}


bool UMinesweeperGame::IsTickable() const
{
	return IsActive && !IsPaused;
}

void UMinesweeperGame::Tick(float InDeltaTime)
{
	if (IsActive && !IsPaused)
	{
		GameTime += InDeltaTime;
	}
}

