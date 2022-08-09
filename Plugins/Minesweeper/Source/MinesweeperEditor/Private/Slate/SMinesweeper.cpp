// Copyright 2022 Brad Monahan. All Rights Reserved.
#include "Slate/SMinesweeper.h"
#include "MinesweeperEditorModule.h"
#include "MinesweeperStyle.h"
#include "MinesweeperSettings.h"
#include "Slate/SMinesweeperGrid.h"
#include "MinesweeperGame.h"
#include "MinesweeperGridCanvas.h"
#include "MinesweeperBlueprintLib.h"
#include "Editor.h"
#include "SlateOptMacros.h"
#include "Widgets/Images/SImage.h"


#define LOCTEXT_NAMESPACE "SMinesweeper"




BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMinesweeper::Construct(const FArguments& InArgs)
{
	SetCanTick(false);


	OnGameSetupClick = InArgs._OnGameSetupClick;
	OnGameOver = InArgs._OnGameOver;


	Game = TStrongObjectPtr<UMinesweeperGame>(NewObject<UMinesweeperGame>(GetTransientPackage()));
	SetCellDrawSize(InArgs._CellDrawSize);


	// main window widget layout
	ChildSlot
	[
		SNew(SVerticalBox)
		    
		// HEADER PANEL
		+ SVerticalBox::Slot().AutoHeight()
		.HAlign(HAlign_Fill).VAlign(VAlign_Center)
		.Padding(1.0f)
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill).VAlign(VAlign_Center)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				ConstructHeaderRow(InArgs._PlayerName)
			]
		]

		// GAME GRID PANEL
		+ SVerticalBox::Slot().AutoHeight()
		.HAlign(HAlign_Center).VAlign(VAlign_Top)
		.Padding(3.0f, 3.0f, 3.0f, 3.0f)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SBorder)
				[
					SNew(SMinesweeperGrid)
					.GridCanvasBrush(&GridCanvasBrush)
					.OnCellLeftClick(this, &SMinesweeper::OnCellLeftClick)
					.OnCellRightClick(this, &SMinesweeper::OnCellRightClick)
					.OnHoverCellChanged(this, &SMinesweeper::OnHoverCellChanged)
				]
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill).VAlign(VAlign_Center)
				.Visibility(this, &SMinesweeper::GetWinLoseVisibility)
				.BorderBackgroundColor(this, &SMinesweeper::GetWinLoseColor)
				//.BorderImage([&]() { return FMinesweeperStyle::GetBrush(HasWon() ? "Border.Win" : "Border.Lose"); })
				[
					ConstructEndGameOverlayPanel()
				]
			]
		]
	];
}

TSharedRef<SHorizontalBox> SMinesweeper::ConstructHeaderRow(TAttribute<FText> InPlayerName)
{
	return 
		SNew(SHorizontalBox)
		// NAME AND TIME PANEL
		+ SHorizontalBox::Slot().FillWidth(1.0f)
		.HAlign(HAlign_Left).VAlign(VAlign_Bottom)
		.Padding(3.0f)
		[
			SNew(SBox).WidthOverride(130)
			.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				ConstructPlayerNameAndGameTimePanel(InPlayerName)
			]
		]
		// MR SMILE BUTTON
		+ SHorizontalBox::Slot().FillWidth(1.0f)
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			//SNew(SBox).WidthOverride(56).WidthOverride(56)
			//.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			//[
				SNew(SButton)
				.ContentPadding(0)
				.HAlign(HAlign_Center).VAlign(VAlign_Center)
				.OnClicked(this, &SMinesweeper::OnRestartGameButtonClick)
				[
					SNew(SImage)
					.DesiredSizeOverride(FVector2D(56, 56))
					.ToolTipText(LOCTEXT("RestartGameTooltip", "Start a new game with the same settings."))
					.Image(this, &SMinesweeper::GetSmileImage)
				]
			//]
		]
		// GAME SETUP AND FLAGS REMAINING
		+ SHorizontalBox::Slot().AutoWidth()
		.HAlign(HAlign_Right).VAlign(VAlign_Fill)
		.Padding(3.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight()
			.HAlign(HAlign_Fill).VAlign(VAlign_Top)
			.Padding(0.0f, 0.0f, 0.0f, 2.0f)
			[
				SNew(SButton)
				.ContentPadding(0)
				.HAlign(HAlign_Center).VAlign(VAlign_Center)
				.ToolTipText(LOCTEXT("NewGameTooltip", "Start a new game with new game settings."))
				.OnClicked(this, &SMinesweeper::OnGameSetupButtonClick)
				[
					SNew(STextBlock)
					.Margin(FMargin(0, 1))
					.Justification(ETextJustify::Center)
					.Visibility(EVisibility::SelfHitTestInvisible)
					.TextStyle(FMinesweeperStyle::Get(), "Text.Small")
					.Text(LOCTEXT("GameSetupLabel", "Game Setup"))
				]
			]
			+ SVerticalBox::Slot().AutoHeight()
			.HAlign(HAlign_Right).VAlign(VAlign_Bottom)
			[
				SNew(SBox).WidthOverride(80)
				.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
				[
					ConstructFlagsRemainingPanel()
				]
			]
		];
}

