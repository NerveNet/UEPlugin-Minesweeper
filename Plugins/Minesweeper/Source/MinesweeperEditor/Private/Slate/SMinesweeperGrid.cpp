// Copyright 2022 Brad Monahan. All Rights Reserved.
#include "Slate/SMinesweeperGrid.h"
#include "MinesweeperStyle.h"
#include "MinesweeperSettings.h"
#include "MinesweeperGame.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Images/SImage.h"


#define LOCTEXT_NAMESPACE "SMinesweeperGrid"




SMinesweeperGrid::SMinesweeperGrid()
{
	// create the canvas we will use to draw all the grid cells on
	FIntVector2 gridSize = Game->GetDifficulty().GridSize();
	FVector2D gridCanvasSize(gridSize.X * CellDrawSize, gridSize.Y * CellDrawSize);

	GridCanvas = TStrongObjectPtr<UMinesweeperGridCanvas>(
		(UMinesweeperGridCanvas*)UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(
			GEditor->GetEditorWorldContext().World(),
			UMinesweeperGridCanvas::StaticClass(),
			gridCanvasSize.X, gridCanvasSize.Y
		)
	);

	GridCanvasBrush.SetResourceObject(GridCanvas.Get());
	GridCanvasBrush.SetImageSize(gridCanvasSize);
	GridCanvasBrush.TintColor = FLinearColor::White;
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMinesweeperGrid::Construct(const FArguments& InArgs, TStrongObjectPtr<UMinesweeperGame> InGame)
{
	CellDrawSize = InArgs._CellDrawSize;

	Game = InGame;
	check(Game.IsValid());
	//Game->OnGameOver.AddDynamic(this, &SMinesweeperGrid::OnGameOver);

	GridCanvas->InitCanvas(Game.Get(), CellDrawSize);

	ChildSlot.Padding(0)
		[
			SAssignNew(GridCanvasWidget, SImage)
			.Image(&GridCanvasBrush)
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


FCursorReply SMinesweeperGrid::OnCursorQuery(const FGeometry& InMyGeometry, const FPointerEvent& InCursorEvent) const
{
	return SCompoundWidget::OnCursorQuery(InMyGeometry, InCursorEvent);
}

TOptional<TSharedRef<SWidget>> SMinesweeperGrid::OnMapCursor(const FCursorReply& InCursorReply) const
{
	return SCompoundWidget::OnMapCursor(InCursorReply);
}

FReply SMinesweeperGrid::OnMouseButtonDown(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	const FVector2D localMousePosition = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	const FIntVector2 cellCoord(localMousePosition.X / CellDrawSize, localMousePosition.Y / CellDrawSize);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		Game->TryOpenCell(cellCoord.X, cellCoord.Y);
		GridCanvas->UpdateResource();
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		Game->TryFlagCall(cellCoord.X, cellCoord.Y);
		GridCanvas->UpdateResource();
	}

	return SCompoundWidget::OnMouseButtonDown(InMyGeometry, InMouseEvent);
}

FReply SMinesweeperGrid::OnMouseButtonUp(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return SCompoundWidget::OnMouseButtonUp(InMyGeometry, InMouseEvent);
}

void SMinesweeperGrid::OnMouseEnter(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return SCompoundWidget::OnMouseEnter(InMyGeometry, InMouseEvent);
}

void SMinesweeperGrid::OnMouseLeave(const FPointerEvent& InMouseEvent)
{
	return SCompoundWidget::OnMouseLeave(InMouseEvent);
}

FReply SMinesweeperGrid::OnMouseMove(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	//const FVector2D localMousePosition = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	//const FIntVector2 cellCoord(localMousePosition.X / CellDrawSize, localMousePosition.Y / CellDrawSize);
	//UE_LOG(LogMinesweeperEditor, Log, TEXT("%s"), *localMousePosition.ToString());

	return SCompoundWidget::OnMouseMove(InMyGeometry, InMouseEvent);
}




#undef LOCTEXT_NAMESPACE

