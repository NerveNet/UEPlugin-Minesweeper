// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/Images/SImage.h"




DECLARE_DELEGATE_OneParam(FMinesweeperGridPositionDelegate, const FVector2D&);
DECLARE_DELEGATE_TwoParams(FMinesweeperGridHoverPositionDelegate, const bool, const FVector2D&);


/**
 * SMinesweeperGrid - Visual representation of a Minesweeper grid.
 */
class SMinesweeperGrid : public SImage
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperGrid)
	{ }

		SLATE_EVENT(FMinesweeperGridPositionDelegate, OnCellLeftClick)

		SLATE_EVENT(FMinesweeperGridPositionDelegate, OnCellRightClick)

		SLATE_EVENT(FMinesweeperGridHoverPositionDelegate, OnHoverCellChanged)
	
		SLATE_ARGUMENT(const FSlateBrush*, GridCanvasBrush)

	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);


	//~ Begin SWidget Overrides
	virtual FCursorReply OnCursorQuery(const FGeometry& InMyGeometry, const FPointerEvent& InCursorEvent) const override;
	virtual TOptional<TSharedRef<SWidget>> OnMapCursor(const FCursorReply& InCursorReply) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& InMyGeometry,const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	//~ End SWidget Overrides


private:
	FMinesweeperGridPositionDelegate OnCellLeftClick;
	FMinesweeperGridPositionDelegate OnCellRightClick;
	FMinesweeperGridHoverPositionDelegate OnHoverCellChanged;

};