TSharedRef<SVerticalBox> SMinesweeper::ConstructPlayerNameAndGameTimePanel(TAttribute<FText> InPlayerName)
{
	return 
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight()
		.HAlign(HAlign_Fill).VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 0.0f, 2.0f)
		[
			SNew(STextBlock)
			.Justification(ETextJustify::Center)
			.TextStyle(FMinesweeperStyle::Get(), "Text.Normal.Bold")
			.Text(InPlayerName)
		]
		+ SVerticalBox::Slot().AutoHeight()
		.HAlign(HAlign_Fill).VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill).VAlign(VAlign_Center)
			.BorderImage(FMinesweeperStyle::GetBrush("GameHeaderTimeBorder"))
			.ToolTipText(LOCTEXT("TimeElapsedTooltip", "Elapsed game time in seconds."))
			[
				SNew(STextBlock)
				.Margin(FMargin(0, 0, 0, -2))
				.Justification(ETextJustify::Center)
				.Visibility(EVisibility::SelfHitTestInvisible)
				.TextStyle(FMinesweeperStyle::Get(), "Text.HeaderLarge")
				.Text(this, &SMinesweeper::GetTimerText)
			]
		];
}

TSharedRef<SVerticalBox> SMinesweeper::ConstructFlagsRemainingPanel()
{
	return 
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight()
		.HAlign(HAlign_Fill).VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 0.0f, 0.0f)
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill).VAlign(VAlign_Center)
			.BorderImage(FMinesweeperStyle::GetBrush("GameHeaderTimeBorder"))
			.ToolTipText(LOCTEXT("FlagsRemainingTooltip", "Number of flags remaining that can be placed."))
			[
				SNew(STextBlock)
				.Margin(FMargin(0, 0, 0, -2))
				.Justification(ETextJustify::Center)
				.Visibility(EVisibility::SelfHitTestInvisible)
				.TextStyle(FMinesweeperStyle::Get(), "Text.HeaderLarge")
				.Text(this, &SMinesweeper::GetFlagsRemainingText)
			]
		];
}

