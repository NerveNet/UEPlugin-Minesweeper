// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperGrid.h"
#include "Slate/SMinesweeperGrid.h"
#include "MinesweeperStatics.h"
#include "MinesweeperVisualTheme.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Images/SImage.h"


#define LOCTEXT_NAMESPACE "Minesweeper"




UMinesweeperGrid::UMinesweeperGrid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	VisualTheme.CopyIfNotNull(UMinesweeperStatics::DefaultVisualTheme());

#if WITH_EDITOR
	OpenCellBrush.SetResourceObject(VisualTheme.OpenCellTexture);
	MineBrush.SetResourceObject(VisualTheme.MineTexture);
#endif
}


void UMinesweeperGrid::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyGrid.Reset();
}

#if WITH_EDITOR
const FText UMinesweeperGrid::GetPaletteCategory()
{
	return LOCTEXT("Misc", "Misc");
}

TSharedRef<SWidget> UMinesweeperGrid::RebuildDesignWidget(TSharedRef<SWidget> InContent)
{
	return 
		SNew(SBorder)
		[
			SNew(SBox).HAlign(HAlign_Center).VAlign(VAlign_Center).Padding(5.0f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().HAlign(HAlign_Center).Padding(0.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("MinesweeperEditorContentTitleLabel", "Minesweeper Grid"))
				]
				+ SVerticalBox::Slot().HAlign(HAlign_Center).Padding(0.0f, 5.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.TextStyle(FEditorStyle::Get(), "SmallText")
					.Text_Lambda([&]() { return FText::Format(LOCTEXT("MinesweeperEditorContentCellDrawSizeLabel", "Cell Draw Size: {0}"), FText::FromString(FString::FromInt(VisualTheme.CellDrawSize))); })
				]
				+ SVerticalBox::Slot().HAlign(HAlign_Center).Padding(0.0f, 5.0f, 0.0f, 0.0f)
				[
					SNew(SBorder)
					[
						SNew(SOverlay)
						+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
						[
							SNew(SBox)
							.WidthOverride_Lambda([&]() { return VisualTheme.CellDrawSize; })
							.HeightOverride_Lambda([&]() { return VisualTheme.CellDrawSize; })
							.HAlign(HAlign_Center).VAlign(VAlign_Fill)
							[
								SNew(SImage)
								.DesiredSizeOverride_Lambda([&]() { return FVector2D(VisualTheme.CellDrawSize); })
								.Image(&OpenCellBrush)
							]
						]
						+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
						[
							SNew(SBox)
							.WidthOverride_Lambda([&]() { return VisualTheme.CellDrawSize; })
							.HeightOverride_Lambda([&]() { return VisualTheme.CellDrawSize; })
							.HAlign(HAlign_Center).VAlign(VAlign_Fill)
							[
								SNew(SImage)
								.DesiredSizeOverride_Lambda([&]() { return FVector2D(VisualTheme.CellDrawSize); })
								.Image(&MineBrush)
							]
						]
					]
				]
			]
		];
}
#endif

TSharedRef<SWidget> UMinesweeperGrid::RebuildWidget()
{
	return
		SAssignNew(MyGrid, SMinesweeperGrid)
		.VisualTheme(VisualTheme)
		.OnCellLeftClick(BIND_UOBJECT_DELEGATE(FMinesweeperGridCellClickDelegate, SlateHandleCellLeftClick))
		.OnCellRightClick(BIND_UOBJECT_DELEGATE(FMinesweeperGridCellClickDelegate, SlateHandleCellRightClick))
		.OnCellHoverChange(BIND_UOBJECT_DELEGATE(FMinesweeperGridCellHoverDelegate, SlateHandleCellHoverChange));
}

void UMinesweeperGrid::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyGrid->SetVisualTheme(VisualTheme);
}


UMinesweeperGame* UMinesweeperGrid::GetGame() const
{
	if (!MyGrid.IsValid()) return nullptr;

	UMinesweeperGridCanvas* gridCanvas = MyGrid->GetGridCanvas();

	return gridCanvas ? gridCanvas->GetGame() : nullptr;
}

void UMinesweeperGrid::SetupGridCanvas(UMinesweeperGame* InGame, const FMinesweeperVisualTheme& InVisualTheme)
{
	MyGrid->SetupGridCanvas(InGame, VisualTheme);
}


void UMinesweeperGrid::SetVisualTheme(const FMinesweeperVisualTheme& InVisualTheme)
{
	VisualTheme.CopyIfNotNull(InVisualTheme);
	MyGrid->SetVisualTheme(VisualTheme);
}

void UMinesweeperGrid::SetCellDrawSize(const float InCellDrawSize)
{
	MyGrid->SetCellDrawSize(InCellDrawSize);
	VisualTheme.CellDrawSize = MyGrid->GetCellDrawSize();
}


void UMinesweeperGrid::UpdateResource()
{
	MyGrid->UpdateResource();
}


void UMinesweeperGrid::ClearHoverCell()
{
	MyGrid->ClearHoverCell();
}

void UMinesweeperGrid::SetHoverCellIndex(const int32 InCellIndex)
{
	MyGrid->SetHoverCellIndex(InCellIndex);
}

void UMinesweeperGrid::SetHoverCellCoord(const int32 InCellX, const int32 InCellY)
{
	MyGrid->SetHoverCellCoord(FIntVector2(InCellX, InCellY));
}


void UMinesweeperGrid::SlateHandleCellLeftClick(const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition)
{
	OnCellLeftClick.Broadcast(InCellX, InCellY, InGridPosition);
}

void UMinesweeperGrid::SlateHandleCellRightClick(const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition)
{
	OnCellRightClick.Broadcast(InCellX, InCellY, InGridPosition);
}

void UMinesweeperGrid::SlateHandleCellHoverChange(const bool InIsHovered, const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition)
{
	OnCellHover.Broadcast(InIsHovered, InCellX, InCellY, InGridPosition);
}




#undef LOCTEXT_NAMESPACE
