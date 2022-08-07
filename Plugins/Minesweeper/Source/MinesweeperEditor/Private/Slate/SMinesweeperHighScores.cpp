// Copyright 2022 Brad Monahan. All Rights Reserved.
#include "Slate/SMinesweeperHighScores.h"
#include "MinesweeperStyle.h"
#include "SlateOptMacros.h"


#define LOCTEXT_NAMESPACE "SMinesweeperHighScores"




BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMinesweeperHighScores::Construct(const FArguments& InArgs)
{
	SVerticalBox::Construct(
		SVerticalBox::FArguments()
	);

	SetHighScores(InArgs._HighScores);
}


TSharedRef<SBorder> CreateHighScoreRow(const int32 InIndex, const FText& InName, const FText& InScore, const FText& InTime, const bool InIsHeader)
{
	const FName textStyle = InIsHeader ? "HighScoreList.Text.Header" : FName("HighScoreList.Text.Rank" + FString::FromInt(InIndex + 1));
	return
		SNew(SBorder)
		.BorderImage(FMinesweeperStyle::GetBrush(InIsHeader ? "HighScoreList.Header" : "TransparentBorder"))
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
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SMinesweeperHighScores::SetHighScores(const TArray<FMinesweeperHighScore>* InHighScores)
{
	ClearChildren();

	if (!InHighScores) return;


	const TArray<FMinesweeperHighScore>& highScores = *InHighScores;

	AddSlot().AutoHeight()
		[
			CreateHighScoreRow(-1, 
				LOCTEXT("HighScoreNameColumnLabel", "Name"), 
				LOCTEXT("HighScoreScoreColumnLabel", "Score"),
				LOCTEXT("HighScoreTimeColumnLabel", "Time"),
				true
			)
		];

	for (int32 i = 0; i < highScores.Num(); ++i)
	{
		AddSlot().AutoHeight()
			[
				CreateHighScoreRow(i, 
					FText::FromString(highScores[i].Name),
					FText::FromString(FString::FromInt(highScores[i].Score)),
					FText::FromString(FString::FromInt(FMath::FloorToInt32(highScores[i].Time))),
					false
				)
			];
	}

	AddSlot().AutoHeight()
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		.Padding(0.0f, 8.0f, 0.0f, 0.0f)
		[
			SNew(STextBlock)
			.TextStyle(FMinesweeperStyle::Get(), "Text.VerySmall")
			.Text(LOCTEXT("HighScoresForExpertOnlyLabel", "* High Scores are only recorded for Expert difficulty."))
		];
}




#undef LOCTEXT_NAMESPACE
