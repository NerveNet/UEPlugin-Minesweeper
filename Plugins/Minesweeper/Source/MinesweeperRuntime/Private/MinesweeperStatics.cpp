// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperStatics.h"
#include "MinesweeperGame.h"
#include "MinesweeperGridCanvas.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/Texture2D.h"


#define LOCTEXT_NAMESPACE "Minesweeper"




UTexture2D* UMinesweeperStatics::DefaultClosedCellTexture()
{
	return LoadObject<UTexture2D>(nullptr, DefaultClosedCellTexturePath());
}

UTexture2D* UMinesweeperStatics::DefaultOpenCellTexture()
{
	return LoadObject<UTexture2D>(nullptr, DefaultOpenCellTexturePath());
}

UTexture2D* UMinesweeperStatics::DefaultOpenCellMineTexture()
{
	return LoadObject<UTexture2D>(nullptr, DefaultOpenCellMineTexturePath());
}

UTexture2D* UMinesweeperStatics::DefaultMineTexture()
{
	return LoadObject<UTexture2D>(nullptr, DefaultMineTexturePath());
}

UTexture2D* UMinesweeperStatics::DefaultFlagTexture()
{
	return LoadObject<UTexture2D>(nullptr, DefaultFlagTexturePath());
}

UTexture2D* UMinesweeperStatics::DefaultHoverCellTexture()
{
	return LoadObject<UTexture2D>(nullptr, DefaultHoverCellTexturePath());
}

UFont* UMinesweeperStatics::DefaultCellFont()
{
	return LoadObject<UFont>(nullptr, DefaultCellFontPath());
}


float UMinesweeperStatics::ClampCellDrawSize(const int32 InCellDrawSize)
{
	return FMath::Clamp(InCellDrawSize, UMinesweeperStatics::MinCellDrawSize(), UMinesweeperStatics::MaxCellDrawSize());
}


FSlateColor UMinesweeperStatics::DefaultNeighborMineCountColor(const int32 InMineCount)
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


FMinesweeperVisualTheme UMinesweeperStatics::DefaultVisualTheme()
{
	FMinesweeperVisualTheme visualTheme;
	visualTheme.CellDrawSize = DefaultCellDrawSize();
	visualTheme.ClosedCellTexture = DefaultClosedCellTexture();
	visualTheme.OpenCellTexture = DefaultOpenCellTexture();
	visualTheme.OpenCellMineTexture = DefaultOpenCellMineTexture();
	visualTheme.MineTexture = DefaultMineTexture();
	visualTheme.FlagTexture = DefaultFlagTexture();
	visualTheme.HoverCellTexture = DefaultHoverCellTexture();
	visualTheme.CellFont = DefaultCellFont();
	visualTheme.HoverCellValidColor = DefaultHoverCellValidColor();
	visualTheme.HoverCellInvalidColor = DefaultHoverCellInvalidColor();
	return visualTheme;
}


UMinesweeperGridCanvas* UMinesweeperStatics::CreateMinesweeperGridCanvas(UObject* InWorldContextObject, UMinesweeperGame* InGame, const FMinesweeperVisualTheme& InVisualTheme)
{
	if (!InGame) return nullptr;

	const FIntVector2 gridSize = InGame->GetDifficulty().GridSize();
	const float cellDrawSize = FMath::Clamp(InVisualTheme.CellDrawSize, MinCellDrawSize(), MaxCellDrawSize());
	const FVector2D gridCanvasSize(gridSize.X * cellDrawSize, gridSize.Y * cellDrawSize);

	UCanvasRenderTarget2D* canvas = UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(
			GetTransientPackage(),
			UMinesweeperGridCanvas::StaticClass(),
			gridCanvasSize.X, gridCanvasSize.Y
		);
	if (!canvas) return nullptr;

	UMinesweeperGridCanvas* gridCanvas = CastChecked<UMinesweeperGridCanvas>(canvas);

	gridCanvas->InitCanvas(InGame, InVisualTheme);

	return gridCanvas;
}


bool UMinesweeperStatics::IsDifficultyValid(const FMinesweeperDifficulty& InDifficulty)
{
	FIntVector2 gridSize = InDifficulty.GridSize();
	return gridSize.X > 0 && gridSize.Y > 0 && gridSize.X <= UMinesweeperGame::MaxGridSize && gridSize.Y <= UMinesweeperGame::MaxGridSize && 
			InDifficulty.MineCount > 0 && InDifficulty.MineCount <= UMinesweeperGame::MaxMineCount;
}




#undef LOCTEXT_NAMESPACE
