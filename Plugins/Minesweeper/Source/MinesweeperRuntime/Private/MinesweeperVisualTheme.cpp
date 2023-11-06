// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperVisualTheme.h"
#include "MinesweeperStatics.h"
#include "UObject/ConstructorHelpers.h"


#define LOCTEXT_NAMESPACE "Minesweeper"




void FMinesweeperVisualTheme::CopyIfNotNull(const FMinesweeperVisualTheme& CopyFrom)
{
	CellDrawSize = UMinesweeperStatics::ClampCellDrawSize(CopyFrom.CellDrawSize);
	CopyObjectsIfNotNull(CopyFrom);
	HoverCellValidColor = CopyFrom.HoverCellValidColor;
	HoverCellInvalidColor = CopyFrom.HoverCellInvalidColor;
}

void FMinesweeperVisualTheme::CopyObjectsIfNotNull(const FMinesweeperVisualTheme& CopyFrom)
{
	if (CopyFrom.ClosedCellTexture) ClosedCellTexture = CopyFrom.ClosedCellTexture;
	if (CopyFrom.OpenCellTexture) OpenCellTexture = CopyFrom.OpenCellTexture;
	if (CopyFrom.OpenCellMineTexture) OpenCellMineTexture = CopyFrom.OpenCellMineTexture;
	if (CopyFrom.MineTexture) MineTexture = CopyFrom.MineTexture;
	if (CopyFrom.FlagTexture) FlagTexture = CopyFrom.FlagTexture;
	if (CopyFrom.HoverCellTexture) HoverCellTexture = CopyFrom.HoverCellTexture;
	if (CopyFrom.CellFont) CellFont = CopyFrom.CellFont;
}





#undef LOCTEXT_NAMESPACE
