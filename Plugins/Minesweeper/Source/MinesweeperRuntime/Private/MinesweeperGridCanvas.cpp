// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperGridCanvas.h"
#include "MinesweeperRuntimeModule.h"
#include "MinesweeperGame.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"


//#define DEFINE_DEBUG_MINES // will always show all mines and neighbor mine count texts on the grid if defined




UMinesweeperGridCanvas::UMinesweeperGridCanvas()
{
	// load default textures for any unspecified
	ClosedCellTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Minesweeper/ClosedCell_64x"));
	OpenCellTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Minesweeper/OpenCell_64x"));
	OpenCellMineTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Minesweeper/OpenCell_Mine_64x"));
	MineTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Minesweeper/Mine_64x"));
	FlagTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Minesweeper/Flag_64x"));
	HoverCellTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Minesweeper/HoverCell_64x"));
	CellFont = LoadObject<UFont>(nullptr, TEXT("/Minesweeper/CellFont"));
	HoverCellValidColor = DefaultHoverCellValidColor().GetSpecifiedColor();
	HoverCellInvalidColor = DefaultHoverCellInvalidColor().GetSpecifiedColor();

	OnCanvasRenderTargetUpdate.AddDynamic(this, &UMinesweeperGridCanvas::UpdateCanvas);
}


void UMinesweeperGridCanvas::InitCanvas(UMinesweeperGame* InGame, const float InCellDrawSize)
{
	Game = InGame;

	SetCellDrawSize(InCellDrawSize);

	UpdateResource();
}


int32 UMinesweeperGridCanvas::GridPositionToCellIndex(UPARAM(ref) const FVector2D& InGridPosition) const
{
	return Game == nullptr ? -1 : Game->GridCoordToIndex(FIntVector2(InGridPosition.X / CellDrawSize, InGridPosition.Y / CellDrawSize));
}

void UMinesweeperGridCanvas::GridPositionToCellCoord(const FVector2D& GridPosition, int32& CellX, int32& CellY) const
{
	CellX = Game == nullptr ? -1 : GridPosition.X / CellDrawSize;
	CellY = Game == nullptr ? -1 : GridPosition.Y / CellDrawSize;
}


void UMinesweeperGridCanvas::SetCellDrawSize(const float InCellDrawSize)
{
	CellDrawSize = FMath::Clamp(InCellDrawSize, 10.0f, 64.0f);
}


void UMinesweeperGridCanvas::SetHoverCellCoord(const int32 CellX, const int32 CellY)
{
	HoverCellIndex = Game == nullptr ? -1 : Game->GridCoordToIndex(FIntVector2(CellX, CellY));
}


FSlateColor UMinesweeperGridCanvas::GetNeighborMineCountColor_Implementation(const int32 InMineCount)
{
	return DefaultNeighborMineCountColor(InMineCount);
}


void UMinesweeperGridCanvas::UpdateCanvas(UCanvas* InCanvas, const int32 InWidth, const int32 InHeight)
{
	if (!InCanvas || !Game) return;
	if (!ClosedCellTexture || !FlagTexture || !MineTexture || !OpenCellTexture) return;


	auto DrawCell = [&](const FVector2D& InPosition, const UTexture2D* InTexture, FVector2D InSize = FVector2D::ZeroVector, const FLinearColor InColor = FLinearColor(1.0f, 1.0f, 1.0f))
	{
		if (InSize == FVector2D::ZeroVector) InSize = FVector2D(CellDrawSize);
		FCanvasTileItem canvasTileItem(InPosition, InTexture->GetResource(), InSize, InColor);
		canvasTileItem.BlendMode = SE_BLEND_Translucent;
		InCanvas->DrawItem(canvasTileItem);
	};


	// draw the minesweeper grid
	Game->ForEachCell([&](const TSharedPtr<FMinesweeperCell> InCell, const int32 InCellIndex, const FVector2D InCellCoord)
		{
			const FVector2D cellPosition = InCellCoord * CellDrawSize;


			// draw open/closed cell background
			{
				UTexture2D* backgroundTexture = ClosedCellTexture;
				if (InCell->bIsOpened)
				{
					backgroundTexture = InCell->bHasMine ? OpenCellMineTexture : OpenCellTexture;
				}
				DrawCell(cellPosition, backgroundTexture);
			}


			// draw neighbor mine count text
#ifdef DEFINE_DEBUG_MINES
			const bool drawNeighborMineCount = true;
#else
			const bool drawNeighborMineCount = InCell->bIsOpened && !InCell->bHasMine && InCell->NeighborMineCount > 0;
#endif
			if (CellFont && drawNeighborMineCount)
			{
				FString neighborMineCountStr = FString::FromInt(InCell->NeighborMineCount);
				FText neighborMineCountText = FText::FromString(neighborMineCountStr);
				
				float outWidth, outHeight;
				CellFont->GetCharSize(neighborMineCountStr[0], outWidth, outHeight);
				int32 textWidth = CellFont->GetStringSize(*neighborMineCountStr);

				const float percentOfCellSize = 0.8f;

				FVector2D textPosition = cellPosition + FVector2D((outWidth * 0.5f) * percentOfCellSize, (outHeight * 0.5f) * 0.2f);
				
				float scale = (CellDrawSize / outHeight) * percentOfCellSize;

				FCanvasTextItem textItem(textPosition, neighborMineCountText, CellFont, GetNeighborMineCountColor(InCell->NeighborMineCount).GetSpecifiedColor());
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
				DrawCell(cellPosition, MineTexture);
			}


			// draw flag
			if (!InCell->bIsOpened && InCell->bIsFlagged)
			{
				DrawCell(cellPosition, FlagTexture);
			}


			// draw hover cell outline
			if (HoverCellIndex > -1 && InCellIndex == HoverCellIndex)
			{
				DrawCell(cellPosition, HoverCellTexture, FVector2D::ZeroVector, InCell->bIsOpened ? HoverCellInvalidColor : HoverCellValidColor);
			}
		});
}


FSlateColor UMinesweeperGridCanvas::DefaultNeighborMineCountColor(const int32 InMineCount)
{
	switch (InMineCount)
	{
	case 1: return FLinearColor(0.0f, 0.0f, 1.0f);
	case 2: return FLinearColor(0.0f, 1.0f, 0.0f);
	case 3: return FLinearColor(1.0f, 0.0f, 0.0f);
	case 4: return FLinearColor(0.0f, 0.0f, 0.75f);
	case 5: return FLinearColor(0.0f, 0.75f, 0.0f);
	case 6: return FLinearColor(0.75f, 0.0f, 0.0f);
	case 7: return FLinearColor(0.0f, 0.0f, 0.5f);
	case 8: return FLinearColor(0.0f, 0.5f, 0.0f);
	}
	return FLinearColor::White;
}
