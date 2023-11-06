// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/Images/SImage.h"
#include "MinesweeperGridCanvas.h"

class UMinesweeperGame;




DECLARE_DELEGATE_ThreeParams(FMinesweeperGridCellClickDelegate, const int32, const int32, const FVector2D&);
DECLARE_DELEGATE_FourParams(FMinesweeperGridCellHoverDelegate, const bool, const int32, const int32, const FVector2D&);


/**
 * SMinesweeperGrid - Visual representation of a Minesweeper grid.
 */
class MINESWEEPERRUNTIME_API SMinesweeperGrid : public SImage
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperGrid)
	{ }

		SLATE_ARGUMENT(UMinesweeperGame*, Game)

		SLATE_ARGUMENT(FMinesweeperVisualTheme, VisualTheme)

		SLATE_EVENT(FMinesweeperGridCellClickDelegate, OnCellLeftClick)

		SLATE_EVENT(FMinesweeperGridCellClickDelegate, OnCellRightClick)

		SLATE_EVENT(FMinesweeperGridCellHoverDelegate, OnCellHoverChange)

	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);


	//~ Begin SWidget Overrides
	virtual FVector2D ComputeDesiredSize(float InLayoutScaleMultiplier) const override;
	//virtual FCursorReply OnCursorQuery(const FGeometry& InMyGeometry, const FPointerEvent& InCursorEvent) const override;
	//virtual TOptional<TSharedRef<SWidget>> OnMapCursor(const FCursorReply& InCursorReply) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& InMyGeometry,const FPointerEvent& InMouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	//~ End SWidget Overrides


	inline UMinesweeperGridCanvas* GetGridCanvas() const { return GridCanvas.Get(); }


	void SetupGridCanvas(UMinesweeperGame* InGame, const FMinesweeperVisualTheme& InVisualTheme);


	void SetVisualTheme(const FMinesweeperVisualTheme& InVisualTheme);

	int32 GetCellDrawSize() const;
	void SetCellDrawSize(const float InCellDrawSize);


	int32 GridPositionToCellIndex(const FVector2D& InGridPosition) const;
	void GridPositionToCellCoord(const FVector2D& InGridPosition, int32& OutCellX, int32& OutCellY) const;
	void MouseEventToCellCoord(const FGeometry& InGeometry, const FPointerEvent& InEvent, int32& OutCellX, int32& OutCellY) const;


	void ClearHoverCell();
	void SetHoverCellIndex(const int32 InCellIndex);
	void SetHoverCellCoord(const FIntVector2& InCellCoord);


	void UpdateResource();


private:
	FMinesweeperVisualTheme VisualTheme;

	/** Render target texture where the cell textures are drawn for each cell. */
	TStrongObjectPtr<UMinesweeperGridCanvas> GridCanvas;

	FSlateBrush GridCanvasBrush;

	FMinesweeperGridCellClickDelegate OnCellLeftClick;
	FMinesweeperGridCellClickDelegate OnCellRightClick;
	FMinesweeperGridCellHoverDelegate OnCellHoverChange;

};
