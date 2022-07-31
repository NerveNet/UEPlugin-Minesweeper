// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "SMinesweeper.h"
#include "MinesweeperEditorModule.h"
#include "MinesweeperStyle.h"
#include "MinesweeperSettings.h"
#include "SMinesweeperCell.h"
#include "Editor.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SEditableTextBox.h"


#define LOCTEXT_NAMESPACE "SMinesweeper"




FMinesweeperDifficulty const SMinesweeper::DefaultDifficulty = FMinesweeperDifficulty::Beginner;


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMinesweeper::Construct(const FArguments& InArgs)
{
	SetCanTick(false);


	Settings = UMinesweeperSettings::Get();


	// main window widget layout
	const int32 difficultyButtonWidth = 100;

	ChildSlot
	[
		SNew(SWidgetSwitcher)
		.WidgetIndex_Lambda([&]() { return ActiveMainPanel; })

		// NEW GAME PANEL
		+ SWidgetSwitcher::Slot()
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Left).VAlign(VAlign_Top)
			.Padding(0.0f, 0.0f, 0.0f, 0.0f)
			[
				// SWITCH WINDOW MODE BUTTON
				SNew(SBox).WidthOverride(8).HeightOverride(60)
				.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
				[
					SNew(SButton)
					.ContentPadding(0)
					.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
					.ToolTipText(LOCTEXT("ToggleWindowModeTooltip", "Toggle between standalone and docking tab window modes."))
					.OnClicked(this, &SMinesweeper::OnSwitchWindowModeClick)
					/*[
						SNew(SBox).WidthOverride(20).HeightOverride(20)
						.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
						[
							SNew(SImage)
							.Visibility(EVisibility::SelfHitTestInvisible)
							.Image(FMinesweeperStyle::GetBrush("ToggleWindowMode"))
						]
					]*/
				]
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)

				// TITLE AREA
				+ SVerticalBox::Slot().AutoHeight()
				.HAlign(HAlign_Center).VAlign(VAlign_Center)
				.Padding(0.0f, 3.0f, 0.0f, 0.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().AutoWidth()
					.HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(SImage).Image(FMinesweeperStyle::GetBrush("Mine"))
					]
					+ SHorizontalBox::Slot().AutoWidth()
					.HAlign(HAlign_Center).VAlign(VAlign_Center)
					.Padding(20.0f, 0.0f)
					[
						SNew(STextBlock)
						.TextStyle(FMinesweeperStyle::Get(), "Text.Title")
						.Text(FMinesweeperEditorModule::GetMinesweeperLabel())
					]
					+ SHorizontalBox::Slot().AutoWidth()
					.HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(SImage).Image(FMinesweeperStyle::GetBrush("Mine"))
					]
				]

				// NEW GAME SETUP / HIGH SCORES BUTTONS
				+ SVerticalBox::Slot().AutoHeight()
				.HAlign(HAlign_Fill).VAlign(VAlign_Center)
				.Padding(5.0f, 10.0f, 5.0f, 0.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill).VAlign(VAlign_Center)
					.Padding(0, 0, 1, 0)
					[
						SNew(SBorder).BorderImage(FMinesweeperStyle::GetBrush("RoundedPanel"))
						[
							SNew(SCheckBox)
							.HAlign(HAlign_Center)
							.Style(FEditorStyle::Get(), "ToggleButtonCheckbox")
							.BorderBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f))
							.IsChecked_Lambda([&]() { return ActiveGameSetupPanel == 0 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
							.OnCheckStateChanged_Lambda([&](ECheckBoxState NewCheckState) { if (NewCheckState == ECheckBoxState::Checked) ActiveGameSetupPanel = 0; })
							[
								SNew(STextBlock)
								.Margin(FMargin(0, 2))
								.TextStyle(FMinesweeperStyle::Get(), "Text.Normal")
								.Text(LOCTEXT("PlayLabel", "Play"))
							]
						]
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill).VAlign(VAlign_Center)
					.Padding(1, 0, 0, 0)
					[
						SNew(SBorder).BorderImage(FMinesweeperStyle::GetBrush("RoundedPanel"))
						[
							SNew(SCheckBox)
							.HAlign(HAlign_Center)
							.Style(FEditorStyle::Get(), "ToggleButtonCheckbox")
							.IsChecked_Lambda([&]() { return ActiveGameSetupPanel == 1 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
							.OnCheckStateChanged_Lambda([&](ECheckBoxState NewCheckState) { if (NewCheckState == ECheckBoxState::Checked) ActiveGameSetupPanel = 1; })
							[
								SNew(STextBlock)
								.Margin(FMargin(0, 2))
								.TextStyle(FMinesweeperStyle::Get(), "Text.Normal")
								.Text(LOCTEXT("HighScoresLabel", "High Scores"))
							]
						]
					]
				]
				
				+ SVerticalBox::Slot().AutoHeight()
				.HAlign(HAlign_Fill).VAlign(VAlign_Center)
				.Padding(5.0f, 5.0f, 5.0f, 0.0f)
				[
					SNew(SBox)
					.WidthOverride(360).HeightOverride(270)
					[
						SNew(SWidgetSwitcher)
						.WidgetIndex_Lambda([&]() { return ActiveGameSetupPanel; })
					
						// NEW GAME SETTINGS
						+ SWidgetSwitcher::Slot()//.AutoWidth()
						.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
						.Padding(5.0f, 5.0f, 5.0f, 0.0f)
						[
							SNew(SBorder)
							//.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
							.BorderImage(FMinesweeperStyle::GetBrush("RoundedPanel"))
							.HAlign(HAlign_Fill).VAlign(VAlign_Center)
							.Padding(10.0f)
							[
								SNew(SVerticalBox)

								// GRID SETTINGS PANEL
								+ SVerticalBox::Slot().AutoHeight()
								.HAlign(HAlign_Fill).VAlign(VAlign_Center)
								.Padding(5.0f, 0.0f, 5.0f, 0.0f)
								[
									SNew(SBox).WidthOverride(300)
									[
										SNew(SVerticalBox)

										// USERNAME
										+ SVerticalBox::Slot().AutoHeight()
										.HAlign(HAlign_Fill).VAlign(VAlign_Center)
										.Padding(10.0f, 0.0f, 10.0f, 10.0f)
										[
											SNew(SHorizontalBox)
											+ SHorizontalBox::Slot().FillWidth(0.5f).VAlign(VAlign_Center)
											[
												SNew(STextBlock)
												.TextStyle(FMinesweeperStyle::Get(), "Text.Normal")
												.Text(LOCTEXT("PlayerNameLabel", "Player Name:"))
											]
											+ SHorizontalBox::Slot().FillWidth(0.5f).HAlign(HAlign_Fill).VAlign(VAlign_Center)
											[
												SAssignNew(NameTextBox, SEditableTextBox)
												.Text(FText::FromString(Settings->LastPlayerName))
												.OnTextChanged(this, &SMinesweeper::OnPlayerNameChanged)
												.OnTextCommitted(this, &SMinesweeper::OnPlayerNameCommitted)
											]
										]

										// NEW GAME GRID WIDTH
										+ SVerticalBox::Slot().AutoHeight()
										.HAlign(HAlign_Fill).VAlign(VAlign_Center)
										.Padding(10.0f, 0.0f, 10.0f, 5.0f)
										[
											SNew(SHorizontalBox)
											+ SHorizontalBox::Slot().FillWidth(0.5f).VAlign(VAlign_Center)
											[
												SNew(STextBlock)
												.TextStyle(FMinesweeperStyle::Get(), "Text.Normal")
												.Text(LOCTEXT("NewGridWidthLabel", "Width:"))
											]
											+ SHorizontalBox::Slot().FillWidth(0.5f).HAlign(HAlign_Fill).VAlign(VAlign_Center)
											[
												SNew(SNumericEntryBox<int32>)
												.AllowSpin(true)
												.MinSliderValue(MinGridSize).MaxSliderValue(MaxGridSize)
												.MinValue(MinGridSize).MaxValue(MaxGridSize)
												.Value_Lambda([&] { return Settings->LastSettings.Width; })
												.OnValueChanged_Lambda([&](int32 InNewValue)
													{
														Settings->LastSettings.Width = InNewValue;
														MaxMineCount = FMath::FloorToInt32(Settings->LastSettings.TotalCells() * 0.25f);
													})
											]
										]

										// NEW GAME GRID HEIGHT
										+ SVerticalBox::Slot().AutoHeight()
										.HAlign(HAlign_Fill).VAlign(VAlign_Center)
										.Padding(10.0f, 0.0f, 10.0f, 5.0f)
										[
											SNew(SHorizontalBox)
											+ SHorizontalBox::Slot().FillWidth(0.5f).VAlign(VAlign_Center)
											[
												SNew(STextBlock)
												.TextStyle(FMinesweeperStyle::Get(), "Text.Normal")
												.Text(LOCTEXT("NewGridHeightLabel", "Height:"))
											]
											+ SHorizontalBox::Slot().FillWidth(0.5f).HAlign(HAlign_Fill).VAlign(VAlign_Center)
											[
												SNew(SNumericEntryBox<int32>)
												.AllowSpin(true)
												.MinSliderValue(MinGridSize).MaxSliderValue(MaxGridSize)
												.MinValue(MinGridSize).MaxValue(MaxGridSize)
												.Value_Lambda([&] { return Settings->LastSettings.Height; })
												.OnValueChanged_Lambda([&](int32 InNewValue)
													{
														Settings->LastSettings.Height = InNewValue;
														MaxMineCount = FMath::FloorToInt32(Settings->LastSettings.TotalCells() * 0.25f);
													})
											]
										]

										// NEW GAME GRID MINE COUNT
										+ SVerticalBox::Slot().AutoHeight()
										.HAlign(HAlign_Fill).VAlign(VAlign_Center)
										.Padding(10.0f, 0.0f, 10.0f, 5.0f)
										[
											SNew(SHorizontalBox)
											+ SHorizontalBox::Slot().FillWidth(0.5f).VAlign(VAlign_Center)
											[
												SNew(STextBlock)
												.TextStyle(FMinesweeperStyle::Get(), "Text.Normal")
												.Text(LOCTEXT("NewGridMineCountLabel", "Mines:"))
											]
											+ SHorizontalBox::Slot().FillWidth(0.5f).HAlign(HAlign_Fill).VAlign(VAlign_Center)
											[
												SNew(SNumericEntryBox<int32>)
												.AllowSpin(true)
												.MinSliderValue(MinMineCount).MaxSliderValue(MaxMineCount)
												.MinValue(MinMineCount).MaxValue(MaxMineCount)
												.Value_Lambda([&] { return Settings->LastSettings.MineCount; })
												.OnValueChanged_Lambda([&](int32 InNewValue) { Settings->LastSettings.MineCount = InNewValue; })
											]
										]
									]
								]

								// DIFFICULTY SETTING
								+ SVerticalBox::Slot().AutoHeight()
								.HAlign(HAlign_Fill).VAlign(VAlign_Center)
								.Padding(0.0f, 5.0f, 0.0f, 10.0f)
								[
									SNew(SHorizontalBox)
									+ SHorizontalBox::Slot().FillWidth(1.0f)
									.HAlign(HAlign_Center).VAlign(VAlign_Center)
									.Padding(1.0f, 0.0f, 1.0f, 0.0f)
									[
										SNew(SBox).WidthOverride(difficultyButtonWidth)
										.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
										[
											SNew(SButton)
											.HAlign(HAlign_Center).VAlign(VAlign_Center)
											.ButtonColorAndOpacity(this, &SMinesweeper::GetDifficultyColor, 0)
											.OnClicked(this, &SMinesweeper::OnDifficultyClick, 0)
											[
												SNew(STextBlock)
												.TextStyle(FMinesweeperStyle::Get(), "Text.Small")
												.Text(LOCTEXT("BeginnerDifficultyLabel", "Beginner"))
											]
										]
									]
									+ SHorizontalBox::Slot().FillWidth(1.0f)
									.HAlign(HAlign_Center).VAlign(VAlign_Center)
									.Padding(1.0f, 0.0f, 1.0f, 0.0f)
									[
										SNew(SBox).WidthOverride(difficultyButtonWidth)
										.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
										[
											SNew(SButton)
											.HAlign(HAlign_Center).VAlign(VAlign_Center)
											.ButtonColorAndOpacity(this, &SMinesweeper::GetDifficultyColor, 1)
											.OnClicked(this, &SMinesweeper::OnDifficultyClick, 1)
											[
												SNew(STextBlock)
												.TextStyle(FMinesweeperStyle::Get(), "Text.Small")
												.Text(LOCTEXT("IntermediateDifficultyLabel", "Intermediate"))
											]
										]
									]
									+ SHorizontalBox::Slot().FillWidth(1.0f)
									.HAlign(HAlign_Center).VAlign(VAlign_Center)
									.Padding(1.0f, 0.0f, 1.0f, 0.0f)
									[
										SNew(SBox).WidthOverride(difficultyButtonWidth)
										.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
										[
											SNew(SButton)
											.HAlign(HAlign_Center).VAlign(VAlign_Center)
											.ButtonColorAndOpacity(this, &SMinesweeper::GetDifficultyColor, 2)
											.OnClicked(this, &SMinesweeper::OnDifficultyClick, 2)
											[
												SNew(STextBlock)
												.TextStyle(FMinesweeperStyle::Get(), "Text.Small")
												.Text(LOCTEXT("ExpertDifficultyLabel", "Expert"))
											]
										]
									]
								]

								// START NEW GAME BUTTON
								+ SVerticalBox::Slot().AutoHeight()
								.HAlign(HAlign_Center).VAlign(VAlign_Center)
								.Padding(0.0f, 10.0f, 0.0f, 0.0f)
								[
									SNew(SBox).WidthOverride(180)
									.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
									[
										SNew(SButton).HAlign(HAlign_Fill)
										.HAlign(HAlign_Center).VAlign(VAlign_Center)
										.OnClicked(this, &SMinesweeper::OnStartNewGameClick)
										[
											SNew(STextBlock)
											.TextStyle(FMinesweeperStyle::Get(), "Text.Normal")
											.Text(LOCTEXT("StartNewGameLabel", "Start New Game"))
										]
									]
								]

								// CONTINUE GAME BUTTON
								+ SVerticalBox::Slot().AutoHeight()
								.HAlign(HAlign_Center).VAlign(VAlign_Center)
								.Padding(0.0f, 10.0f, 0.0f, 0.0f)
								[
									SNew(SBox).WidthOverride(180)
									.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
									.Visibility_Lambda([&]() { return IsGameActive ? EVisibility::Visible : EVisibility::Collapsed; })
									[
										SNew(SButton).HAlign(HAlign_Fill)
										.HAlign(HAlign_Center).VAlign(VAlign_Center)
										.OnClicked(this, &SMinesweeper::OnContinueGameClick)
										[
											SNew(STextBlock)
											.TextStyle(FMinesweeperStyle::Get(), "Text.Normal")
											.Text(LOCTEXT("ContinueGameLabel", "Continue Game"))
										]
									]
								]
							]
						]

						// HIGH SCORES LIST
						+ SWidgetSwitcher::Slot()
						.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
						.Padding(5.0f, 5.0f, 5.0f, 0.0f)
						[
							SNew(SBorder)
							.BorderImage(FMinesweeperStyle::GetBrush("RoundedPanel"))
							.HAlign(HAlign_Fill).VAlign(VAlign_Center)
							.Padding(10.0f)
							[
								SAssignNew(HighScoresList, SVerticalBox)
							]
						]
					]
				]
			]
		]

		// GAME PANEL
		+ SWidgetSwitcher::Slot()
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
					SNew(SHorizontalBox)

					// NAME AND TIME PANEL
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					.HAlign(HAlign_Left).VAlign(VAlign_Bottom)
					.Padding(3.0f)
					[
						SNew(SBox).WidthOverride(130)
						.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot().AutoHeight()
							.HAlign(HAlign_Fill).VAlign(VAlign_Center)
							.Padding(0.0f, 0.0f, 0.0f, 2.0f)
							[
								SNew(STextBlock)
								.Justification(ETextJustify::Center)
								.TextStyle(FMinesweeperStyle::Get(), "Text.Normal.Bold")
								.Text_Lambda([&]() { return FText::FromString(Settings->LastPlayerName); })
							]
							+ SVerticalBox::Slot().AutoHeight()
							.HAlign(HAlign_Fill).VAlign(VAlign_Center)
							[
								SNew(SBorder)
								.HAlign(HAlign_Fill).VAlign(VAlign_Center)
								.BorderImage(FMinesweeperStyle::GetBrush("RoundedPanel"))
								.ToolTipText(LOCTEXT("TimeElapsedTooltip", "Elapsed game time in seconds."))
								[
									SNew(STextBlock)
									.Margin(FMargin(0, 1, 0, 0))
									.Justification(ETextJustify::Center)
									.Visibility(EVisibility::SelfHitTestInvisible)
									.TextStyle(FMinesweeperStyle::Get(), "Text.HeaderLarge")
									.Text(this, &SMinesweeper::GetTimerText)
								]
							]
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
							.OnClicked(this, &SMinesweeper::OnRestartGameClick)
							[
								SNew(SImage)
								.DesiredSizeOverride(FVector2D(56, 56))
								.ToolTipText(LOCTEXT("RestartGameTooltip", "Start a new game with the same settings."))
								.Image(this, &SMinesweeper::GetSmileImage)
							]
						//]
					]

					// FLAGS REMAINING
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
							.OnClicked(this, &SMinesweeper::OnGotoNewGamePanel)
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
							SNew(SBox).WidthOverride(60)
							.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
							[
								SNew(SVerticalBox)
								/*+ SVerticalBox::Slot().AutoHeight()
								.HAlign(HAlign_Fill).VAlign(VAlign_Center)
								.Padding(0.0f, 0.0f, 0.0f, 2.0f)
								[
									SNew(STextBlock)
									.Justification(ETextJustify::Center)
									.TextStyle(FMinesweeperStyle::Get(), "Text.Small")
									.Text(LOCTEXT("FlagsRemainingLabel", "Flags"))
								]*/
								+ SVerticalBox::Slot().AutoHeight()
								.HAlign(HAlign_Fill).VAlign(VAlign_Center)
								.Padding(0.0f, 0.0f, 0.0f, 0.0f)
								[
									SNew(SBorder)
									.HAlign(HAlign_Fill).VAlign(VAlign_Center)
									.BorderImage(FMinesweeperStyle::GetBrush("RoundedPanel"))
									.ToolTipText(LOCTEXT("FlagsRemainingTooltip", "Number of flags remaining that can be placed."))
									[
										SNew(STextBlock)
										.Margin(FMargin(0, 1, 0, 0))
										.Justification(ETextJustify::Center)
										.Visibility(EVisibility::SelfHitTestInvisible)
										.TextStyle(FMinesweeperStyle::Get(), "Text.HeaderLarge")
										.Text(this, &SMinesweeper::GetFlagsRemainingText)
									]
								]
							]
						]
					]
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
					SAssignNew(GridPanel, SUniformGridPanel)
					.MinDesiredSlotWidth(CellSize)
					.MinDesiredSlotHeight(CellSize)
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.HAlign(HAlign_Fill).VAlign(VAlign_Center)
					.Visibility(this, &SMinesweeper::GetWinLoseVisibility)
					.BorderBackgroundColor_Lambda([&]() { return HasWon() ? FMinesweeperStyle::GetColor("Color.Win") : FMinesweeperStyle::GetColor("Color.Lose"); })
					//.BorderImage([&]() { return HasWon() ? FMinesweeperStyle::GetBrush("Border.Win") : FMinesweeperStyle::GetBrush("Border.Lose"); })
					[
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
							.TextStyle(FMinesweeperStyle::Get(), "Text.WinLose")
							.Visibility_Lambda([&]() { return LastHighScoreRank > -1 ? EVisibility::SelfHitTestInvisible : EVisibility::Hidden; })
							.Text(LOCTEXT("NewHighScoreLabel", "New High Score!"))
						]
						+ SVerticalBox::Slot().AutoHeight()
						.HAlign(HAlign_Center).VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.TextStyle(FMinesweeperStyle::Get(), "Text.WinLose")
							.Visibility_Lambda([&]() { return LastHighScoreRank > -1 ? EVisibility::SelfHitTestInvisible : EVisibility::Hidden; })
							.Text_Lambda([&]() { return FText::Format(LOCTEXT("NewHighScoreRankLabel", "Rank {0}"), FText::FromString(FString::FromInt(LastHighScoreRank + 1))); })
						]
					]
				]
			]
		]
	];

	RefreshHighScores();
	
	ActiveTimerHandle = RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SMinesweeper::UpdateGameTick));
}


