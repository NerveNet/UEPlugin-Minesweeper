// Copyright 2022 Brad Monahan. All Rights Reserved.
#include "Slate/SMinesweeperGrid.h"
#include "MinesweeperDifficulty.h"
#include "MinesweeperGame.h"
#include "MinesweeperGridCanvas.h"
#include "MinesweeperStatics.h"
#include "SlateOptMacros.h"


#define LOCTEXT_NAMESPACE "Minesweeper"




BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMinesweeperGrid::Construct(const FArguments& InArgs)
{
	OnCellLeftClick = InArgs._OnCellLeftClick;
	OnCellRightClick = InArgs._OnCellRightClick;
	OnCellHoverChange = InArgs._OnCellHoverChange;

	SetVisualTheme(InArgs._VisualTheme);

	SImage::Construct(
		SImage::FArguments().Image(&GridCanvasBrush)
	);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SMinesweeperGrid::SetupGridCanvas(UMinesweeperGame* InGame, const FMinesweeperVisualTheme& InVisualTheme)
{
	if (!InGame) return;

	float cellDrawSize = InVisualTheme.CellDrawSize > -1.0f ? InVisualTheme.CellDrawSize : UMinesweeperStatics::DefaultCellDrawSize();
	cellDrawSize = FMath::Clamp(cellDrawSize, UMinesweeperStatics::MinCellDrawSize(), UMinesweeperStatics::MaxCellDrawSize());

	const FIntVector2 gridSize = InGame->GetDifficulty().GridSize();
	const FVector2D gridCanvasSize(gridSize.X * cellDrawSize, gridSize.Y * cellDrawSize);

	if (!GridCanvas.IsValid())
	{
		GridCanvas = TStrongObjectPtr<UMinesweeperGridCanvas>(
			UMinesweeperStatics::CreateMinesweeperGridCanvas(GetTransientPackage(), InGame, InVisualTheme)
		);

		GridCanvasBrush.SetResourceObject(GridCanvas.Get());
		GridCanvasBrush.ImageSize = gridCanvasSize;
		//GridCanvasBrush.SetImageSize(gridCanvasSize);
	}
	else
	{
		GridCanvas->ResizeTarget(gridCanvasSize.X, gridCanvasSize.Y);

		GridCanvas->InitCanvas(InGame, InVisualTheme);
	}
}


void SMinesweeperGrid::SetVisualTheme(const FMinesweeperVisualTheme& InVisualTheme)
{
	VisualTheme.CopyIfNotNull(InVisualTheme);

	if (GridCanvas.IsValid())
	{
		GridCanvas->SetVisualTheme(InVisualTheme);
	}
}

int32 SMinesweeperGrid::GetCellDrawSize() const
{
	return GridCanvas.IsValid() ? GridCanvas->GetCellDrawSize() : -1;
}

void SMinesweeperGrid::SetCellDrawSize(const float InCellDrawSize)
{
	VisualTheme.CellDrawSize = FMath::Clamp(InCellDrawSize, UMinesweeperStatics::MinCellDrawSize(), UMinesweeperStatics::MaxCellDrawSize());

	if (GridCanvas.IsValid())
	{
		GridCanvas->SetCellDrawSize(VisualTheme.CellDrawSize);
	}
}


int32 SMinesweeperGrid::GridPositionToCellIndex(const FVector2D& InGridPosition) const
{
	if (!GridCanvas.IsValid()) return -1;
	return GridCanvas->GridPositionToCellIndex(InGridPosition);
}

void SMinesweeperGrid::GridPositionToCellCoord(const FVector2D& InGridPosition, int32& OutCellX, int32& OutCellY) const
{
	if (!GridCanvas.IsValid()) return;
	GridCanvas->GridPositionToCellCoord(InGridPosition, OutCellX, OutCellY);
}

void SMinesweeperGrid::MouseEventToCellCoord(const FGeometry& InGeometry, const FPointerEvent& InEvent, int32& OutCellX, int32& OutCellY) const
{
	if (!GridCanvas.IsValid()) return;
	GridCanvas->MouseEventToCellCoord(InGeometry, InEvent, OutCellX, OutCellY);
}


void SMinesweeperGrid::ClearHoverCell()
{
	if (!GridCanvas.IsValid()) return;
	GridCanvas->ClearHoverCell();
}

void SMinesweeperGrid::SetHoverCellIndex(const int32 CellIndex)
{
	if (!GridCanvas.IsValid()) return;
	GridCanvas->SetHoverCellIndex(CellIndex);
}

void SMinesweeperGrid::SetHoverCellCoord(const FIntVector2& InCellCoord)
{
	if (!GridCanvas.IsValid()) return;
	GridCanvas->SetHoverCellCoord(InCellCoord.X, InCellCoord.Y);
}


void SMinesweeperGrid::UpdateResource()
{
	if (!GridCanvas.IsValid()) return;
	GridCanvas->UpdateResource();
}


FVector2D SMinesweeperGrid::ComputeDesiredSize(float InLayoutScaleMultiplier) const
{
	if (GridCanvas.IsValid())
	{
		FIntVector2 size;
		GridCanvas->GetSize(size.X, size.Y);
		return FVector2D(size.X, size.Y);
	}
	return FVector2D(VisualTheme.CellDrawSize);
}

/*FCursorReply SMinesweeperGrid::OnCursorQuery(const FGeometry& InMyGeometry, const FPointerEvent& InCursorEvent) const
{
	return SImage::OnCursorQuery(InMyGeometry, InCursorEvent);
}

TOptional<TSharedRef<SWidget>> SMinesweeperGrid::OnMapCursor(const FCursorReply& InCursorReply) const
{
	return SImage::OnMapCursor(InCursorReply);
}*/

FReply SMinesweeperGrid::OnMouseButtonDown(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	if (!GridCanvas.IsValid()) return FReply::Unhandled();

	const FVector2D localMousePosition = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	FIntVector2 cellCoord;
	GridCanvas->GridPositionToCellCoord(localMousePosition, cellCoord.X, cellCoord.Y);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		OnCellLeftClick.ExecuteIfBound(cellCoord.X, cellCoord.Y, localMousePosition);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		OnCellRightClick.ExecuteIfBound(cellCoord.X, cellCoord.Y, localMousePosition);
	}

	return FReply::Handled();
}

void SMinesweeperGrid::OnMouseEnter(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	if (!GridCanvas.IsValid()) return;

	const FVector2D localMousePosition = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	FIntVector2 cellCoord;
	GridCanvas->GridPositionToCellCoord(localMousePosition, cellCoord.X, cellCoord.Y);

	OnCellHoverChange.ExecuteIfBound(true, cellCoord.X, cellCoord.Y, localMousePosition);
}

void SMinesweeperGrid::OnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (!GridCanvas.IsValid()) return;

	OnCellHoverChange.ExecuteIfBound(false, -1, -1, FVector2D(-1));
}

FReply SMinesweeperGrid::OnMouseMove(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	if (!GridCanvas.IsValid()) return FReply::Unhandled();

	const FVector2D localMousePosition = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	//UE_LOG(LogMinesweeperEditor, Log, TEXT("%s"), *localMousePosition.ToString());

	FIntVector2 cellCoord;
	GridCanvas->GridPositionToCellCoord(localMousePosition, cellCoord.X, cellCoord.Y);

	OnCellHoverChange.ExecuteIfBound(true, cellCoord.X, cellCoord.Y, localMousePosition);

	return FReply::Handled();
}




#undef LOCTEXT_NAMESPACE

