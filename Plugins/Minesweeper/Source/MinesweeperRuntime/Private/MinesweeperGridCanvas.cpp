// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperGridCanvas.h"
#include "MinesweeperRuntimeModule.h"
#include "MinesweeperGame.h"
#include "MinesweeperStatics.h"
#include "MinesweeperVisualTheme.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"


#define LOCTEXT_NAMESPACE "Minesweeper"


//#define DEFINE_DEBUG_MINES // will always show all mines and neighbor mine count texts on the grid if defined




UMinesweeperGridCanvas::UMinesweeperGridCanvas()
{
	VisualTheme.CopyIfNotNull(UMinesweeperStatics::DefaultVisualTheme());

	OnCanvasRenderTargetUpdate.AddDynamic(this, &UMinesweeperGridCanvas::UpdateCanvas);
}


void UMinesweeperGridCanvas::InitCanvas(UMinesweeperGame* InGame, const FMinesweeperVisualTheme& InVisualTheme)
{
	Game = InGame;

	VisualTheme.CopyIfNotNull(InVisualTheme);

	UpdateResource();
}


void UMinesweeperGridCanvas::SetVisualTheme(const FMinesweeperVisualTheme& InVisualTheme)
{
	VisualTheme.CopyIfNotNull(InVisualTheme);

	UpdateResource();
}

void UMinesweeperGridCanvas::SetCellDrawSize(const float InCellDrawSize)
{
	VisualTheme.CellDrawSize = UMinesweeperStatics::ClampCellDrawSize(InCellDrawSize);

	UpdateResource();
}


int32 UMinesweeperGridCanvas::GridPositionToCellIndex(const FVector2D& InGridPosition) const
{
	return Game == nullptr ? -1 : Game->GridCoordToIndex(FIntVector2(InGridPosition.X / VisualTheme.CellDrawSize, InGridPosition.Y / VisualTheme.CellDrawSize));
}

void UMinesweeperGridCanvas::GridPositionToCellCoord(const FVector2D& InGridPosition, int32& OutCellX, int32& OutCellY) const
{
	if (Game)
	{
		OutCellX = InGridPosition.X / VisualTheme.CellDrawSize;
		OutCellY = InGridPosition.Y / VisualTheme.CellDrawSize;
	}
	else
	{
		OutCellX = -1;
		OutCellY = -1;
	}
}

void UMinesweeperGridCanvas::MouseEventToCellCoord(const FGeometry& InGeometry, const FPointerEvent& InEvent, int32& OutCellX, int32& OutCellY) const
{
	const FVector2D gridPosition = InGeometry.AbsoluteToLocal(InEvent.GetScreenSpacePosition());
	GridPositionToCellCoord(gridPosition, OutCellX, OutCellY);
}


void UMinesweeperGridCanvas::SetHoverCellIndex(const int32 InCellIndex)
{
	if (!Game) return;
	HoverCellIndex = Game->IsValidGridIndex(InCellIndex) ? InCellIndex : -1;
}

void UMinesweeperGridCanvas::SetHoverCellCoord(const int32 InCellX, const int32 InCellY)
{
	if (!Game) return;
	HoverCellIndex = Game->IsValidGridCoord(FIntVector2(InCellX, InCellY)) ? Game->GridCoordToIndex(FIntVector2(InCellX, InCellY)) : -1;
}


FSlateColor UMinesweeperGridCanvas::GetNeighborMineCountColor_Implementation(const int32 InMineCount)
{
	return UMinesweeperStatics::DefaultNeighborMineCountColor(InMineCount);
}


void UMinesweeperGridCanvas::UpdateCanvas(UCanvas* InCanvas, const int32 InWidth, const int32 InHeight)
{
	if (!InCanvas || !Game) return;


	auto DrawCell = [&](const FVector2D& InPosition, const UTexture2D* InTexture, FVector2D InSize = FVector2D::ZeroVector, const FLinearColor InColor = FLinearColor(1.0f, 1.0f, 1.0f))
	{
		if (!InTexture) return;
		if (InSize == FVector2D::ZeroVector) InSize = FVector2D(VisualTheme.CellDrawSize);
		FCanvasTileItem canvasTileItem(InPosition, InTexture->GetResource(), InSize, InColor);
		canvasTileItem.BlendMode = SE_BLEND_Translucent;
		InCanvas->DrawItem(canvasTileItem);
	};


	// draw the minesweeper grid
	Game->ForEachCell([&](const TSharedPtr<FMinesweeperCell> InCell, const int32 InCellIndex, const FVector2D InCellCoord)
		{
			const FVector2D cellPosition = InCellCoord * VisualTheme.CellDrawSize;


			// draw open/closed cell background
			{
				UTexture2D* backgroundTexture = VisualTheme.ClosedCellTexture;
				if (InCell->bIsOpened)
				{
					backgroundTexture = InCell->bHasMine ? VisualTheme.OpenCellMineTexture : VisualTheme.OpenCellTexture;
				}
				DrawCell(cellPosition, backgroundTexture);
			}


			// draw neighbor mine count text
#ifdef DEFINE_DEBUG_MINES
			const bool drawNeighborMineCount = true;
#else
			const bool drawNeighborMineCount = InCell->bIsOpened && !InCell->bHasMine && InCell->NeighborMineCount > 0;
#endif
			if (VisualTheme.CellFont && drawNeighborMineCount)
			{
				FString neighborMineCountStr = FString::FromInt(InCell->NeighborMineCount);
				FText neighborMineCountText = FText::FromString(neighborMineCountStr);
				
				float outWidth, outHeight;
				VisualTheme.CellFont->GetCharSize(neighborMineCountStr[0], outWidth, outHeight);
				int32 textWidth = VisualTheme.CellFont->GetStringSize(*neighborMineCountStr);

				const float percentOfCellSize = 0.8f;

				FVector2D textPosition = cellPosition + FVector2D((outWidth * 0.5f) * percentOfCellSize, (outHeight * 0.5f) * 0.2f);
				
				float scale = (VisualTheme.CellDrawSize / outHeight) * percentOfCellSize;

				FCanvasTextItem textItem(textPosition, neighborMineCountText, VisualTheme.CellFont, GetNeighborMineCountColor(InCell->NeighborMineCount).GetSpecifiedColor());
				textItem.Scale = FVector2D(scale);
				textItem.BlendMode = SE_BLEND_Translucent;
				InCanvas->DrawItem(textItem);
			}

			
			// draw mine
#ifdef DEFINE_DEBUG_MINES
			const bool drawMine = InCell->bHasMine;
#else
			const bool drawMine = InCell->bHasMine && Game->IsGameOver();
#endif
			if (drawMine)
			{
				DrawCell(cellPosition, VisualTheme.MineTexture);
			}


			// draw flag
			if (!InCell->bIsOpened && InCell->bIsFlagged)
			{
				DrawCell(cellPosition, VisualTheme.FlagTexture);
			}


			// draw hover cell outline
			if (HoverCellIndex > -1 && InCellIndex == HoverCellIndex)
			{
				DrawCell(cellPosition, VisualTheme.HoverCellTexture, FVector2D::ZeroVector, InCell->bIsOpened ? VisualTheme.HoverCellInvalidColor : VisualTheme.HoverCellValidColor);
			}
		});
}




#undef LOCTEXT_NAMESPACE