TSharedRef<SBorder> CreateHighScoreRow(const int32 InIndex, const FText& InName, const FText& InScore, const FText& InTime, const bool InIsHeader)
{
	const FName textStyle = InIsHeader ? "HighScoreList.Text.Header" : FName("HighScoreList.Text.Rank" + FString::FromInt(InIndex + 1));
	return
		SNew(SBorder)
		.BorderImage(FMinesweeperStyle::GetBrush(InIsHeader ? "HighScoreList.Header" : "HighScoreList.Row"))
		.HAlign(HAlign_Fill).VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().FillWidth(0.05f)
			.HAlign(HAlign_Center).VAlign(VAlign_Center)
			.Padding(5.0f, 0.0f, 5.0f, 0.0f)
			[
				SNew(STextBlock)
				.Visibility(InIsHeader ? EVisibility::Hidden : EVisibility::SelfHitTestInvisible)
				.TextStyle(FMinesweeperStyle::Get(), textStyle)
				.Text(FText::FromString(FString::FromInt(InIndex + 1)))
			]
			+ SHorizontalBox::Slot().FillWidth(0.5f)
			.HAlign(HAlign_Left).VAlign(VAlign_Center)
			.Padding(InIsHeader ? 10.0f : 5.0f, 0.0f, 5.0f, 0.0f)
			[
				SNew(STextBlock)
				.TextStyle(FMinesweeperStyle::Get(), textStyle)
				.Text(InName)
			]
			+ SHorizontalBox::Slot().FillWidth(0.25f)
			.HAlign(HAlign_Center).VAlign(VAlign_Center)
			.Padding(5.0f, 0.0f, 5.0f, 0.0f)
			[
				SNew(STextBlock)
				.TextStyle(FMinesweeperStyle::Get(), textStyle)
				.Text(InScore)
			]
			+ SHorizontalBox::Slot().FillWidth(0.2f)
			.HAlign(HAlign_Center).VAlign(VAlign_Center)
			.Padding(5.0f, 0.0f, 5.0f, 0.0f)
			[
				SNew(STextBlock)
				.TextStyle(FMinesweeperStyle::Get(), textStyle)
				.Text(InTime)
			]
		];
}

