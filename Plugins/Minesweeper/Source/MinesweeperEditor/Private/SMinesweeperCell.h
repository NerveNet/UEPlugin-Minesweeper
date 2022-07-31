// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/DeclarativeSyntaxSupport.h"




/**
 * Represents a single minesweeper cell on the grid.
*/
class MINESWEEPEREDITOR_API SMinesweeperCell : public SCompoundWidget
{
public:
	/** Delegate signature for when this cell is clicked by a mouse button. */
	DECLARE_DELEGATE_OneParam(FOnCellClick, TSharedPtr<SMinesweeperCell>);

	SLATE_BEGIN_ARGS(SMinesweeperCell)
	{ }

		/** The delegate for when this cell is left clicked. */
		SLATE_EVENT(FOnCellClick, OnCellLeftClick)

		/** The delegate for when this cell is right clicked. */
		SLATE_EVENT(FOnCellClick, OnCellRightClick)

	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs, const FIntVector2 InCellCoord);


private:
	/** The delegate for when this cell is left clicked. */
	FOnCellClick OnCellLeftClick;

	/** The delegate for when this cell is right clicked. */
	FOnCellClick OnCellRightClick;


	FText GetToolTipText() const;
	FSlateColor GetBackgroundColorAndOpacity() const;
	const FSlateBrush* GetBackgroundImage() const;
	EVisibility GetMineCountTextVisibility() const;
	FSlateColor GetMineCountTextColor() const;
	FText GetMineCountText() const;
	EVisibility GetMineImageVisibility() const;
	EVisibility GetFlagImageVisibility() const;


	//~ Begin SWidget Overrides
	FReply OnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//~ End SWidget Overrides


public:
	/** The coordinates on the grid of this cell. */
	FIntVector2 CellCoordinates;

	/** True if this cell contains a mine. */
	bool HasMine = false;

	/** True if this cell should display it's mine. */
	bool ShowMine = false;

	/** Holds the number of mines that surround this cell. */
	int32 NeighborMineCount = -1;

	/** True if this cell has been left clicked and is open. */
	bool IsOpened = false;

	/** True if the user has marked this cell with a flag. */
	bool IsFlagged = false;

	/** True if the user is allowed to mark this cell with a flag. */
	bool CanPlaceFlag = true;


	/** Resets this cell to it's default state for a new game. */
	void Reset();

};

