// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "MinesweeperDifficulty.generated.h"




/**
 * Minesweeper Difficulty
 */
USTRUCT(BlueprintType)
struct MINESWEEPERRUNTIME_API FMinesweeperDifficulty
{
	GENERATED_USTRUCT_BODY()

	/** Width of the Minesweeper grid in cells. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperDifficulty", Meta = (UIMin = 1, ClampMin = 1, UIMax = 64, ClampMax = 64))
		int32 Width = 1;

	/** Height of the Minesweeper grid in cells. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperDifficulty", Meta = (UIMin = 1, ClampMin = 1, UIMax = 64, ClampMax = 64))
		int32 Height = 1;

	/** Mine count of the Minesweeper grid. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperDifficulty", Meta = (UIMin = 1, ClampMin = 1, UIMax = 500, ClampMax = 500))
		int32 MineCount = 1;


	FMinesweeperDifficulty() { }
	FMinesweeperDifficulty(const int32 InWidth, const int32 InHeight, const int32 InMineCount)
		: Width(InWidth), Height(InHeight), MineCount(InMineCount) { }


	bool operator == (const FMinesweeperDifficulty& InOther) const
	{
		return (Width == InOther.Width && Height == InOther.Height && MineCount == InOther.MineCount);
	}

	bool operator != (const FMinesweeperDifficulty& InOther) const
	{
		return !(*this == InOther);
	}

	void operator = (const FMinesweeperDifficulty& InOther)
	{
		Width = InOther.Width;
		Height = InOther.Height;
		MineCount = InOther.MineCount;
	}


	FIntVector2 GridSize() const { return FIntVector2(Width, Height); }
	int32 TotalCells() const { return Width * Height; }

	bool IsBeginner() const { return *this == Beginner(); }
	bool IsIntermediate() const { return *this == Intermediate(); }
	bool IsExpert() const { return *this == Expert(); }

	
	static FMinesweeperDifficulty Beginner() { return FMinesweeperDifficulty(9, 9, 10); }		// mine probability: 10 /  81 = 0.12345%
	static FMinesweeperDifficulty Intermediate() { return FMinesweeperDifficulty(16, 16, 40); }	// mine probability: 40 / 256 = 0.15625%
	static FMinesweeperDifficulty Expert() { return FMinesweeperDifficulty(30, 16, 99); }		// mine probability: 99 / 576 = 0.17187%

};

