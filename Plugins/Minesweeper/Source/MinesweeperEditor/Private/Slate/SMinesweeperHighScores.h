// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SBoxPanel.h"
#include "MinesweeperSettings.h"




/**
 * Minesweeper High Scores List
 */
class MINESWEEPEREDITOR_API SMinesweeperHighScores : public SVerticalBox
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperHighScores)
		: _HighScores(nullptr)
	{ }

		SLATE_ARGUMENT(const TArray<FMinesweeperHighScore>*, HighScores)

	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);


	void SetHighScores(const TArray<FMinesweeperHighScore>* InHighScores);

};
