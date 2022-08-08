// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperBlueprintLib.h"
#include "MinesweeperGame.h"
#include "MinesweeperGridCanvas.h"




UMinesweeperGridCanvas* UMinesweeperBlueprintLib::CreateMinesweeperGridCanvas(UObject* InWorldContextObject, UMinesweeperGame* InGame, const float InCellDrawSize)
{
	if (!InGame) return nullptr;

	const FIntVector2 gridSize = InGame->GetDifficulty().GridSize();
	const float cellDrawSize = FMath::Clamp(InCellDrawSize, 10.0f, 64.0f);
	const FVector2D gridCanvasSize(gridSize.X * cellDrawSize, gridSize.Y * cellDrawSize);

	UMinesweeperGridCanvas* gridCanvas = CastChecked<UMinesweeperGridCanvas>(
		UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(
			GetTransientPackage(),
			UMinesweeperGridCanvas::StaticClass(),
			gridCanvasSize.X, gridCanvasSize.Y
		)
	);

	gridCanvas->InitCanvas(InGame, cellDrawSize);

	return gridCanvas;
}


FMinesweeperDifficulty UMinesweeperBlueprintLib::BeginnerDifficulty()
{
	return FMinesweeperDifficulty::Beginner();
}

FMinesweeperDifficulty UMinesweeperBlueprintLib::IntermediateDifficulty()
{
	return FMinesweeperDifficulty::Intermediate();
}

FMinesweeperDifficulty UMinesweeperBlueprintLib::ExpertDifficulty()
{
	return FMinesweeperDifficulty::Expert();
}


bool UMinesweeperBlueprintLib::IsBeginnerDifficulty(const FMinesweeperDifficulty& InDifficulty)
{
	return InDifficulty.IsBeginner();
}

bool UMinesweeperBlueprintLib::IsIntermediateDifficulty(const FMinesweeperDifficulty& InDifficulty)
{
	return InDifficulty.IsIntermediate();
}

bool UMinesweeperBlueprintLib::IsExpertDifficulty(const FMinesweeperDifficulty& InDifficulty)
{
	return InDifficulty.IsExpert();
}

