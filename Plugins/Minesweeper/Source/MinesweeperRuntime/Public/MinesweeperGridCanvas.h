// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "MinesweeperVisualTheme.h"
#include "MinesweeperGridCanvas.generated.h"

class UMinesweeperGame;




/**
 * Canvas render target texture used to draw all grid cells. Inherit in blueprints to enable custom textures and cell draw size.
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class MINESWEEPERRUNTIME_API UMinesweeperGridCanvas : public UCanvasRenderTarget2D
{
	GENERATED_BODY()
	
public:
	UMinesweeperGridCanvas();


	/** Returns the Minesweeper game logic object used to draw the grid. */
	UFUNCTION(BlueprintPure, Category = "MinesweeperGrid")
		FORCEINLINE UMinesweeperGame* GetGame() const { return Game; }


	/// <summary>
	/// Initializes this canvas with a Minesweeper game and cell draw size.
	/// </summary>
	/// <param name="Game">The Minesweeper game logic object.</param>
	/// <param name="CellDrawSize">The draw size in pixels for each cell.</param>
	//UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void InitCanvas(UMinesweeperGame* Game, const FMinesweeperVisualTheme& InVisualTheme);


	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void SetVisualTheme(const FMinesweeperVisualTheme& Theme);

	UFUNCTION(BlueprintPure, Category = "MinesweeperGridCanvas")
		FORCEINLINE float GetCellDrawSize() const { return VisualTheme.CellDrawSize; }

	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void SetCellDrawSize(const float Size);


	/** Returns the grid cell index based on a position on the grid and the cell draw size. Returns -1 if the grid position is invalid (off the grid). */
	UFUNCTION(BlueprintPure, Category = "MinesweeperGridCanvas")
		int32 GridPositionToCellIndex(const FVector2D& GridPosition) const;

	/** Returns the grid cell coordinate based on a position on the grid and the cell draw size. Returns -1 if the grid position is invalid (off the grid). */
	UFUNCTION(BlueprintPure, Category = "MinesweeperGridCanvas")
		void GridPositionToCellCoord(const FVector2D& GridPosition, int32& CellX, int32& CellY) const;

	/**  */
	UFUNCTION(BlueprintPure, Category = "MinesweeperGridCanvas")
		void MouseEventToCellCoord(const FGeometry& Geometry, const FPointerEvent& Event, int32& CellX, int32& CellY) const;


	/** Removes all hovered cell drawing visualizations. */
	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		FORCEINLINE void ClearHoverCell() { HoverCellIndex = -1; }

	/** Sets the cell index that will be drawn as hovered by the mouse. -1 will skip drawing. */
	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void SetHoverCellIndex(const int32 CellIndex);

	/** Sets the cell coord that will be drawn as hovered by the mouse. -1 for CellX or CellY will skip drawing. */
	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void SetHoverCellCoord(const int32 CellX, const int32 CellY); // FIntVector2 not supported in blueprints


	/** Override this function to set your own colors for the neighboring mine count text. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MinesweeperGridCanvas")
		FSlateColor GetNeighborMineCountColor(const int32 MineCount);


protected:
	UPROPERTY() UMinesweeperGame* Game = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MinesweeperGridCanvas")
		FMinesweeperVisualTheme VisualTheme;


	UPROPERTY() int32 HoverCellIndex = -1;


	UFUNCTION() virtual void UpdateCanvas(UCanvas* InCanvas, const int32 InWidth, const int32 InHeight);

};
