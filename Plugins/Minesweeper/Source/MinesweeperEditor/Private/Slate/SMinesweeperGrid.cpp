// Copyright 2022 Brad Monahan. All Rights Reserved.
#include "Slate/SMinesweeperGrid.h"
#include "SlateOptMacros.h"


#define LOCTEXT_NAMESPACE "SMinesweeperGrid"




BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMinesweeperGrid::Construct(const FArguments& InArgs)
{
	OnCellLeftClick = InArgs._OnCellLeftClick;
	OnCellRightClick = InArgs._OnCellRightClick;
	OnHoverCellChanged = InArgs._OnHoverCellChanged;

	SImage::Construct(
		SImage::FArguments().Image(InArgs._GridCanvasBrush)
	);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


FCursorReply SMinesweeperGrid::OnCursorQuery(const FGeometry& InMyGeometry, const FPointerEvent& InCursorEvent) const
{
	return SImage::OnCursorQuery(InMyGeometry, InCursorEvent);
}

TOptional<TSharedRef<SWidget>> SMinesweeperGrid::OnMapCursor(const FCursorReply& InCursorReply) const
{
	return SImage::OnMapCursor(InCursorReply);
}

FReply SMinesweeperGrid::OnMouseButtonDown(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	const FVector2D localMousePosition = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		OnCellLeftClick.ExecuteIfBound(localMousePosition);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		OnCellRightClick.ExecuteIfBound(localMousePosition);
	}

	return FReply::Handled();
}

FReply SMinesweeperGrid::OnMouseButtonUp(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void SMinesweeperGrid::OnMouseEnter(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	const FVector2D localMousePosition = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	OnHoverCellChanged.ExecuteIfBound(true, localMousePosition);
}

void SMinesweeperGrid::OnMouseLeave(const FPointerEvent& InMouseEvent)
{
	OnHoverCellChanged.ExecuteIfBound(false, FVector2D::ZeroVector);
}

FReply SMinesweeperGrid::OnMouseMove(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	const FVector2D localMousePosition = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	//UE_LOG(LogMinesweeperEditor, Log, TEXT("%s"), *localMousePosition.ToString());

	OnHoverCellChanged.ExecuteIfBound(true, localMousePosition);

	return FReply::Handled();
}




#undef LOCTEXT_NAMESPACE

