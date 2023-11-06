// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinesweeperVisualTheme.h"
#include "MinesweeperGrid.generated.h"

class SMinesweeperGrid;
class UMinesweeperGame;
class UMinesweeperGridCanvas;
class UTexture2D;




DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMinesweeperCellClickDelegate, const int32, CellX, const int32, CellY, const FVector2D&, GridPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FMinesweeperCellHoverDelegate, const bool, IsHovered, const int32, CellX, const int32, CellY, const FVector2D&, GridPosition);


/**
 * UMinesweeperGrid - UMG widget visual representation of a Minesweeper grid.
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class MINESWEEPERRUNTIME_API UMinesweeperGrid : public UWidget
{
	GENERATED_UCLASS_BODY()
	
public:
	//~ Begin UWidget Overrides
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	virtual void SynchronizeProperties() override;
	//~ End UWidget Overrides

	//~ Begin UVisual Overrides
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Overrides


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MinesweeperGrid")
		FMinesweeperVisualTheme VisualTheme;


	UPROPERTY(BlueprintAssignable, Category = "MinesweeperGrid")
		FMinesweeperCellClickDelegate OnCellLeftClick;

	UPROPERTY(BlueprintAssignable, Category = "MinesweeperGrid")
		FMinesweeperCellClickDelegate OnCellRightClick;

	UPROPERTY(BlueprintAssignable, Category = "MinesweeperGrid")
		FMinesweeperCellHoverDelegate OnCellHover;


	/** Returns the Minesweeper game logic object used to draw the grid. */
	UFUNCTION(BlueprintPure, Category = "MinesweeperGrid")
		UMinesweeperGame* GetGame() const;

	/**  */
	UFUNCTION(BlueprintCallable, Category = "MinesweeperGrid")
		void SetupGridCanvas(UMinesweeperGame* Game, const FMinesweeperVisualTheme& Theme);


	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void SetVisualTheme(const FMinesweeperVisualTheme& Theme);

	UFUNCTION(BlueprintPure, Category = "MinesweeperGrid")
		FORCEINLINE float GetCellDrawSize() const { return VisualTheme.CellDrawSize; }

	UFUNCTION(BlueprintCallable, Category = "MinesweeperGrid")
		void SetCellDrawSize(const float Size);


	UFUNCTION(BlueprintCallable, Category = "MinesweeperGrid")
		void UpdateResource();


	/** Removes all hovered cell drawing visualizations. */
	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void ClearHoverCell();

	/** Sets the cell index that will be drawn as hovered by the mouse. -1 will skip drawing. */
	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void SetHoverCellIndex(const int32 CellIndex);

	/** Sets the cell coord that will be drawn as hovered by the mouse. -1 for CellX or CellY will skip drawing. */
	UFUNCTION(BlueprintCallable, Category = "MinesweeperGridCanvas")
		void SetHoverCellCoord(const int32 CellX, const int32 CellY); // FIntVector2 not supported in blueprints


protected:
	void SlateHandleCellLeftClick(const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition);
	void SlateHandleCellRightClick(const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition);
	void SlateHandleCellHoverChange(const bool InIsHovered, const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition);


	//~ Begin UWidget Overrides
#if WITH_EDITOR
	virtual TSharedRef<SWidget> RebuildDesignWidget(TSharedRef<SWidget> InContent) override;
#endif
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Overrides


	/** Cached pointer to the underlying slate widget owned by this UWidget. */
	TSharedPtr<SMinesweeperGrid> MyGrid;


#if WITH_EDITOR
	FSlateBrush OpenCellBrush;
	FSlateBrush MineBrush;
#endif

};

