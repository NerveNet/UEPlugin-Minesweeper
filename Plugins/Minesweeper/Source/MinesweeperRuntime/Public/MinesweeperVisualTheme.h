// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "MinesweeperVisualTheme.generated.h"

class UTexture2D;
class UFont;




/**
 * Minesweeper Visual Theme
 */
USTRUCT(BlueprintType)
struct MINESWEEPERRUNTIME_API FMinesweeperVisualTheme
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Config, EditAnywhere, AdvancedDisplay, Category = "General", Meta = (UIMin = 10.0, ClampMin = 10.0, UIMax = 64.0, ClampMax = 64.0,
			DisplayName = "Cell Draw Size",
			Tooltip = "The size of each cell on the screen in pixels."))
		float CellDrawSize = 28.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperVisualTheme")
		UTexture2D* ClosedCellTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperVisualTheme")
		UTexture2D* OpenCellTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperVisualTheme")
		UTexture2D* OpenCellMineTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperVisualTheme")
		UTexture2D* MineTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperVisualTheme")
		UTexture2D* FlagTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperVisualTheme")
		UTexture2D* HoverCellTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperVisualTheme")
		UFont* CellFont = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperVisualTheme")
		FLinearColor HoverCellValidColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperVisualTheme")
		FLinearColor HoverCellInvalidColor;


	void CopyIfNotNull(const FMinesweeperVisualTheme& CopyFrom);
	void CopyObjectsIfNotNull(const FMinesweeperVisualTheme& CopyFrom);

};