TSharedRef<SVerticalBox> SMinesweeper::ConstructEndGameOverlayPanel()
{
	return 
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight()
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		.Padding(0, 0,0, 20)
		[
			SNew(STextBlock)
			.TextStyle(FMinesweeperStyle::Get(), "Text.WinLose")
			.ColorAndOpacity(this, &SMinesweeper::GetWinLoseColor)
			.Text(this, &SMinesweeper::GetWinLoseText)
		]
		+ SVerticalBox::Slot().AutoHeight()
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		.Padding(0, 0, 0, 10)
		[
			SNew(STextBlock)
			.Visibility(this, &SMinesweeper::GetHighScoreRankVisibility)
			.TextStyle(FMinesweeperStyle::Get(), "Text.WinLose")
			.Text(LOCTEXT("NewHighScoreLabel", "New High Score!"))
		]
		+ SVerticalBox::Slot().AutoHeight()
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Visibility(this, &SMinesweeper::GetHighScoreRankVisibility)
			.TextStyle(FMinesweeperStyle::Get(), "Text.WinLose")
			.Text(this, &SMinesweeper::GetHighScoreRankText)
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


bool SMinesweeper::IsGameActive() const
{
	return Game->IsGameActive();
}

void SMinesweeper::StartNewGame(const FMinesweeperDifficulty& InDifficulty)
{
	Game->SetupGame(InDifficulty);
	SetGridSize(InDifficulty.GridSize());
}

void SMinesweeper::RestartGame()
{
	Game->RestartGame();
	GridCanvas->UpdateResource();
}

void SMinesweeper::PauseGame()
{
	Game->PauseGame();
}

void SMinesweeper::ContinueGame()
{
	Game->ResumeGame();
}


FText SMinesweeper::GetTimerText() const
{
	return IsGameActive() ? FText::FromString(FString::FromInt(FMath::FloorToInt32(Game->GetGameTime()))) : FText();
}

const FSlateBrush* SMinesweeper::GetSmileImage() const
{
	return FMinesweeperStyle::GetBrush((Game->IsGameActive() || Game->GetGameTime() == 0.0f || Game->HasWon()) ? "MrSmile.Alive" : "MrSmile.Dead");
}

FText SMinesweeper::GetFlagsRemainingText() const
{
	return IsGameActive() ? FText::FromString(FString::FromInt(Game->GetFlagsRemaining())) : FText();
}

EVisibility SMinesweeper::GetWinLoseVisibility() const
{
	return (!Game->IsGameActive() && Game->GetGameTime() > 0.0f) ? EVisibility::SelfHitTestInvisible : EVisibility::Hidden;
}

FSlateColor SMinesweeper::GetWinLoseColor() const
{
	return FMinesweeperStyle::GetColor(Game->HasWon() ? "Color.Win" : "Color.Lose");
}

FText SMinesweeper::GetWinLoseText() const
{
	return Game->HasWon() ? LOCTEXT("GameWinnerLabel", "Winner!") : LOCTEXT("GameLoserLabel", "You Lose!");
}

EVisibility SMinesweeper::GetHighScoreRankVisibility() const
{
	return LastHighScoreRank > -1 ? EVisibility::SelfHitTestInvisible : EVisibility::Hidden;
}

FText SMinesweeper::GetHighScoreRankText() const
{
	return FText::Format(LOCTEXT("NewHighScoreRankLabel", "Rank {0}"), FText::FromString(FString::FromInt(LastHighScoreRank + 1)));
}


FReply SMinesweeper::OnRestartGameButtonClick()
{
	RestartGame();

	return FReply::Handled();
}

FReply SMinesweeper::OnGameSetupButtonClick()
{
	OnGameSetupClick.ExecuteIfBound();

	return FReply::Handled();
}


void SMinesweeper::SetGridSize(const FIntVector2& InGridSize, const float InNewCellDrawSize)
{
	const float cellDrawSize = FMath::Clamp(InNewCellDrawSize > -1.0f ? InNewCellDrawSize : GetCellDrawSize(), 10.0f, 64.0f);
	FVector2D gridCanvasSize(InGridSize.X * cellDrawSize, InGridSize.Y * cellDrawSize);

	if (!GridCanvas.IsValid())
	{
		GridCanvas = TStrongObjectPtr<UMinesweeperGridCanvas>(
			UMinesweeperBlueprintLib::CreateMinesweeperGridCanvas(GetTransientPackage(), Game.Get(), cellDrawSize)
		);

		UMinesweeperSettings* settings = UMinesweeperSettings::Get();

		GridCanvas->SetClosedCellTexture(Cast<UTexture2D>(settings->ClosedCellTexture.TryLoad()));
		GridCanvas->SetOpenCellTexture(Cast<UTexture2D>(settings->OpenCellTexture.TryLoad()));
		GridCanvas->SetOpenCellMineTexture(Cast<UTexture2D>(settings->OpenCellMineTexture.TryLoad()));
		GridCanvas->SetMineTexture(Cast<UTexture2D>(settings->MineTexture.TryLoad()));
		GridCanvas->SetFlagTexture(Cast<UTexture2D>(settings->FlagTexture.TryLoad()));
		GridCanvas->SetHoverCellTexture(Cast<UTexture2D>(settings->HoverCellTexture.TryLoad()));
		GridCanvas->SetCellFont((UFont*)(settings->CellFont.TryLoad()));

		GridCanvasBrush.SetResourceObject(GridCanvas.Get());
		GridCanvasBrush.TintColor = FLinearColor::White;
	}
	else
	{
		GridCanvas->ResizeTarget(gridCanvasSize.X, gridCanvasSize.Y);
	}

	GridCanvasBrush.SetImageSize(gridCanvasSize);

	GridCanvas->InitCanvas(Game.Get(), cellDrawSize);
}

float SMinesweeper::GetCellDrawSize() const
{
	return GridCanvas.IsValid() ? GridCanvas->GetCellDrawSize() : UMinesweeperGridCanvas::DefaultCellDrawSize();
}

void SMinesweeper::SetCellDrawSize(const float InCellDrawSize)
{
	// texture needs to be resized when cell draw size is changed
	SetGridSize(Game->GetDifficulty().GridSize(), InCellDrawSize);
}


void SMinesweeper::OnCellLeftClick(const FVector2D& InGridPosition)
{
	if (!GridCanvas.IsValid()) return;

	FIntVector2 cellCoord;
	GridCanvas->GridPositionToCellCoord(InGridPosition, cellCoord.X, cellCoord.Y);

	Game->TryOpenCell(cellCoord.X, cellCoord.Y);

	GridCanvas->UpdateResource();
}

void SMinesweeper::OnCellRightClick(const FVector2D& InGridPosition)
{
	if (!GridCanvas.IsValid()) return;

	FIntVector2 cellCoord;
	GridCanvas->GridPositionToCellCoord(InGridPosition, cellCoord.X, cellCoord.Y);

	Game->TryFlagCall(cellCoord.X, cellCoord.Y);

	GridCanvas->UpdateResource();
}

void SMinesweeper::OnHoverCellChanged(const bool InIsHovered, const FVector2D& InGridPosition)
{
	if (!GridCanvas.IsValid()) return;

	if (InIsHovered)
		GridCanvas->SetHoverCellIndex(GridCanvas->GridPositionToCellIndex(InGridPosition));
	else
		GridCanvas->ClearHoverCell();

	GridCanvas->UpdateResource();
}




#undef LOCTEXT_NAMESPACE
