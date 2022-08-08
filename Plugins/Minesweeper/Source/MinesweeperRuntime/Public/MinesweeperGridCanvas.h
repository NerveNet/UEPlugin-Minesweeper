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
	//UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void InitCanvas(UMinesweeperGame* Game, const float CellDrawSize = 28.0f);


	/** Returns the grid cell index based on a position on the grid and the cell draw size. Returns -1 if the grid position is invalid (off the grid). */
	UFUNCTION(BlueprintPure, Category = "MinesweeperGridCanvas")
		int32 GridPositionToCellIndex(const FVector2D& GridPosition) const;

	/** Returns the grid cell coordinate based on a position on the grid and the cell draw size. Returns -1 if the grid position is invalid (off the grid). */
	UFUNCTION(BlueprintPure, Category = "MinesweeperGridCanvas")
		void GridPositionToCellCoord(const FVector2D& GridPosition, int32& CellX, int32& CellY) const;


	/** Returns the draw size of each cell in pixels. */
	UFUNCTION(BlueprintPure, Category = "MinesweeperGridCanvas")
		FORCEINLINE float GetCellDrawSize() const { return CellDrawSize; }

	/** Sets the draw size of each cell in pixels. The canvas must be updated manually to reflect changes. */
	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void SetCellDrawSize(const float InCellDrawSize);


	/** Removes all hovered celvoid SetCellDrawSize(const float InCellDrawSize) { CellDrawSize = InCellDrawSize; }l drawing visualizations. */
	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		FORCEINLINE void ClearHoverCell() { HoverCellIndex = -1; }

	/** Sets the cell index that will be drawn as hovered by the mouse. -1 will skip drawing. */
	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		FORCEINLINE void SetHoverCellIndex(const int32 CellIndex) { HoverCellIndex = CellIndex; }

	/** Sets the cell coord that will be drawn as hovered by the mouse. -1 for CellX or CellY will skip drawing. */
	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void SetHoverCellCoord(const int32 CellX, const int32 CellY); // FIntVector2 not supported in blueprints


	/**  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MinesweeperGridCanvas")
		FSlateColor GetNeighborMineCountColor(const int32 MineCount);


	static FSlateColor DefaultNeighborMineCountColor(const int32 InMineCount);
	static FSlateColor DefaultHoverCellValidColor() { return FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); }
	static FSlateColor DefaultHoverCellInvalidColor() { return FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); }

	static float DefaultCellDrawSize() { return 28.0f; }


	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		FORCEINLINE void SetClosedCellTexture(UTexture2D* InClosedCellTexture) { if (InClosedCellTexture) { ClosedCellTexture = InClosedCellTexture; } }

	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		FORCEINLINE void SetOpenCellTexture(UTexture2D* InOpenCellTexture) { if (InOpenCellTexture) { OpenCellTexture = InOpenCellTexture; } }

	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		FORCEINLINE void SetOpenCellMineTexture(UTexture2D* InOpenCellMineTexture) { if (InOpenCellMineTexture) { OpenCellMineTexture = InOpenCellMineTexture; } }

	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		FORCEINLINE void SetMineTexture(UTexture2D* InMineTexture) { if (InMineTexture) { MineTexture = InMineTexture; } }

	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		FORCEINLINE void SetFlagTexture(UTexture2D* InFlagTexture) { if (InFlagTexture) { FlagTexture = InFlagTexture; } }

	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		FORCEINLINE void SetHoverCellTexture(UTexture2D* InHoverCellTexture) { if (InHoverCellTexture) { HoverCellTexture = InHoverCellTexture; } }

	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		FORCEINLINE void SetCellFont(UFont* InCellFont) { if (InCellFont) { CellFont = InCellFont; } }


protected:
	UPROPERTY() UMinesweeperGame* Game = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperGridCanvas")
		float CellDrawSize = 28.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperGridCanvas")
		UTexture2D* ClosedCellTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperGridCanvas")
		UTexture2D* OpenCellTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperGridCanvas")
		UTexture2D* OpenCellMineTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperGridCanvas")
		UTexture2D* MineTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperGridCanvas")
		UTexture2D* FlagTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperGridCanvas")
		UTexture2D* HoverCellTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperGridCanvas")
		UFont* CellFont = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperGridCanvas")
		FLinearColor HoverCellValidColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesweeperGridCanvas")
		FLinearColor HoverCellInvalidColor;


	UPROPERTY() int32 HoverCellIndex = -1;


	/**  */
	UFUNCTION() virtual void UpdateCanvas(UCanvas* InCanvas, const int32 InWidth, const int32 InHeight);

};
