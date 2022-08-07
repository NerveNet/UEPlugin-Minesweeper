// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "MinesweeperGridCanvas.generated.h"

class UMinesweeperGame;




/**
 * Canvas render target texture used to draw all grid cells. Inherit in blueprints to enable custom textures and cell draw size.
 */
UCLASS()
class MINESWEEPERRUNTIME_API UMinesweeperGridCanvas : public UCanvasRenderTarget2D
{
	GENERATED_BODY()
	
public:
	UMinesweeperGridCanvas();


	/// <summary>
	/// Initializes this canvas with a Minesweeper game and cell draw size.
	/// </summary>
	/// <param name="Game">The Minesweeper game logic object.</param>
	/// <param name="CellDrawSize">The draw size in pixels for each cell.</param>
	UFUNCTION(BlueprintCallable, Category = "Minesweeper")
		void InitCanvas(UMinesweeperGame* Game, const float CellDrawSize);


	/** Returns the grid cell index based on a position on the grid. Returns -1 if the grid position is invalid (off the grid). */
	UFUNCTION(BlueprintCallable, Category = "Minesweeper")
		int32 GridPositionToGridIndex(UPARAM(ref) const FVector2D& GridPosition) const;


	/** Returns the draw size of each cell in pixels. */
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		float GetCellDrawSize() const { return CellDrawSize; }


	/**  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Minesweeper")
		FSlateColor GetNeighborMineCountColor(const int32 MineCount);

	static FSlateColor DefaultNeighborMineCountColor(const int32 InMineCount);


	/**  */
	UFUNCTION(BlueprintCallable, Category = "Minesweeper")
		void SetHoverCellIndex(const int32 CellIndex);


protected:
	UPROPERTY() UMinesweeperGame* Game = nullptr;


	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minesweeper")
		float CellDrawSize = 28.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minesweeper")
		UTexture2D* ClosedCellTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minesweeper")
		UTexture2D* OpenCellTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minesweeper")
		UTexture2D* OpenCellMineTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minesweeper")
		UTexture2D* MineTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minesweeper")
		UTexture2D* FlagTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minesweeper")
		UFont* CellFont = nullptr;


	/**  */
	UFUNCTION() virtual void UpdateCanvas(UCanvas* InCanvas, const int32 InWidth, const int32 InHeight);

};
