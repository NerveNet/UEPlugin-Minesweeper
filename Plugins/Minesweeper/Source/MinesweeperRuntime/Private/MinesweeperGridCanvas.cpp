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
	ClosedCellTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Minesweeper/ClosedCell_32x"));
	OpenCellTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Minesweeper/OpenCell_32x"));
	OpenCellMineTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Minesweeper/OpenCell_Mine_32x"));
	MineTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Minesweeper/Mine_32x"));
	FlagTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Minesweeper/Flag_32x"));
	CellFont = LoadObject<UFont>(nullptr, TEXT("/Minesweeper/CellFont"));

	OnCanvasRenderTargetUpdate.AddDynamic(this, &UMinesweeperGridCanvas::UpdateCanvas);
}


void UMinesweeperGridCanvas::InitCanvas(UMinesweeperGame* InGame, const float InCellDrawSize)
{
	Game = InGame;

	CellDrawSize = InCellDrawSize;

	UpdateResource();
}


int32 UMinesweeperGridCanvas::GridPositionToGridIndex(UPARAM(ref) const FVector2D& InGridPosition) const
{
	return Game == nullptr ? -1 : Game->GridCoordToIndex(FIntVector2(InGridPosition.X / CellDrawSize, InGridPosition.Y / CellDrawSize));
}


FSlateColor UMinesweeperGridCanvas::GetNeighborMineCountColor_Implementation(const int32 InMineCount)
{
	return DefaultNeighborMineCountColor(InMineCount);
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

void UMinesweeperGridCanvas::SetHoverCellIndex(const int32 CellIndex)
{

}


void UMinesweeperGridCanvas::UpdateCanvas(UCanvas* InCanvas, const int32 InWidth, const int32 InHeight)
{
	if (!InCanvas || !Game) return;
	if (!ClosedCellTexture || !FlagTexture || !MineTexture || !OpenCellTexture) return;


	auto DrawCell = [&](const FVector2D& InPosition, const UTexture2D* InTexture, FVector2D InSize = FVector2D::ZeroVector, const FLinearColor InColor = FLinearColor(1.0f, 1.0f, 1.0f))
	{
		if (InSize == FVector2D::ZeroVector)
		{
			InSize = FVector2D(CellDrawSize);
		}
		FCanvasTileItem canvasTileItem(InPosition, InTexture->GetResource(), InSize, InColor);
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
				
				UFont* font = GEngine->GetLargeFont();
				float outWidth, outHeight;
				font->GetCharSize(neighborMineCountStr[0], outWidth, outHeight);
				int32 textWidth = font->GetStringSize(*neighborMineCountStr);
				
				FVector2D textPosition(InCellCoord.X * CellDrawSize, InCellCoord.Y * CellDrawSize);
				//textPosition.X += (CellDrawSize - textWidth) * 0.5f;
				//textPosition.Y += (CellDrawSize - outHeight) * 0.5f;
				//float textXPos = (CellDrawSize - textWidth) * 0.5f;
				//float textYPos = (CellDrawSize - outHeight) * 0.5f;

				float scale = (CellDrawSize / outHeight) * 0.8f;

				FCanvasTextItem textItem(textPosition, neighborMineCountText, font, GetNeighborMineCountColor(InCell->NeighborMineCount).GetSpecifiedColor());
				textItem.Scale = FVector2D(scale);
				textItem.BlendMode = SE_BLEND_Translucent;
				InCanvas->DrawItem(textItem);
			}

			
			// draw mine
#ifdef DEFINE_DEBUG_MINES
			const bool drawMine = InCell.bHasMine;
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
		});
}

