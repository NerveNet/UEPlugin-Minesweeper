// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/DeclarativeSyntaxSupport.h"




class MINESWEEPEREDITOR_API SMinesweeperCell : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnCellClick, TSharedPtr<SMinesweeperCell>);

	SLATE_BEGIN_ARGS(SMinesweeperCell)
	{ }

		SLATE_EVENT(FOnCellClick, OnCellLeftClick)

		SLATE_EVENT(FOnCellClick, OnCellRightClick)

	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs, const FIntVector2 InCellCoord);


private:
	FOnCellClick OnCellLeftClick;
	FOnCellClick OnCellRightClick;


	FText GetToolTipText() const;
	FSlateColor GetBackgroundColorAndOpacity() const;
	const FSlateBrush* GetBackgroundImage() const;
	EVisibility GetMineCountTextVisibility() const;
	FSlateColor GetMineCountTextColor() const;
	FText GetMineCountText() const;
	EVisibility GetMineImageVisibility() const;
	EVisibility GetFlagImageVisibility() const;


	FReply OnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


public:
	FIntVector2 CellCoordinates;

	bool IsOpened = false;
	bool IsFlagged = false;
	bool HasMine = false;
	bool ShowMine = false;

	int32 NeighborMineCount = -1;
	bool CanPlaceFlag = true;

};

