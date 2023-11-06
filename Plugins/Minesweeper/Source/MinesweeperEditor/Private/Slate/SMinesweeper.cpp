// Copyright 2022 Brad Monahan. All Rights Reserved.
#include "Slate/SMinesweeper.h"
#include "MinesweeperEditorModule.h"
#include "MinesweeperStyle.h"
#include "MinesweeperSettings.h"
#include "Slate/SMinesweeperGrid.h"
#include "MinesweeperGame.h"
#include "MinesweeperStatics.h"
#include "Editor.h"
#include "SlateOptMacros.h"
#include "Widgets/Images/SImage.h"


#define LOCTEXT_NAMESPACE "Minesweeper"




BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMinesweeper::Construct(const FArguments& InArgs)
{
	SetCanTick(false);


	OnGameSetupClick = InArgs._OnGameSetupClick;
	OnGameOver = InArgs._OnGameOver;


	Game = TStrongObjectPtr<UMinesweeperGame>(NewObject<UMinesweeperGame>(GetTransientPackage()));


	UMinesweeperSettings* settings = UMinesweeperSettings::Get();
	settings->OnVisualThemeChanged.AddSP(this, &SMinesweeper::OnVisualThemeChanged);


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
					SAssignNew(GridWidget, SMinesweeperGrid)
					.VisualTheme(settings->VisualTheme)
					.OnCellLeftClick(this, &SMinesweeper::OnCellLeftClick)
					.OnCellRightClick(this, &SMinesweeper::OnCellRightClick)
					.OnCellHoverChange(this, &SMinesweeper::OnHoverCellChange)
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


void SMinesweeper::OnVisualThemeChanged(const FPropertyChangedEvent& InPropertyChangedEvent)
{
	if (!VisualThemeChangeTimerHandle.IsValid())
	{
		const UMinesweeperSettings* settings = UMinesweeperSettings::GetConst();
		GridWidget->SetupGridCanvas(Game.Get(), settings->VisualTheme);
	}

	// disable resizing for one second to throttle sizing from settings
	GEditor->GetTimerManager()->SetTimer(VisualThemeChangeTimerHandle, [&]() { VisualThemeChangeTimerHandle.Invalidate(); }, 0.0f, false, 1.0f);
}


bool SMinesweeper::IsGameActive() const
{
	return Game->IsGameActive();
}

void SMinesweeper::StartNewGame(const FMinesweeperDifficulty& InDifficulty)
{
	Game->SetupGame(InDifficulty);

	const UMinesweeperSettings* settings = UMinesweeperSettings::GetConst();
	GridWidget->SetupGridCanvas(Game.Get(), settings->VisualTheme);
}

void SMinesweeper::RestartGame()
{
	Game->RestartGame();
	GridWidget->UpdateResource();
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


void SMinesweeper::OnCellLeftClick(const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition)
{
	if (!Game.IsValid() || !GridWidget.IsValid()) return;

	Game->TryOpenCell(InCellX, InCellY);

	GridWidget->UpdateResource();
}

void SMinesweeper::OnCellRightClick(const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition)
{
	if (!Game.IsValid() || !GridWidget.IsValid()) return;

	Game->TryFlagCell(InCellX, InCellY);

	GridWidget->UpdateResource();
}

void SMinesweeper::OnHoverCellChange(const bool InIsHovered, const int32 InCellX, const int32 InCellY, const FVector2D& InGridPosition)
{
	if (!Game.IsValid() || !GridWidget.IsValid()) return;

	if (InIsHovered)
		GridWidget->SetHoverCellCoord(FIntVector2(InCellX, InCellY));
	else
		GridWidget->ClearHoverCell();

	GridWidget->UpdateResource();
}




#undef LOCTEXT_NAMESPACE
