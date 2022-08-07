// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "MinesweeperGridCanvas.h"

class SUniformGridPanel;
class UMinesweeperGame;
struct FMinesweeperCell;




/**
 * 
 */
class SMinesweeperGrid : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperGrid)
		: _CellDrawSize(28)
	{ }

		SLATE_ARGUMENT(int32, CellDrawSize)

	SLATE_END_ARGS()


	SMinesweeperGrid();

	void Construct(const FArguments& InArgs, TStrongObjectPtr<UMinesweeperGame> InGame);


	//~ Begin SWidget Overrides
	virtual FCursorReply OnCursorQuery(const FGeometry& InMyGeometry, const FPointerEvent& InCursorEvent) const override;
	virtual TOptional<TSharedRef<SWidget>> OnMapCursor(const FCursorReply& InCursorReply) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& InMyGeometry,const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	//~ End SWidget Overrides


	TStrongObjectPtr<UMinesweeperGame> GetGame() const { return Game; }


	void CreateGridCells();


private:
	int32 CellDrawSize;
	bool UseGridCanvas;

	TStrongObjectPtr<UMinesweeperGame> Game;


	/** The random seed used to generate the current grid mine layout. */
	int32 CurrentGridSeed = 0;


	/** Internal slate brush that holds the texture resource. */
	FSlateBrush GridCanvasBrush;

	/** Render target texture where the cell textures are drawn for each cell. */
	TStrongObjectPtr<UMinesweeperGridCanvas> GridCanvas;

	TSharedPtr<SImage> GridCanvasWidget;


};
