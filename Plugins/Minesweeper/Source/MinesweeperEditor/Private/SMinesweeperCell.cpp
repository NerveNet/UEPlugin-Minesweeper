// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "SMinesweeperCell.h"
#include "MinesweeperStyle.h"
#include "SlateOptMacros.h"
#include "Widgets/SOverlay.h"


#define LOCTEXT_NAMESPACE "SMinesweeperCell"

//#define DEFINE_DEBUG_MINES // will always show all mines and neighbor mine count texts on the grid if defined




BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMinesweeperCell::Construct(const FArguments& InArgs, const FIntVector2 InCellCoord)
{
	SetCanTick(false);

	CellCoordinates = InCellCoord;

	OnCellLeftClick = InArgs._OnCellLeftClick;
	OnCellRightClick = InArgs._OnCellRightClick;

	ChildSlot
	[
		SNew(SOverlay)
		.ToolTipText(this, &SMinesweeperCell::GetToolTipText)
		.Visibility(EVisibility::SelfHitTestInvisible)
		+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.Visibility(EVisibility::SelfHitTestInvisible)
			.ColorAndOpacity(this, &SMinesweeperCell::GetBackgroundColorAndOpacity)
			.Image(this, &SMinesweeperCell::GetBackgroundImage)
		]
		+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.TextStyle(FMinesweeperStyle::Get(), "Text.MineCount")
			.Visibility(this, &SMinesweeperCell::GetMineCountTextVisibility)
			.ColorAndOpacity(this, &SMinesweeperCell::GetMineCountTextColor)
			.Text(this, &SMinesweeperCell::GetMineCountText)
		]
		+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.Visibility(this, &SMinesweeperCell::GetMineImageVisibility)
			.Image(FMinesweeperStyle::GetBrush("Mine"))
		]
		+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.Visibility(this, &SMinesweeperCell::GetFlagImageVisibility)
			.Image(FMinesweeperStyle::GetBrush("Flag"))
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


FText SMinesweeperCell::GetToolTipText() const
{
	return FText::FromString("(" + FString::FromInt(CellCoordinates.X) + ", " + FString::FromInt(CellCoordinates.Y) + ")");
}

FSlateColor SMinesweeperCell::GetBackgroundColorAndOpacity() const
{
	return FLinearColor::White;
}

const FSlateBrush* SMinesweeperCell::GetBackgroundImage() const
{
	FName backgroundBrushNAme = "ClosedCell";
	if (IsOpened)
	{
		backgroundBrushNAme = HasMine ? "OpenCell.Mine" : "OpenCell";
	}
	return FMinesweeperStyle::GetBrush(backgroundBrushNAme);
}

EVisibility SMinesweeperCell::GetMineCountTextVisibility() const
{
#ifdef DEFINE_DEBUG_MINES
	return EVisibility::SelfHitTestInvisible;
#else
	return (IsOpened && NeighborMineCount > 0) ? EVisibility::SelfHitTestInvisible : EVisibility::Hidden;
#endif
}

FSlateColor SMinesweeperCell::GetMineCountTextColor() const
{
	if (NeighborMineCount == 1) return FMinesweeperStyle::GetColor("Color.OneNeighborText");
	if (NeighborMineCount == 2) return FMinesweeperStyle::GetColor("Color.TwoNeighborText");
	if (NeighborMineCount == 3) return FMinesweeperStyle::GetColor("Color.ThreeNeighborText");
	if (NeighborMineCount == 4) return FMinesweeperStyle::GetColor("Color.FourNeighborText");
	if (NeighborMineCount == 5) return FMinesweeperStyle::GetColor("Color.FiveNeighborText");
	if (NeighborMineCount == 6) return FMinesweeperStyle::GetColor("Color.SixNeighborText");
	if (NeighborMineCount == 7) return FMinesweeperStyle::GetColor("Color.SevenNeighborText");
	if (NeighborMineCount == 8) return FMinesweeperStyle::GetColor("Color.EightNeighborText");
	return FLinearColor::White;
}

FText SMinesweeperCell::GetMineCountText() const
{
	if (NeighborMineCount == 0 || HasMine)
	{
		return FText();
	}
	return FText::FromString(FString::FromInt(NeighborMineCount));
}

EVisibility SMinesweeperCell::GetMineImageVisibility() const
{
#ifdef DEFINE_DEBUG_MINES
	return (HasMine) ? EVisibility::SelfHitTestInvisible : EVisibility::Hidden;
#else
	return (HasMine && ShowMine) ? EVisibility::SelfHitTestInvisible : EVisibility::Hidden;
#endif
}

EVisibility SMinesweeperCell::GetFlagImageVisibility() const
{
	return (!IsOpened && IsFlagged) ? EVisibility::SelfHitTestInvisible : EVisibility::Hidden;
}


FReply SMinesweeperCell::OnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const FKey key = InMouseEvent.GetEffectingButton();

	if (key == EKeys::LeftMouseButton)
	{
		if (!IsOpened && !IsFlagged)
		{
			// broadcast left click message
			if (OnCellLeftClick.IsBound())
			{
				OnCellLeftClick.Execute(SharedThis(this));
			}
		}
	}
	else if (key == EKeys::RightMouseButton)
	{
		if (!IsOpened && CanPlaceFlag)
		{
			IsFlagged = !IsFlagged;

			// broadcast right click message
			if (OnCellRightClick.IsBound())
			{
				OnCellRightClick.Execute(SharedThis(this));
			}
		}
	}

	return FReply::Handled();
}




#undef LOCTEXT_NAMESPACE
