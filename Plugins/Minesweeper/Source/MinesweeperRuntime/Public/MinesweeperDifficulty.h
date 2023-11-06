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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperDifficulty", Meta = (UIMin = 1, ClampMin = 1, UIMax = 30, ClampMax = 30))
		int32 Width = 1;

	/** Height of the Minesweeper grid in cells. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperDifficulty", Meta = (UIMin = 1, ClampMin = 1, UIMax = 30, ClampMax = 30))
		int32 Height = 1;

	/** Mine count of the Minesweeper grid. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperDifficulty", Meta = (UIMin = 1, ClampMin = 1, UIMax = 400, ClampMax = 400))
		int32 MineCount = 1;


	FMinesweeperDifficulty() 
		: Width(1), Height(1), MineCount(1) { }
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

};