void SMinesweeper::RefreshHighScores()
{
	HighScoresList->ClearChildren();

	HighScoresList->AddSlot().AutoHeight()
		[
			CreateHighScoreRow(-1, 
				LOCTEXT("HighScoreNameColumnLabel", "Name"), 
				LOCTEXT("HighScoreScoreColumnLabel", "Score"),
				LOCTEXT("HighScoreTimeColumnLabel", "Time"),
				true
			)
		];

	for (int32 i = 0; i < Settings->HighScores.Num(); i++)
	{
		HighScoresList->AddSlot().AutoHeight()
			[
				CreateHighScoreRow(i, 
					FText::FromString(Settings->HighScores[i].Name), 
					FText::FromString(FString::FromInt(Settings->HighScores[i].Score)),
					FText::FromString(FString::FromInt(FMath::FloorToInt32(Settings->HighScores[i].Time))), 
					false
				)
			];
	}

	HighScoresList->AddSlot().AutoHeight()
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		.Padding(0.0f, 8.0f, 0.0f, 0.0f)
		[
			SNew(STextBlock)
			.TextStyle(FMinesweeperStyle::Get(), "Text.VerySmall")
			.Text(LOCTEXT("HighScoresForExpertOnlyLabel", "* High Scores are only recorded for Expert difficulty."))
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


EActiveTimerReturnType SMinesweeper::UpdateGameTick(double InCurrentTime, float InDeltaTime)
{
	if (IsGameActive && !IsGamePaused)
	{
		GameTime += InDeltaTime;
	}
	return EActiveTimerReturnType::Continue;
}


FString SanitizePlayerName(const FString InName)
{
	FString outName = "";

	const auto nameCharArray = InName.GetCharArray();
	const auto validCharArray = FString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_").GetCharArray();

	FString badChars = "";

	for (int32 i = 0; i < nameCharArray.Num(); i++)
	{
		if (i >= 20) break; // limit to 20 characters
		if (validCharArray.Contains(nameCharArray[i]))
		{
			outName += nameCharArray[i];
		}
	}

	return outName;
}

void SMinesweeper::OnPlayerNameChanged(const FText& NewText)
{
	Settings->LastPlayerName = SanitizePlayerName(NewText.ToString());
	NameTextBox->SetText(FText::FromString(Settings->LastPlayerName));
}

void SMinesweeper::OnPlayerNameCommitted(const FText& NewText, ETextCommit::Type InTextCommit)
{
	Settings->LastPlayerName = SanitizePlayerName(NewText.ToString());
}


FReply SMinesweeper::OnDifficultyClick(const int32 InDifficultyLevel)
{
	if (InDifficultyLevel == 0)
	{
		Settings->LastSettings = FMinesweeperDifficulty::Beginner;
	}
	else if (InDifficultyLevel == 1)
	{
		Settings->LastSettings = FMinesweeperDifficulty::Intermediate;
	}
	else if (InDifficultyLevel == 2)
	{
		Settings->LastSettings = FMinesweeperDifficulty::Expert;
	}
	else if (InDifficultyLevel == 3)
	{
		const FVector2D viewportSize = GEditor->GetActiveViewport()->GetSizeXY();
		Settings->LastSettings.Width = FMath::FloorToInt32(viewportSize.X / 32.0f);
		Settings->LastSettings.Height = FMath::FloorToInt32(viewportSize.Y / 32.0f);
		Settings->LastSettings.MineCount = FMath::CeilToInt32(Settings->LastSettings.TotalCells() * 0.25f);
	}

	return FReply::Handled();
}

FReply SMinesweeper::OnStartNewGameClick()
{
	CurrentGridSeed = FMath::Rand();
	CurrentSettings = Settings->LastSettings;

	//IsGameActive = true;
	GameTime = 0.0f;

	// Attempt to hide the game until the creation of slots and widgets. Trying to hide view of stretching controls during short lag spike.
	SetVisibility(EVisibility::Hidden);
	
	// clear all grid panel children and recreate all grid cells
	GridPanel->ClearChildren();
	CellMap.Empty();

	// add slots and widget to each slot
	for (int32 cellIndex = 0; cellIndex < TotalCellCount(); cellIndex++)
	{
		const FIntVector2 cellCoord = GridIndexToCoord(cellIndex);

		TSharedRef<SMinesweeperCell> newCellWidget =
			SNew(SMinesweeperCell, cellCoord)
			.OnCellLeftClick(this, &SMinesweeper::OnLeftClickCell)
			.OnCellRightClick(this, &SMinesweeper::OnRightClickCell);

		// to map for quick access by grid coord
		CellMap.Add(cellIndex, newCellWidget);

		// add widget to slot
		GridPanel->AddSlot(cellCoord.X, cellCoord.Y)
			[
				newCellWidget
			];
	}

	ActiveMainPanel = 1;

	// Show the game again after all slots and widgets have been created
	SetVisibility(EVisibility::SelfHitTestInvisible);

	return FReply::Handled();
}

FReply SMinesweeper::OnContinueGameClick()
{
	IsGamePaused = false;

	ActiveMainPanel = 1;

	return FReply::Handled();
}

FReply SMinesweeper::OnNewGameClick()
{
	// pause the game timer
	if (IsGameActive)
	{
		IsGamePaused = true;
	}

	ActiveMainPanel = 0;

	return FReply::Handled();
}

FReply SMinesweeper::OnRestartGameClick()
{
	IsGameActive = false;
	GameTime = 0.0f;

	// reset all cells to starting values
	ForEachCell([&](TSharedPtr<SMinesweeperCell> InCellWidget)
		{
			InCellWidget->Reset();
		});

	return FReply::Handled();
}

FReply SMinesweeper::OnSwitchWindowModeClick()
{
	FMinesweeperEditorModule::Get().ToggleMinesweeperWindowMode();

	{
		Settings->HighScores.Insert(FMinesweeperHighScore(Settings->LastPlayerName, 58045, 751, 355), 2);
		Settings->HighScores.RemoveAt(Settings->HighScores.Num() - 1);

		RefreshHighScores();
	}

	return FReply::Handled();
}

FReply SMinesweeper::OnGotoNewGamePanel()
{
	// pause the game timer
	if (IsGameActive)
	{
		IsGamePaused = true;
	}

	ActiveMainPanel = 0;

	return FReply::Handled();
}


FSlateColor SMinesweeper::GetDifficultyColor(const int32 InDifficultyLevel) const
{
	const FLinearColor transparentColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	const FLinearColor selectedColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);

	if (InDifficultyLevel == 0)
	{
		return CurrentSettings.IsBeginner() ? selectedColor : transparentColor;
	}
	else if (InDifficultyLevel == 1)
	{
		return CurrentSettings.IsIntermediate() ? selectedColor : transparentColor;
	}
	else if (InDifficultyLevel == 2)
	{
		return CurrentSettings.IsExpert() ? selectedColor : transparentColor;
	}
	else if (InDifficultyLevel == 3)
	{
		
	}

	return transparentColor;
}

FText SMinesweeper::GetTimerText() const
{
	return IsGameActive ? FText::FromString(FString::FromInt(FMath::FloorToInt32(GameTime))) : FText();
}

const FSlateBrush* SMinesweeper::GetSmileImage() const
{
	return FMinesweeperStyle::GetBrush((IsGameActive || GameTime == 0.0f || HasWon()) ? "MrSmile.Alive" : "MrSmile.Dead");
}

FText SMinesweeper::GetFlagsRemainingText() const
{
	return FText::FromString(FString::FromInt(FlagsRemaining));
}

EVisibility SMinesweeper::GetWinLoseVisibility() const
{
	return (!IsGameActive && GameTime > 0.0f) ? EVisibility::SelfHitTestInvisible : EVisibility::Hidden;
}

FSlateColor SMinesweeper::GetWinLoseColor() const
{
	return FMinesweeperStyle::GetColor(HasWon() ? "Color.Win" : "Color.Lose");
}

FText SMinesweeper::GetWinLoseText() const
{
	return HasWon() ? LOCTEXT("GameWinnerLabel", "Winner") : LOCTEXT("GameLoserLabel", "Loser");
}


void SMinesweeper::OnLeftClickCell(TSharedPtr<SMinesweeperCell> InCellWidget)
{
	if (!InCellWidget.IsValid()) return;

	if (IsGameActive && GameTime > 0.0f) // game is active and started
	{
		TotalClicks++;
		OpenCell(InCellWidget);

		if (InCellWidget->HasMine)
		{
			// the game has ended in a loser!
			IsGameActive = false;

			LastHighScoreRank = -1;

			// show all mines
			ForEachCell([&](TSharedPtr<SMinesweeperCell> InCellWidget)
				{
					InCellWidget->ShowMine = true;
				});
		}
		else
		{
			// check for win condition
			if (HasWon())
			{
				// the game has ended in a winner!
				IsGameActive = false;

				// only check for high score on expert setting
				if (Settings->LastSettings.IsExpert())
				{
					// calculate high score
					// less time is higher score, less clicks is higher score
					int32 score = FMath::FloorToInt32(((float)MaxScore / GameTime) + ((float)MaxScore / (float)TotalClicks));

					// check for new high score
					LastHighScoreRank = -1;
					for (int32 rank = 0; rank < Settings->HighScores.Num(); rank++)
					{
						if (score > Settings->HighScores[rank].Score)
						{
							LastHighScoreRank = rank;
							break;
						}
					}

					// insert the new high score into the list and remove the last rank if we have a new high score
					if (LastHighScoreRank > -1)
					{
						Settings->HighScores.Insert(FMinesweeperHighScore(Settings->LastPlayerName, score, GameTime, TotalClicks), LastHighScoreRank);
						Settings->HighScores.RemoveAt(Settings->HighScores.Num() - 1);

						RefreshHighScores();
					}
				}
			}
		}
	}
	else if (!IsGameActive && GameTime == 0.0f) // game is NOT active and has NOT started
	{
		// start of a new game
		IsGameActive = true;
		TotalClicks = 1;
		FlagsRemaining = Settings->LastSettings.MineCount;
		NumClosedCells = Settings->LastSettings.TotalCells();
		NumOpenedCells = 0;

		FRandomStream randStream(CurrentGridSeed);

		// calculate placement of mines after user clicks to avoid the user ever clicked a mine on the first click
		const int32 totalCellCount = NumClosedCells;
		int32 minesToPlace = FlagsRemaining;
		while (minesToPlace > 0)
		{
			int32 randIndex = randStream.RandRange(0, totalCellCount - 1);
			FIntVector2 gridCoord = GridIndexToCoord(randIndex);

			TSharedPtr<SMinesweeperCell> cellWidget = GetCellWidget(gridCoord);
			if (!cellWidget.IsValid()) break;

			if (cellWidget != InCellWidget && !cellWidget->HasMine)
			{
				cellWidget->HasMine = true;
				minesToPlace--;
			}
		}

		// calculate neighboring mine counts for each cell
		int32 currentNeighborMineCount = 0;
		for (int32 cellIndex = 0; cellIndex < TotalCellCount(); cellIndex++)
		{
			const FIntVector2 cellCoord = GridIndexToCoord(cellIndex);

			TArray<TSharedPtr<SMinesweeperCell>> neighborCells = GetNeighborCellWidgets(cellCoord);

			currentNeighborMineCount = 0;
			for (uint32 i = 0; i < (uint32)neighborCells.Num(); i++)
			{
				if (neighborCells[i]->HasMine) currentNeighborMineCount++;
			}

			auto cellWidget = GetCellWidget(cellCoord);
			if (cellWidget.IsValid())
			{
				cellWidget->NeighborMineCount = currentNeighborMineCount;
			}
		}

		OpenCell(InCellWidget);
	}
}

void SMinesweeper::OnRightClickCell(TSharedPtr<SMinesweeperCell> InCellWidget)
{
	if (!InCellWidget.IsValid()) return;

	TotalClicks++;

	if (InCellWidget->IsFlagged)
	{
		if (FlagsRemaining > 0)
		{
			FlagsRemaining--;

			// disable flagging for all cells if they have used them all
			if (FlagsRemaining == 0)
			{
				ForEachCell([&](TSharedPtr<SMinesweeperCell> InCellWidget)
					{
						InCellWidget->CanPlaceFlag = false;
					});
			}
		}
	}
	else
	{
		// enable flagging for all cells
		if (FlagsRemaining == 0)
		{
			ForEachCell([&](TSharedPtr<SMinesweeperCell> InCellWidget)
				{
					InCellWidget->CanPlaceFlag = true;
				});
		}

		FlagsRemaining++;
	}
}



#pragma region Grid / Cell Functions

bool SMinesweeper::IsValidGridIndex(const int32 InCellIndex) const
{
	return InCellIndex >= 0 && InCellIndex < TotalCellCount();
}

bool SMinesweeper::IsValidGridCoord(const FIntVector2 InCellCoord) const
{
	return InCellCoord.X >= 0 && InCellCoord.Y >= 0 && InCellCoord.X < CurrentSettings.Width && InCellCoord.Y < CurrentSettings.Height;
}

int32 SMinesweeper::GridCoordToIndex(const FIntVector2 InCellCoord) const
{
	return (CurrentSettings.Width * InCellCoord.Y) + InCellCoord.X;
}

FIntVector2 SMinesweeper::GridIndexToCoord(const int32 InCellIndex) const
{
	return FIntVector2(FMath::FloorToInt32((float)(InCellIndex % CurrentSettings.Width)), FMath::FloorToInt32((float)InCellIndex / (float)CurrentSettings.Width));
}

TSharedPtr<SMinesweeperCell> SMinesweeper::GetCellWidget(const FIntVector2 InCellCoord)
{
	if (!IsValidGridCoord(InCellCoord)) return nullptr;

	const int32 cellIndex = GridCoordToIndex(InCellCoord);
	if (CellMap.Contains(cellIndex))
	{
		return CellMap[cellIndex];
	}
		
	return nullptr;
}

TArray<TSharedPtr<SMinesweeperCell>> SMinesweeper::GetNeighborCellWidgets(const FIntVector2 InCellCoord)
{
	TArray<TSharedPtr<SMinesweeperCell>> outCellWidgets;

	// 2d neighboring tile offsets mapped to array indices
	// 0 - 1 - 2
	// 3 - 4 - 5
	// 6 - 7 - 8

	const TArray<FIntVector2> Offsets = {
		FIntVector2(-1,-1), FIntVector2(0,-1), FIntVector2(1,-1), 
		FIntVector2(-1, 0), FIntVector2(0, 0), FIntVector2(1, 0), 
		FIntVector2(-1, 1), FIntVector2(0, 1), FIntVector2(1, 1) };

	for (int32 i = 0; i < 9; i++)
	{
		if (i == 4) continue; // skip middle cell (self)
		
		// FIntVector2 doesn't have a + operator (???), so adding components individually
		FIntVector2 cellCoord(InCellCoord.X + Offsets[i].X, InCellCoord.Y + Offsets[i].Y);

		// all cells that are outside of the grid bounds will return null here and not be added
		TSharedPtr<SMinesweeperCell> cellWidget = GetCellWidget(cellCoord);
		if (cellWidget.IsValid())
		{
			outCellWidgets.Add(cellWidget);
		}
	}

    return outCellWidgets;
}

void SMinesweeper::OpenCell(TSharedPtr<SMinesweeperCell> InCellWidget)
{
	InCellWidget->IsOpened = true;
	NumClosedCells--;
	NumOpenedCells++;

	if (InCellWidget->NeighborMineCount == 0)
	{
		OpenNeighbors(InCellWidget);
	}
}

void SMinesweeper::OpenNeighbors(TSharedPtr<SMinesweeperCell> InCellWidget)
{
	TArray<TSharedPtr<SMinesweeperCell>> neighborCellWidgets = GetNeighborCellWidgets(InCellWidget->CellCoordinates);
	for (auto widget : neighborCellWidgets)
	{
		if (!widget->IsOpened)
		{
			OpenCell(widget);
		}
	}
}

void SMinesweeper::ForEachCell(TFunctionRef<void(TSharedPtr<SMinesweeperCell>)> InFunc)
{
	for (int32 cellIndex = 0; cellIndex < TotalCellCount(); cellIndex++)
	{
		FIntVector2 gridCoord = GridIndexToCoord(cellIndex);
		TSharedPtr<SMinesweeperCell> cellWidget = GetCellWidget(gridCoord);
		if (cellWidget.IsValid())
		{
			InFunc(cellWidget);
		}
	}
}

#pragma endregion




#undef LOCTEXT_NAMESPACE
